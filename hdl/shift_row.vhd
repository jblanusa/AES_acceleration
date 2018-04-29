library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity shift_row is
	port(stage_input		: in std_logic_vector(127 downto 0);
		 stage_output		: out std_logic_vector(127 downto 0));
end entity shift_row;

architecture behav of shift_row is

	type matrix is array(15 downto 0) of std_logic_vector(7 downto 0);
	
	signal matrix_in, matrix_out : matrix;
begin
	
	TO_MX: for I in 15 downto 0 generate
		matrix_in(15-I) <= stage_input(8*I+7 downto 8*I);	
		stage_output(8*i+7 downto 8*i) <= matrix_out(15-i);
	end generate TO_MX;
	
	-- first column
	matrix_out(0)  <=  matrix_in(0);
	matrix_out(1)  <=  matrix_in(5);
	matrix_out(2)  <=  matrix_in(10);
	matrix_out(3)  <=  matrix_in(15);
	-- second column
	matrix_out(4)  <=  matrix_in(4);
	matrix_out(5)  <=  matrix_in(9);
	matrix_out(6)  <=  matrix_in(14);
	matrix_out(7)  <=  matrix_in(3);
	-- third column
	matrix_out(8)  <=  matrix_in(8);
	matrix_out(9)  <=  matrix_in(13);
	matrix_out(10) <=  matrix_in(2);
	matrix_out(11) <=  matrix_in(7);
	-- forth column
	matrix_out(12) <=  matrix_in(12);
	matrix_out(13) <=  matrix_in(1);
	matrix_out(14) <=  matrix_in(6);
	matrix_out(15) <=  matrix_in(11);
	
end architecture behav;