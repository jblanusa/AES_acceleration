library ieee;
use ieee.std_logic_1164.all;

entity pipe_reg is
port (
      signal clk   : in  std_logic;
      signal rst_b : in  std_logic;
      signal rsin  : in  std_logic_vector(127 downto 0);
      signal rsout : out std_logic_vector(127 downto 0));
end entity pipe_reg;

architecture bhv of pipe_reg is

   signal reg_int_next, reg_int_reg : std_logic_vector(127 downto 0);

begin

	REG : process (clk, rst_b)
	begin
		if rst_b = '0' then
 			reg_int_reg <= (others => '0');
      	elsif rising_edge(clk) then
        	reg_int_reg <= reg_int_next;
      	end if;
	end process REG;
  
	NSL : reg_int_next <= rsin;
	OL : rsout <= reg_int_reg;

end architecture bhv;