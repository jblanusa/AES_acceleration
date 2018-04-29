library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY aes_custom IS
	PORT(
		signal clk : IN STD_LOGIC; -- CPU's master-input clk <required for multi-cycle>
		signal reset : IN STD_LOGIC; -- CPU's master asynchronous reset <required for multi-cycle>
		signal clk_en: IN STD_LOGIC; -- Clock-qualifier <required for multi-cycle>
		signal start: IN STD_LOGIC; -- True when this instr. issues <required for multi-cycle>
		signal done: OUT STD_LOGIC; -- True when instr. completes <required for variable muli-cycle>
		signal dataa: IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- operand A <always required>
		signal datab: IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- operand B <optional>
		signal result : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) -- result <always required>
	);
END aes_custom;

ARCHITECTURE struct OF aes_custom IS
	component aes_fsmd IS
	PORT(
		    data_in     : IN  std_logic_vector(127 downto 0);
		    data_out    : OUT std_logic_vector(127 downto 0);
		    key	        : IN  std_logic_vector(127 downto 0);
		    start		: in std_logic;
		    ready		: out std_logic;
		    clk			: in std_logic;
		    rst_b		: in std_logic
	    );
	END component aes_fsmd;
		
	type state_type is (ST_IDLE, ST_LOAD, ST_DONE_LOAD, ST_START, ST_WAIT, ST_DONE_OP);
	constant RST_STATE : state_type := ST_IDLE;
	signal state_reg, state_next : state_type;
	
	signal input_data, input_key : std_logic_vector(127 downto 0);
	signal data_out_reg, data_out_next : std_logic_vector(127 downto 0);
	signal cnt_in_next, cnt_in_reg : natural range 0 to 5;
	signal cnt_out_next, cnt_out_reg : natural range 0 to 5;
	
	type data_arr is array(3 downto 0) of std_logic_vector(31 downto 0);	
	signal datas_next, datas_reg, keys_next, keys_reg, datas_out : data_arr;
	
	signal start_aes, ready_aes, rst_b : std_logic;
begin
	rst_b <= not reset;
	
	AES: aes_fsmd
	    PORT MAP(
		    data_in => input_data,
		    data_out => data_out_next,
		    start => start_aes,
		    ready => ready_aes,
		    key => input_key,
		    clk => clk,
		    rst_b => rst_b
		    );
		    
		    
	-- control part : state register
	CP_SR : process(clk, reset) is
	begin
		if reset = '1' then
			state_reg <= RST_STATE;
			cnt_in_reg <= 0;
			cnt_out_reg <= 0;
			datas_reg <= (others => (others => '0'));
			data_out_reg <= (others => '0');
			keys_reg <= (others => (others => '0'));
		elsif rising_edge(clk) then
			if clk_en = '1' then
				state_reg <= state_next;
				cnt_in_reg <= cnt_in_next;
				cnt_out_reg <= cnt_out_next;	
				keys_reg <= keys_next;
				datas_reg <= datas_next;
				data_out_reg <= data_out_next;
			end if;
		end if;
	end process CP_SR;
	
	-- control part : next-state logic
	CP_NSL:  process(state_reg, start, cnt_in_reg, ready_aes) is
	begin
		state_next <= state_reg; -- avoid latches ?
		case state_reg is
			when ST_IDLE => 
				if start = '1' then
					state_next <= ST_LOAD;
				end if;
			when ST_LOAD =>
				if cnt_in_reg >= 3 then
					state_next <= ST_START;
				else
					state_next <= ST_DONE_LOAD;
				end if;
			when ST_DONE_LOAD => 
				if start = '1' then
					state_next <= ST_LOAD;
				end if;
			when ST_START =>
					if (ready_aes = '0') then
						state_next <= ST_WAIT;
					end if;
			when ST_WAIT => 
					if (ready_aes = '1') then
						state_next <= ST_DONE_OP;
					end if;
			when ST_DONE_OP => 
				state_next <= ST_IDLE;
		end case;
	end process CP_NSL;
	
					
	input_data(127 downto 96) <= datas_reg(0);
	input_data(95 downto 64) <= datas_reg(1);
	input_data(63 downto 32) <= datas_reg(2);
	input_data(31 downto 0) <= datas_reg(3);
	input_key(127 downto 96) <= keys_reg(0);
	input_key(95 downto 64) <= keys_reg(1);
	input_key(63 downto 32) <= keys_reg(2);
	input_key(31 downto 0) <= keys_reg(3);
	datas_out(0) <= data_out_next(127 downto 96);
	datas_out(1) <= data_out_next(95 downto 64);
	datas_out(2) <= data_out_next(63 downto 32);
	datas_out(3) <= data_out_next(31 downto 0);
	
	result <= datas_out(cnt_in_reg);
	-- datapath : routing mux
	DP_RMUX : process (state_reg, dataa, keys_reg,datas_reg,  datab,data_out_reg, cnt_in_reg, datas_out, start)
	begin	
		done <= '0';
		start_aes <= '0';
		keys_next <= keys_reg;
		datas_next <= datas_reg;
		cnt_in_next <= 0;
		case state_reg is
				when ST_IDLE => 
					null;	
				when ST_LOAD =>
					datas_next(cnt_in_reg) <= dataa;
					keys_next(cnt_in_reg) <= datab;
					cnt_in_next <= cnt_in_reg;
				when ST_DONE_LOAD => 
					cnt_in_next <= cnt_in_reg + 1;	
					done <= '1';
				when ST_START => 
					start_aes	 <= '1';
				when ST_WAIT =>	
						null;
				when ST_DONE_OP => 
					done <= '1';
		end case;
	end process DP_RMUX;
end architecture struct;