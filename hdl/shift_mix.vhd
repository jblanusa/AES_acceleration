library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity shift_mix is
	port(stage_input		: in std_logic_vector(127 downto 0);
		 stage_output		: out std_logic_vector(127 downto 0));
end entity shift_mix;

architecture behav of shift_mix is

	type matrix is array(15 downto 0) of std_logic_vector(7 downto 0);
	
	signal matrix_in, matrix_out, matrix2_in, matrix3_in : matrix;
begin
	
	TO_MX: for I in 15 downto 0 generate
		matrix_in(15-I) <= stage_input(8*I+7 downto 8*I);
	
		matrix2_in(15-I) <= (matrix_in(15-I)(6 downto 0) & '0') xor "00011011" when matrix_in(15-I)(7) = '1' else
							(matrix_in(15-I)(6 downto 0) & '0');
							
		matrix3_in(15-I) <= (matrix_in(15-I)(6 downto 0) & '0') xor matrix_in(15-I) xor "00011011" when matrix_in(15-I)(7) = '1' else
							(matrix_in(15-I)(6 downto 0) & '0') xor matrix_in(15-I);
		
		stage_output(8*i+7 downto 8*i) <= matrix_out(15-i);
	end generate TO_MX;
	
	-- 4X4 matrix multiplication, mix column and row shift
	--row one
	matrix_out(0)  <= matrix2_in(0)  XOR matrix3_in(5)  XOR matrix_in(10)  XOR matrix_in(15);
	matrix_out(4)  <= matrix2_in(4)  XOR matrix3_in(9)  XOR matrix_in(14)  XOR matrix_in(3);
	matrix_out(8)  <= matrix2_in(8)  XOR matrix3_in(13)  XOR matrix_in(2) XOR matrix_in(7);
	matrix_out(12) <= matrix2_in(12) XOR matrix3_in(1) XOR matrix_in(6) XOR matrix_in(11);
	--row two
	matrix_out(1)  <= matrix_in(0)  XOR matrix2_in(5)  XOR matrix3_in(10)  XOR matrix_in(15); 
	matrix_out(5)  <= matrix_in(4)  XOR matrix2_in(9)  XOR matrix3_in(14)  XOR matrix_in(3); 
	matrix_out(9)  <= matrix_in(8)  XOR matrix2_in(13)  XOR matrix3_in(2) XOR matrix_in(7); 
	matrix_out(13) <= matrix_in(12) XOR matrix2_in(1) XOR matrix3_in(6) XOR matrix_in(11); 
	--row three
	matrix_out(2)  <= matrix_in(0)  XOR matrix_in(5)  XOR matrix2_in(10)  XOR matrix3_in(15);
	matrix_out(6)  <= matrix_in(4)  XOR matrix_in(9)  XOR matrix2_in(14)  XOR matrix3_in(3);
	matrix_out(10) <= matrix_in(8)  XOR matrix_in(13)  XOR matrix2_in(2) XOR matrix3_in(7);
	matrix_out(14) <= matrix_in(12) XOR matrix_in(1) XOR matrix2_in(6) XOR matrix3_in(11);
	--row four
	matrix_out(3)  <= matrix3_in(0)  XOR matrix_in(5)  XOR matrix_in(10)  XOR matrix2_in(15);
	matrix_out(7)  <= matrix3_in(4)  XOR matrix_in(9)  XOR matrix_in(14)  XOR matrix2_in(3);
	matrix_out(11) <= matrix3_in(8)  XOR matrix_in(13)  XOR matrix_in(2) XOR matrix2_in(7);
	matrix_out(15) <= matrix3_in(12) XOR matrix_in(1) XOR matrix_in(6) XOR matrix2_in(11);	
	
end architecture behav;
