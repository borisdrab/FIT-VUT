-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Boris Nicolas Dráb (xdrabbo00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity UART_RX_FSM is
    port(
       CLK        : in std_logic;
       RST        : in std_logic;
       DIN        : in std_logic;
       START      : in std_logic;
       LASTBIT    : in std_logic;
       STOP       : in std_logic;
       START_EN   : out std_logic;
       STOP_EN    : out std_logic;
       CNT_EN     : out std_logic;
       MEM_EN     : out std_logic);
end entity;

architecture behavioral of UART_RX_FSM is

     type state_type is (S_IDLE, S_START_BIT, S_DATA, S_STOP_BIT);
     signal state_reg : state_type;
     signal state_next : state_type:= S_IDLE;

begin

    -------- State register --------

    register_state: process(CLK, RST)
    begin
        if rising_edge(CLK) then
            if RST = '1' then
                state_reg <= S_IDLE;
            else
                state_reg <= state_next;
            end if;
        end if;
    end process;

    -------- Next state logic --------

    next_state_logic: process(state_reg, DIN, START, LASTBIT, STOP, RST)
    begin
        state_next <= state_reg;
        if RST = '1' then
            state_next <= S_IDLE;
        else
            case state_reg is
                when S_IDLE =>
                    if DIN = '0' then
                        state_next <= S_START_BIT;
                    end if;
                when S_START_BIT =>
                    if START = '1' then
                        state_next <= S_DATA;
                    end if;
                when S_DATA =>
                    if LASTBIT = '1' then
                        state_next <= S_STOP_BIT;
                    end if;
                when S_STOP_BIT =>
                    if STOP = '1' then
                        state_next <= S_IDLE;
                    end if;
            end case;
        end if;
    end process;
    
    -------- Output combinatorial logic --------

    process(state_reg)
    begin
        START_EN <= '0';
        STOP_EN <= '0';
        CNT_EN <= '0';
        MEM_EN <= '0';
        case state_reg is
            when S_IDLE =>
                null;
            when S_START_BIT =>
                START_EN <= '1';
            when S_DATA =>
                CNT_EN <= '1';
                MEM_EN <= '1';
            when S_STOP_BIT =>
                STOP_EN <= '1';
                MEM_EN <= '1';
        end case;
    end process;

end architecture;
