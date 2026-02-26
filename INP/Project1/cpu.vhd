-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2025 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): jmeno <xdrabbo00 AT stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector (12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector (7 downto 0);  -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector (7 downto 0);   -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                     -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                     -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector (7 downto 0);    -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                       -- data platna
   IN_REQ    : out std_logic;                      -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector (7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_INV  : out std_logic;                      -- pozadavek na aktivaci inverzniho zobrazeni (1)
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

---------------------------------------------------------------------------//
--Konštanty pre inštruk.
---------------------------------------------------------------------------//
constant OP_PTR_INC    : std_logic_vector (7 downto 0) := x"3E";
constant OP_PTR_DEC    : std_logic_vector (7 downto 0) := x"3C";
constant OP_VAL_INC    : std_logic_vector (7 downto 0) := x"2B";
constant OP_VAL_DEC    : std_logic_vector (7 downto 0) := x"2D";
constant OP_PRINT    : std_logic_vector (7 downto 0) := x"2E";
constant OP_INPUT    : std_logic_vector (7 downto 0) := x"2C";
constant OP_WHILE_LEF    : std_logic_vector (7 downto 0) := x"5B";
constant OP_WHILE_RIG    : std_logic_vector (7 downto 0) := x"5D";
constant OP_DO_LEF    : std_logic_vector (7 downto 0) := x"28";
constant OP_DO_RIG    : std_logic_vector (7 downto 0) := x"29";
constant OP_HALT    : std_logic_vector (7 downto 0) := x"40";

--------------------------------------------------------------------------//  
-- Registre
--------------------------------------------------------------------------//  
signal pc   : std_logic_vector (12 downto 0);  -- program counter
signal ptr  : std_logic_vector (12 downto 0);  -- data pointer
signal cnt  : std_logic_vector (7 downto 0);   -- hĺbka vnorenia slučiek

signal fastinit_val   : std_logic_vector (7 downto 0);
signal data_wdata_int : std_logic_vector (7 downto 0);

-------------------------------------------------------------------------//

signal pc_inc       : std_logic;
signal pc_dec       : std_logic;
signal ptr_inc      : std_logic;
signal ptr_dec      : std_logic;
signal cnt_inc      : std_logic;
signal cnt_dec      : std_logic;
signal cnt_clr      : std_logic;

signal ready_reg         : std_logic;
signal ready_set         : std_logic;

signal fastinit_en       : std_logic;

-----------------------------------------------

signal addr_sel  : std_logic;                
signal wdata_sel : std_logic_vector (1 downto 0);
signal hex_data  : std_logic_vector (7 downto 0);

--------------------------------------------------------------------------//  
-- Final State Machine stavy
---------------------------------------------------------------------------//  
type fsm_state is (
  S_RESET,
  S_INIT_READ,
  S_INIT,
  S_FETCH,
  S_DECODE,

  S_INC_VALUE,
  S_DEC_VALUE,

  S_PRINT,
  S_INPUT,

  S_WHILE_LEF_CHECK,
  S_WHILE_LEF_SKIP,

  S_WHILE_LEF_SKIP_SCAN,

  S_WHILE_RIG_CHECK,
  S_WHILE_RIG_SKIP,

  S_WHILE_RIG_SKIP_SCAN,

  S_DO_RIG_CHECK,
  S_DO_RIG_SKIP,

  S_DO_RIG_SKIP_SCAN,

  S_HALT
);

signal state : fsm_state;
signal next_state : fsm_state;

begin

------------------------------------------------------------------------//  
-- PROGRAM Counter (PC register)
------------------------------------------------------------------------//  
pc_reg : process (CLK, RESET)
begin
  if RESET = '1' then
    pc <= "0000000000000";
  elsif rising_edge(CLK) then
    if EN = '1' then
      if pc_inc = '1' then
        if pc = "1111111111111" then
          pc <= "0000000000000";
        else
          pc <= pc + 1;
        end if;
      elsif pc_dec = '1' then
        if pc = "0000000000000" then
          pc <= "1111111111111";
        else
          pc <= pc - 1;
        end if;
      end if;
    end if;
  end if;
end process;

---------------------------------------------------------------------------//  
-- DATA POINTER
---------------------------------------------------------------------------//  
ptr_reg : process (CLK, RESET)
begin
  if RESET = '1' then
    ptr <= "0000000000000";
  elsif rising_edge (CLK) then
    if EN = '1' then
      if ptr_inc = '1' then
        if ptr = "1111111111111" then
          ptr <= "0000000000000";
        else
          ptr <= ptr + 1;
        end if;
      elsif ptr_dec = '1' then
        if ptr = "0000000000000" then
          ptr <= "1111111111111";
        else
          ptr <= ptr - 1;
        end if;
      end if;
    end if;
  end if;
end process;

--------------------------------------------------------------------------//  
-- CNT register (counter pre slučky (vnorenosť))
---------------------------------------------------------------------------//  
cnt_reg : process (CLK, RESET)
begin
  if RESET = '1' then
    cnt <= "00000000";
  elsif rising_edge (CLK) then
    if EN = '1' then
      if cnt_clr = '1' then
        cnt <= "00000000";
      elsif cnt_inc = '1' then
        cnt <= cnt + 1;
      elsif cnt_dec = '1' then
        cnt <= cnt - 1;
      end if;
    end if;
  end if;
end process;

---------------------------------------------------------------------------//  
-- Multiplexor adresy RAM
---------------------------------------------------------------------------//  
DATA_ADDR <= ptr 
  when addr_sel = '0' 
  else pc;

--------------------------------------------------------------------------//  
-- Multiplex. zapisovania dat do RAM
-------------------------------------------------------------------------- // 
with wdata_sel select
  data_wdata_int <=
    IN_DATA         when "00",   -- vstup 
    hex_data        when "01",
    DATA_RDATA - 1  when "10",   -- decrement
    DATA_RDATA + 1  when "11",   -- increment
    "00000000"  when others;

DATA_WDATA <= fastinit_val 
  when fastinit_en = '1' 
  else data_wdata_int;

--------------------------------------------------------------------------//
-- Register pre READY flag
--------------------------------------------------------------------------//
ready_reg_process : process (CLK, RESET)
begin
  if RESET = '1' then
    ready_reg <= '0';
  elsif rising_edge (CLK) then
    if EN = '1' then
      if ready_set = '1' then
        ready_reg <= '1';
      end if;
    end if;
  end if;
end process;

READY <= ready_reg;

--------------------------------------------------------------------------//
-- Stavovy register fsm
--------------------------------------------------------------------------//
fsm_reg : process (CLK, RESET)
begin
  if RESET = '1' then
    state <= S_RESET;
  elsif rising_edge (CLK) then
    if EN = '1' then
      state <= next_state;
    end if;
  end if;
end process;

--------------------------------------------------------------------------// 
-- HEX Prevodník

hex_decode_proc : process (DATA_RDATA)
begin
  case DATA_RDATA is
    -- 0 - 9
    when x"30" => hex_data <= x"00";
    when x"31" => hex_data <= x"10";
    when x"32" => hex_data <= x"20";
    when x"33" => hex_data <= x"30";
    when x"34" => hex_data <= x"40";
    when x"35" => hex_data <= x"50";
    when x"36" => hex_data <= x"60";
    when x"37" => hex_data <= x"70";
    when x"38" => hex_data <= x"80";
    when x"39" => hex_data <= x"90";

    -- A - F
    when x"41" => hex_data <= x"A0";
    when x"42" => hex_data <= x"B0";
    when x"43" => hex_data <= x"C0";
    when x"44" => hex_data <= x"D0";
    when x"45" => hex_data <= x"E0";
    when x"46" => hex_data <= x"F0";

    when others =>
      hex_data <= "00000000";
  end case;
 end process;

----------------------------------------------------------------------------// 
-- KOMBIN. LOGIKA FSM
----------------------------------------------------------------------------/-/ 

fsm_comb : process (state, DATA_RDATA, IN_VLD, OUT_BUSY, cnt)
begin

  -- Default hodnoty riadiacích signálov
  pc_inc    <= '0';
  pc_dec    <= '0';
  ptr_inc   <= '0';
  ptr_dec   <= '0';
  cnt_inc   <= '0';
  cnt_dec   <= '0';
  cnt_clr   <= '0';

  addr_sel  <= '1';
  wdata_sel <= "00";
  DATA_EN   <= '0';
  DATA_RDWR <= '1';

  IN_REQ    <= '0';
  OUT_WE    <= '0';
  OUT_INV   <= '0';
  OUT_DATA  <= "00000000";

  ready_set    <= '0';
  DONE         <= '0';
  fastinit_en  <= '0';
  fastinit_val <= "00000000";

  next_state <= state;

  case state is

    --------------------------------------------------------------------
    when S_RESET =>                 -- pripravime RAM čitanie pre init fázu.
      DATA_EN   <= '0';
      DATA_RDWR <= '1';
      next_state <= S_INIT_READ;

    --------------------------------------------------------------------
    when S_INIT_READ =>
      addr_sel  <= '0';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';
      next_state <= S_INIT;

    --------------------------------------------------------------------
    when S_INIT =>                      -- postupujeme, kým nenájdeme halt.
      addr_sel  <= '0';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';

      if DATA_RDATA = OP_HALT then          -- @ halt
        ptr_inc   <= '1';
        ready_set <= '1';
        next_state <= S_FETCH;
      else
        ptr_inc   <= '1';
        next_state <= S_INIT_READ;
      end if;

    --------------------------------------------------------------------
    when S_FETCH =>                         -- načítanie inštruk.
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';
      next_state <= S_DECODE;

    --------------------------------------------------------------------
    when S_DECODE =>                               -- dekodér - rozcestník na príslušný stav.
      if DATA_RDATA = OP_PTR_INC then              -- '>' PTR zvýš.
        ptr_inc   <= '1';
        pc_inc    <= '1';
        next_state <= S_FETCH;

      elsif DATA_RDATA = OP_PTR_DEC then           -- '<' PTR zníž.
        ptr_dec   <= '1';
        pc_inc    <= '1';
        next_state <= S_FETCH;

      elsif DATA_RDATA = OP_VAL_INC then           -- '+' value zvýš.
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_INC_VALUE;

      elsif DATA_RDATA = OP_VAL_DEC then           -- '-' value  zníž.
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_DEC_VALUE;

      elsif DATA_RDATA = OP_PRINT then           -- '.' vytlač hodnotu
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_PRINT;

      elsif DATA_RDATA = OP_INPUT then           -- ',' vstup
        IN_REQ    <= '1';
        next_state <= S_INPUT;

      elsif DATA_RDATA = OP_WHILE_LEF then           -- '['  ľavá zátvorka
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_WHILE_LEF_CHECK;

      elsif DATA_RDATA = OP_WHILE_RIG then          -- ']' pravá zátvorkaa
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_WHILE_RIG_CHECK;

      elsif DATA_RDATA = OP_DO_LEF then               -- '(' do-while ľavá zátvorka
        pc_inc    <= '1';
        next_state <= S_FETCH;

      elsif DATA_RDATA = OP_DO_RIG then               -- ')' do-while pravá zátvorka
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_DO_RIG_CHECK;

      elsif (DATA_RDATA >= x"30" and DATA_RDATA <= x"39") or        -- Hex hodnota
            (DATA_RDATA >= x"41" and DATA_RDATA <= x"46") then
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '0';
        wdata_sel <= "01";
        pc_inc    <= '1';
        next_state <= S_FETCH;

      elsif DATA_RDATA = OP_HALT then                           -- halt
        DONE      <= '1';
        next_state <= S_HALT;

      else
        pc_inc    <= '1';
        next_state <= S_FETCH;
      end if;

    --------------------------------------------------------------------
    --------------------------------------------------------------------
    when S_INC_VALUE =>                   -- navýšenie hodnoty v bunke
      addr_sel  <= '0';
      DATA_EN   <= '1';
      DATA_RDWR <= '0';
      wdata_sel <= "11";
      pc_inc    <= '1';
      next_state <= S_FETCH;

    --------------------------------------------------------------------
    when S_DEC_VALUE =>                   -- zníženie hodnoty v bunke
      addr_sel  <= '0';
      DATA_EN   <= '1';
      DATA_RDWR <= '0';
      wdata_sel <= "10";
      pc_inc    <= '1';
      next_state <= S_FETCH;

    --------------------------------------------------------------------
    when S_PRINT =>
      addr_sel  <= '0';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';

      if OUT_BUSY = '0' then
        OUT_DATA <= DATA_RDATA;
        OUT_WE   <= '1';
        pc_inc   <= '1';
        next_state <= S_FETCH;
      else
        next_state <= S_PRINT;
      end if;

    --------------------------------------------------------------------
    when S_INPUT =>
      IN_REQ <= '1';

      if IN_VLD = '1' then
        addr_sel  <= '0';
        DATA_EN   <= '1';
        DATA_RDWR <= '0';
        wdata_sel <= "00";
        pc_inc    <= '1';
        next_state <= S_FETCH;
      else
        next_state <= S_INPUT;
      end if;

    --------------------------------------------------------------------
    when S_WHILE_LEF_CHECK =>           -- overenie podmienky -> preskakujeme celý blok [ ], inak normálne pokračujeme 

      if DATA_RDATA = x"00" then
        cnt_clr   <= '1';
        pc_inc    <= '1';
        addr_sel  <= '1';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_WHILE_LEF_SKIP;
      else
        pc_inc    <= '1';
        next_state <= S_FETCH;
      end if;

    --------------------------------------------------------------------
    when S_WHILE_LEF_SKIP =>            -- zbierame vnorenosť slučiek
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';
      next_state <= S_WHILE_LEF_SKIP_SCAN;

    --------------------------------------------------------------------
    when S_WHILE_LEF_SKIP_SCAN =>
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';

      if DATA_RDATA = OP_WHILE_LEF then
        cnt_inc   <= '1';
        pc_inc    <= '1';
        next_state <= S_WHILE_LEF_SKIP;

      elsif DATA_RDATA = OP_WHILE_RIG then
        if cnt = x"00" then
          pc_inc    <= '1';
          next_state <= S_FETCH;
        else
          cnt_dec   <= '1';
          pc_inc    <= '1';
          next_state <= S_WHILE_LEF_SKIP;
        end if;

      elsif DATA_RDATA = OP_HALT then
        DONE      <= '1';
        next_state <= S_HALT;

      else
        pc_inc    <= '1';
        next_state <= S_WHILE_LEF_SKIP;
      end if;

    --------------------------------------------------------------------
    when S_WHILE_RIG_CHECK =>
      if DATA_RDATA = x"00" then
        pc_inc    <= '1';
        next_state <= S_FETCH;
      else
        cnt_clr   <= '1';
        pc_dec    <= '1';
        addr_sel  <= '1';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_WHILE_RIG_SKIP;
      end if;

    --------------------------------------------------------------------
    when S_WHILE_RIG_SKIP =>
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';
      next_state <= S_WHILE_RIG_SKIP_SCAN;

    --------------------------------------------------------------------
    when S_WHILE_RIG_SKIP_SCAN =>
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';

      if DATA_RDATA = OP_WHILE_RIG then
        cnt_inc  <= '1';
        pc_dec   <= '1';
        next_state <= S_WHILE_RIG_SKIP;

      elsif DATA_RDATA = OP_WHILE_LEF then
        if cnt = x"00" then
          pc_inc    <= '1';
          next_state <= S_FETCH;

        else
          cnt_dec <= '1';
          pc_dec  <= '1';
          next_state <= S_WHILE_RIG_SKIP;
        end if;

      else
        pc_dec    <= '1';
        next_state <= S_WHILE_RIG_SKIP;
      end if;

    --------------------------------------------------------------------
    when S_DO_RIG_CHECK =>              -- implementácia DO-WHILE
      if DATA_RDATA = x"00" then
        pc_inc    <= '1';
        next_state <= S_FETCH;
      else
        cnt_clr   <= '1';
        pc_dec    <= '1';
        addr_sel  <= '1';
        DATA_EN   <= '1';
        DATA_RDWR <= '1';
        next_state <= S_DO_RIG_SKIP;
      end if;

    --------------------------------------------------------------------
    when S_DO_RIG_SKIP  =>
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';
      next_state <= S_DO_RIG_SKIP_SCAN;

    --------------------------------------------------------------------
    when S_DO_RIG_SKIP_SCAN  =>
      addr_sel  <= '1';
      DATA_EN   <= '1';
      DATA_RDWR <= '1';

      if DATA_RDATA = OP_DO_RIG then
        cnt_inc   <= '1';
        pc_dec    <= '1';
        next_state <= S_DO_RIG_SKIP;

      elsif DATA_RDATA = OP_DO_LEF then
        if cnt = x"00" then
          pc_inc    <= '1';
          next_state <= S_FETCH;
        else
          cnt_dec   <= '1';
          pc_dec    <= '1';
          next_state <= S_DO_RIG_SKIP;
        end if;

      else
        pc_dec    <= '1';
        next_state <= S_DO_RIG_SKIP;
      end if;

    --------------------------------------------------------------------//

    when S_HALT =>            -- konečný stav
      DONE <= '1';
      next_state <= S_HALT;

  end case;
end process;

end behavioral;
