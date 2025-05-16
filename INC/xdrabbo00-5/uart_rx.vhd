-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Boris Nicolas Dráb (xdrabbo00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;


-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK          : in std_logic;
        RST          : in std_logic;
        DIN          : in std_logic;
        DOUT         : out std_logic_vector(7 downto 0);
        DOUT_VLD     : out std_logic
    );
end entity;


-------- Architecture implementation --------
architecture behavioral of UART_RX is

    signal START_FLAG   : std_logic;
    signal STOP_FLAG    : std_logic;
    signal LAST_BIT     : std_logic;

    signal START_COUNT  : std_logic_vector(2 downto 0);
    signal STOP_COUNT   : std_logic_vector(3 downto 0);
    signal CLK_COUNT    : std_logic_vector(3 downto 0);
    
    signal BIT_ADDRESS  : std_logic_vector(2 downto 0);
    signal DOUT_DEC     : std_logic_vector(7 downto 0);
    signal SAMPLE_FLAG  : std_logic;

    signal MEM_EN_O     : std_logic;
    signal START_EN_O   : std_logic;
    signal STOP_EN_O    : std_logic;
    signal COUNT_EN_O   : std_logic;
    
begin

    -------- Instance of RX FSM --------
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        START => START_FLAG,
        LASTBIT => LAST_BIT,
        STOP => STOP_FLAG,
        START_EN => START_EN_O,
        STOP_EN => STOP_EN_O,
        CNT_EN => COUNT_EN_O,
        MEM_EN => MEM_EN_O
    );


    -------- Start counter --------
    start_cnt: process(CLK, START_EN_O)
    begin
        if START_EN_O = '0' then
            START_COUNT <= (others => '0');
        elsif rising_edge(CLK) then
            if START_EN_O = '1' then
                START_COUNT <= START_COUNT + 1;
            end if;
        end if;
    end process;


    -------- Start comparator --------
    start_cmp: process(CLK, START_EN_O, START_COUNT)
    begin
        if START_EN_O = '0' then
            START_FLAG <= '0';
        elsif rising_edge(CLK) then
            if START_EN_O = '1' and  START_COUNT = "111" then
                START_FLAG <= '1';
            else
                START_FLAG <= '0';
            end if;
        end if;
    end process;


    -------- Stop-bit counter --------
    stop_cnt: process(CLK, STOP_EN_O)
    begin
        if STOP_EN_O = '0' then
            STOP_COUNT <= (others => '0');
        elsif rising_edge(CLK) then
            if STOP_EN_O = '1' then
                STOP_COUNT <= STOP_COUNT + 1;
            end if;
        end if;
    end process;


    -------- Stop comparator --------
    stop_cmp:process(CLK, STOP_EN_O, STOP_COUNT)
    begin
        if STOP_EN_O = '0' then
            STOP_FLAG <= '0';
        elsif rising_edge(CLK) then
            if STOP_EN_O = '1' and STOP_COUNT = "1110" then
                STOP_FLAG <= '1';
            else
                STOP_FLAG <= '0';
            end if;
        end if;
    end process;


    -------- 16x clock counter --------
    clk_cnt: process(CLK, COUNT_EN_O)
    begin
        if COUNT_EN_O = '0' then
            CLK_COUNT <= (others => '0');
        elsif rising_edge(CLK) then
            if COUNT_EN_O = '1' then
                CLK_COUNT <= CLK_COUNT + 1;
            end if;
        end if;
    end process;


    -------- mid-bit sample comparator --------
    bit_cmp: process(CLK, COUNT_EN_O, CLK_COUNT)
    begin
        if COUNT_EN_O = '0' then
            SAMPLE_FLAG <= '0';
        elsif rising_edge(CLK) then
            if COUNT_EN_O = '1' and CLK_COUNT = "1101" then
                SAMPLE_FLAG <= '1';
            else
                SAMPLE_FLAG <= '0';
            end if;
        end if;
    end process;


    -------- bit address counter --------
    BIT_ADDRESS_cnt: process(CLK, SAMPLE_FLAG, COUNT_EN_O)
    begin
        if COUNT_EN_O = '0' then
            BIT_ADDRESS <= (others => '0');
        elsif rising_edge(CLK) then
            if SAMPLE_FLAG = '1' then
                BIT_ADDRESS <= BIT_ADDRESS + 1;
            end if;
        end if;
    end process;


    -------- last-bit comparator --------
    bit_address_cmp: process(SAMPLE_FLAG, COUNT_EN_O, BIT_ADDRESS)
    begin
        if COUNT_EN_O = '0' then
            LAST_BIT <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if COUNT_EN_O = '1' and BIT_ADDRESS = "111" then
                LAST_BIT <= '1';
            else
                LAST_BIT <= '0';
            end if;
        end if;
    end process;


    -------- output decoder --------
    out_dec: process(BIT_ADDRESS)
    begin
        DOUT_DEC <= "00000000";
        if MEM_EN_O = '0' then
            DOUT_DEC <= (others => '0');
        end if;
        case BIT_ADDRESS is
            when "111" => DOUT_DEC <= "10000000";
            when "110" => DOUT_DEC <= "01000000";
            when "101" => DOUT_DEC <= "00100000";
            when "100" => DOUT_DEC <= "00010000";
            when "011" => DOUT_DEC <= "00001000";
            when "010" => DOUT_DEC <= "00000100";
            when "001" => DOUT_DEC <= "00000010";
            when "000" => DOUT_DEC <= "00000001";
            when others => DOUT_DEC <= (others => '0');
        end case;
    end process;


    -------- output registers --------
    out_reg0: process(SAMPLE_FLAG, MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(0) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(0) = '1' then
                DOUT(0) <= DIN;
            end if;
        end if;
    end process;  

    out_reg1: process(SAMPLE_FLAG, MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(1) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(1) = '1' then
                DOUT(1) <= DIN;
            end if;
        end if;
    end process;

    out_reg2: process(SAMPLE_FLAG, MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(2) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(2) = '1' then
                DOUT(2) <= DIN;
            end if;
        end if;
    end process;    

    out_reg3: process(SAMPLE_FLAG, MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(3) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(3) = '1' then
                DOUT(3) <= DIN;
            end if;
        end if;
    end process;   

    out_reg4: process(SAMPLE_FLAG, MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(4) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(4) = '1' then
                DOUT(4) <= DIN;
            end if;
        end if;
    end process;  

    out_reg5: process(SAMPLE_FLAG, MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(5) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(5) = '1' then
                DOUT(5) <= DIN;
            end if;
        end if;
    end process;

    out_reg6: process(SAMPLE_FLAG,MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(6) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(6) = '1' then
                DOUT(6) <= DIN;
            end if;
        end if;
    end process;

    out_reg7: process(SAMPLE_FLAG,MEM_EN_O)
    begin
        if MEM_EN_O = '0' then
            DOUT(7) <= '0';
        elsif rising_edge(SAMPLE_FLAG) then
            if DOUT_DEC(7) = '1' then
                DOUT(7) <= DIN;
            end if;
        end if;
    end process;  

    DOUT_VLD <= STOP_FLAG;

end architecture;