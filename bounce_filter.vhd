-- bounce_filter
-- filters bounces comming from push buttons.
-- We can set the number of input to filter.
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity bounce_filter is
	generic(
		N : natural := 1);
	port(
		clk   : in  std_logic;
		-- input
		input  : in  std_logic_vector(N-1 downto 0);
		output : out std_logic_vector(N-1 downto 0)
		);
end entity;

architecture synth of bounce_filter is
-- assuming the default 24MHz, we want at least 10 ms
-- 10'000'000 ns / 50ns = 200'000 cycles => 2^18 is fine 
-- 
type  t_array   is array (natural range <>) of std_logic_vector(17 downto 0);

signal r_counters : t_array(0 to N-1);
signal r_input    : std_logic_vector(N-1 downto 0);
begin

-- registers
process(clk)
begin
	if (rising_edge(clk)) then
		r_input <= input;
	end if;
end process;

-- counters
g_counters: for i in 0 to N-1 generate
	process(clk)
	begin
		if (rising_edge(clk)) then
			if (r_input(i) /= input(i)) then
				r_counters(i) <= (others => '1');
			else
				if (r_counters(i) /= 0) then
					r_counters(i) <= r_counters(i) - 1;
				else
					output(i) <= r_input(i);
				end if;
			end if;
		end if;
	end process;
end generate;

end architecture;