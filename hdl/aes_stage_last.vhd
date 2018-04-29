LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;

ENTITY aes_stage_last IS
PORT(
    d_in        :	IN  std_logic_vector(127 downto 0);
    key	        :	IN  std_logic_vector(127 downto 0);
    data_out    :	OUT std_logic_vector(127 downto 0)
    );
END aes_stage_last;

ARCHITECTURE behav OF aes_stage_last IS

component sbox is
  port (
    --! @brief Input to the S-box.
    In_DI  : in  std_logic_vector(7 downto 0);
    --! @brief Substituted output of the S-box.
    Out_DO : out std_logic_vector(7 downto 0));
end component sbox;

COMPONENT shift_row 
PORT(
    stage_input		: in std_logic_vector(127 downto 0);
	stage_output		: out std_logic_vector(127 downto 0)
    );
END COMPONENT;

SIGNAL bytesub   : std_logic_vector(127 downto 0);
SIGNAL sbox_in : std_logic_vector(127 downto 0);
SIGNAL mux       : std_logic_vector(127 downto 0);


BEGIN

sbox_in <= d_in XOR key;

sbox_16: FOR i IN 15 DOWNTO 0 GENERATE
    sbox_map:	sbox
    PORT MAP(
	    In_DI => sbox_in(8*i+7 downto 8*i),
	    Out_DO => bytesub(8*i+7 downto 8*i)
	    );
END GENERATE sbox_16;
	    
shiftrow_s:  shift_row
PORT MAP(
	stage_input => bytesub,
	stage_output => data_out
	);


END behav;			    
	