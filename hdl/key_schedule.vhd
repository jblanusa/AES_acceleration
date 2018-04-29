LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;

ENTITY key_schedule IS
PORT(
    key_in	    :   IN  std_logic_vector(127 downto 0);
    key_out	    :   OUT std_logic_vector(127 downto 0);
    round_constant  :	IN  std_logic_vector(7 downto 0)
    );
END key_schedule;

ARCHITECTURE beh OF key_schedule IS

component sbox is
  port (
    --! @brief Input to the S-box.
    In_DI  : in  std_logic_vector(7 downto 0);
    --! @brief Substituted output of the S-box.
    Out_DO : out std_logic_vector(7 downto 0));
end component sbox;
    
SIGNAL U          : std_logic_vector(31 downto 0);
SIGNAL left_shift : std_logic_vector(31 downto 0);
SIGNAL sbox_out   : std_logic_vector(31 downto 0);

SIGNAL key_reg_in  : std_logic_vector(127 downto 0);
SIGNAL next_key    : std_logic_vector(127 downto 0);
SIGNAL key_reg_out : std_logic_vector(127 downto 0);
SIGNAL upperbyte   : std_logic_vector(7 downto 0);

TYPE word_array is ARRAY (3 downto 0) OF std_logic_vector(31 downto 0); 
SIGNAL key_word, next_key_word : word_array;

BEGIN
	
	--mapping a vector into array of words
	
	key_word(0) <= key_in(127 downto 96);
	key_word(1) <= key_in(95 downto 64);
	key_word(2) <= key_in(63 downto 32);
	key_word(3) <= key_in(31 downto 0);
	
	--calculating the next key words
	
	 next_key_word(3) <= key_word(3) XOR next_key_word(2);
	 next_key_word(2) <= key_word(2) XOR next_key_word(1);
	 next_key_word(1) <= key_word(1) XOR next_key_word(0);
	 next_key_word(0) <= key_word(0) XOR U;
	
	
	 key_out <= next_key_word(0) & next_key_word(1) & next_key_word(2) & next_key_word(3);
	
	 -- calculation of U
	
	 left_shift <= (key_word(3)(23 downto 16) &
	               key_word(3)(15 downto 8) &
	               key_word(3)(7 downto 0) &
	               key_word(3)(31 downto 24));
	
	 --key subbyte transformation
	 SBOX_I: for I in 0 to 3 generate
	 sbox_q: sbox
		 PORT MAP(
		        In_DI   => left_shift((I+1)*8-1 downto I*8),
		        Out_DO  => sbox_out((I+1)*8-1 downto I*8)
		        );
	  end generate SBOX_I;
	 
	 --XOR the upperbyte and round constant
	
	 upperbyte <= sbox_out(31 downto 24) XOR round_constant;
	
	
	 U <= upperbyte & sbox_out(23 downto 0);


END beh;