LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

ENTITY aes_block IS
	PORT(
	    data_in     : IN  std_logic_vector(127 downto 0);
	    data_out    : OUT std_logic_vector(127 downto 0);
	    key	        : IN  std_logic_vector(127 downto 0);
	    clk			: in std_logic;
	    rst_b		: in std_logic
	    );
END aes_block;

ARCHITECTURE behav OF aes_block IS

	component aes_stage IS
		PORT(
		    d_in        :	IN  std_logic_vector(127 downto 0);
		    key	        :	IN  std_logic_vector(127 downto 0);
		    data_out    :	OUT std_logic_vector(127 downto 0)
		    );
	END component aes_stage;
	
	component aes_stage_last IS
		PORT(
		    d_in        :	IN  std_logic_vector(127 downto 0);
		    key	        :	IN  std_logic_vector(127 downto 0);
		    data_out    :	OUT std_logic_vector(127 downto 0)
		    );
	END component aes_stage_last;

	component pipe_reg is
	port (
	      signal clk   : in  std_logic;
	      signal rst_b : in  std_logic;
	      signal rsin  : in  std_logic_vector(127 downto 0);
	      signal rsout : out std_logic_vector(127 downto 0));
	end component pipe_reg;
	
	component key_schedule IS
	PORT(
	    key_in	    :   IN  std_logic_vector(127 downto 0);
	    key_out	    :   OUT std_logic_vector(127 downto 0);
	    round_constant  :	IN  std_logic_vector(7 downto 0)
	    );
	END component key_schedule;
	
type ram_type is array(natural range<>) of std_logic_vector(7 downto 0);
constant rcon: ram_type(0 to 9) := (X"01", X"02", X"04", X"08", X"10", X"20", X"40", X"80", X"1b", X"36");

type data_pipeline_in is array(9 downto 0) of std_logic_vector(127 downto 0);
type data_pipeline_out is array(10 downto 0) of std_logic_vector(127 downto 0);

SIGNAL key_pipe_in   : data_pipeline_in;
SIGNAL key_pipe_out   : data_pipeline_out;
SIGNAL data_pipe_in   : data_pipeline_in;
SIGNAL data_pipe_out   : data_pipeline_out;


BEGIN

data_pipe_out(0) <= data_in;
key_pipe_out(0) <= key;

PIPE_STAGES_I: FOR I IN 0 to 9 GENERATE

AES_10: if I = 9 generate	
	AES10:	aes_stage_last
	    PORT MAP(
		    d_in => data_pipe_out(I),
		    key => key_pipe_out(I),
		    data_out => data_pipe_in(I)
		    );	  
end generate AES_10;
		
AES_I: if I < 9 generate	
	AESI:	aes_stage
	    PORT MAP(
		    d_in => data_pipe_out(I),
		    key => key_pipe_out(I),
		    data_out => data_pipe_in(I)
		    );
end generate AES_I;	  
		    
	KSI: key_schedule
	    PORT MAP(
		    key_in => key_pipe_out(I),
		    key_out => key_pipe_in(I),
		    round_constant => rcon(I)
		    );			    		
		    
	REG_DATA: pipe_reg
	    PORT MAP(
		    rsin => data_pipe_in(I),
		    rsout => data_pipe_out(I+1),
		    clk => clk,
		    rst_b => rst_b
		    );	
		    
	REG_KEY: pipe_reg
	    PORT MAP(
		    rsin => key_pipe_in(I),
		    rsout => key_pipe_out(I+1),
		    clk => clk,
		    rst_b => rst_b
		    );
		    
END GENERATE PIPE_STAGES_I; 

data_out <= data_pipe_out(10) XOR key_pipe_out(10);

END behav;			    
	