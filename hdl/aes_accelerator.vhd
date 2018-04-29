library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY aes_accelerator IS
	PORT(
		clk: in std_logic;
		rst_n: in std_logic;
		-- Slave part
		as_address: in std_logic_vector(2 downto 0);
		as_chipSelect: in std_logic;
		as_write: in std_logic;
		as_read: in std_logic;
		as_writeData: in std_logic_vector(31 downto 0);
		as_readData: out std_logic_vector(31 downto 0);
		-- Master part
		am_address: out std_logic_vector(31 downto 0);
		am_byteEnable: out std_logic_vector(3 downto 0);
		am_write: out std_logic;
		am_read: out std_logic;
		am_writeData: out std_logic_vector(31 downto 0);
		am_readData: in std_logic_vector(31 downto 0);
		am_waitRequest: in std_logic

	);
END aes_accelerator;

ARCHITECTURE struct OF aes_accelerator IS
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
	
	constant ADDR_INC   : integer := 4;
	
	type states is (ST_IDLE, ST_READ, ST_READ_END, ST_START, ST_WAIT, ST_WRITE, ST_WRITE_END);
	
	signal done : std_logic;
	signal start : std_logic;
	signal inc_addr : std_logic;
	-- registri
	signal state_reg, state_next : states;
	signal start_address_reg, start_address_next : unsigned(31 downto 0);
	signal dest_address_reg, dest_address_next : unsigned(31 downto 0);
	signal current_address_reg, current_address_next : unsigned(31 downto 0);
	signal current_dest_address_reg, current_dest_address_next : unsigned(31 downto 0);
	signal data_length_reg, data_length_next: unsigned(15 downto 0);
	signal data_counter_reg, data_counter_next: unsigned(15 downto 0);
	signal result_reg, result_next : std_logic_vector(31 downto 0);
	
	type data_arr is array(3 downto 0) of std_logic_vector(31 downto 0);	
	signal datas_next, datas_reg, keys_next, keys_reg, datas_out : data_arr;
	
	signal input_data, input_key : std_logic_vector(127 downto 0);
	signal data_out	: std_logic_vector(127 downto 0);
	signal start_aes, ready_aes: std_logic;
	
	signal counter_next, counter_reg : natural range 0 to 4;
BEGIN
	
	-- Registers
	--
	REG: process (clk, rst_n)
	begin
		if rst_n = '0' then
			state_reg <= ST_IDLE;
			start_address_reg <= (others => '0');
			dest_address_reg <= (others => '0');
			data_length_reg <= (others => '0');
			current_address_reg <= (others => '0');
			current_dest_address_reg <= (others => '0');
			data_counter_reg <= (others => '0');
			result_reg <= (others => '0');
			datas_reg <= (others => (others => '0'));
			keys_reg <= (others => (others => '0'));
			counter_reg <= 0;
		elsif rising_edge(clk) then
			state_reg <= state_next;
			start_address_reg <= start_address_next;
			dest_address_reg <= dest_address_next;
			data_length_reg <= data_length_next;
			current_address_reg <= current_address_next;
			current_dest_address_reg <= current_dest_address_next;
			data_counter_reg <= data_counter_next;
			result_reg <= result_next;
			keys_reg <= keys_next;
			datas_reg <= datas_next;
			counter_reg <= counter_next;
		end if;
	end process REG;
	
	-- Slave write
	--
	SlaveWrite: process(as_chipSelect, as_write, as_writeData, as_address, start_address_reg, data_length_reg, start)
	begin
		start_address_next <= start_address_reg; -- avoid latches
		dest_address_next <= dest_address_reg;
		data_length_next  <= data_length_reg;
		keys_next <= keys_reg;
		start <= '0';
		if as_chipSelect = '1' and as_write = '1' then
			case as_address is
				when "000" => start_address_next <= unsigned(as_writeData);
				when "001" => dest_address_next <= unsigned(as_writeData);
				when "010" => data_length_next <= unsigned(as_writeData(15 downto 0));
				when "011" => start <= as_writeData(0);
					inc_addr <= as_writeData(1);
				when "100" => keys_next(0) <= as_writeData(7 downto 0) & as_writeData(15 downto 8) & as_writeData(23 downto 16) & as_writeData(31 downto 24);
				when "101" => keys_next(1) <= as_writeData(7 downto 0) & as_writeData(15 downto 8) & as_writeData(23 downto 16) & as_writeData(31 downto 24);
				when "110" => keys_next(2) <= as_writeData(7 downto 0) & as_writeData(15 downto 8) & as_writeData(23 downto 16) & as_writeData(31 downto 24);
				when "111" => keys_next(3) <= as_writeData(7 downto 0) & as_writeData(15 downto 8) & as_writeData(23 downto 16) & as_writeData(31 downto 24);
				when others => null;
			end case;
		end if;
	end process SlaveWrite;
	
	-- Slave Read
	--
	SlaveRead:	process(as_chipSelect, as_read, as_address)
	begin
		as_readData <= (others => '0'); 
		if as_chipSelect = '1' and as_read = '1' then
			case as_address is
				when "000" => as_readData <= std_logic_vector(start_address_reg);
				when "001" => as_readData(15 downto 0) <= std_logic_vector(data_length_reg);
				when "010" => as_readData(0) <= start;
				when "011" => as_readData(0) <= done;
				when others => null;
			end case;
		end if;
	end process SlaveRead;
	
	-- Next State Logic
	--
	NSL: process (state_reg, start, am_waitRequest, ready_aes, counter_reg)
	begin
		state_next <= state_reg; --avoid latches
		case state_reg is
			when ST_IDLE => 
				if start = '1' then
					state_next <= ST_READ;
				end if;
			when ST_READ => 
				if am_waitRequest = '0' then
					state_next <= ST_READ_END;
				else
					state_next <= ST_READ;
				end if;
			when ST_READ_END => 
				if counter_reg = 3 then				
					state_next <= ST_START;
				else
					state_next <= ST_READ;	
				end if;
			when ST_START  => 
				if (ready_aes = '0') then
					state_next <= ST_WAIT;
				end if;
			when ST_WAIT  => 
				if (ready_aes = '1') then
					state_next <= ST_WRITE;
				end if;
			when ST_WRITE =>
				if am_waitRequest = '0' then
					state_next <= ST_WRITE_END;
				else
					state_next <= ST_WRITE;
				end if;
			when ST_WRITE_END =>
				if counter_reg = 3 then				
					if data_counter_reg = 1 then
						state_next <= ST_IDLE;
					else 
						state_next <= ST_READ;
					end if;
				else
					state_next <= ST_WRITE;	
				end if;
		end case;
	end process NSL;
	
	AES: aes_fsmd
	    PORT MAP(
		    data_in => input_data,
		    data_out => data_out,
		    start => start_aes,
		    ready => ready_aes,
		    key => input_key,
		    clk => clk,
		    rst_b => rst_n
		    );
		    
	input_data(127 downto 96) <= datas_reg(0);
	input_data(95 downto 64) <= datas_reg(1);
	input_data(63 downto 32) <= datas_reg(2);
	input_data(31 downto 0) <= datas_reg(3);
	input_key(127 downto 96) <= keys_reg(0);
	input_key(95 downto 64) <= keys_reg(1);
	input_key(63 downto 32) <= keys_reg(2);
	input_key(31 downto 0) <= keys_reg(3);
	datas_out(0) <= data_out(127 downto 96);
	datas_out(1) <= data_out(95 downto 64);
	datas_out(2) <= data_out(63 downto 32);
	datas_out(3) <= data_out(31 downto 0);
	
   	-- Datapath
	--
	DP: process (state_reg, am_waitRequest,am_readData, start, data_out, counter_reg, current_dest_address_reg, current_address_reg, datas_out,start_address_reg,data_length_reg, data_counter_reg)	
	begin
		am_read <= '0';
		am_write <= '0';
		done <= '0';
		start_aes <= '0';
		counter_next  <= counter_reg;
		am_address <= (others => '0');
		am_writeData <= (others => '0');
		result_next <= result_reg;
		datas_next <= datas_reg;
		current_address_next <= current_address_reg;
		current_dest_address_next <= current_dest_address_reg;
		data_counter_next <= data_counter_reg;
		case state_reg is
			when ST_IDLE => 
				done <= '1';
				if start = '1' then
					current_address_next <= start_address_reg;
					current_dest_address_next <= dest_address_reg;
					data_counter_next <= data_length_reg;
				end if;
				counter_next <= 0;
			when ST_READ => 
				done <= '0';
				
				am_read <= '1';
				am_address <= std_logic_vector(current_address_reg + ADDR_INC*counter_reg);
			when ST_READ_END => 
				datas_next(counter_reg) <= am_readData(7 downto 0) & am_readData(15 downto 8) & am_readData(23 downto 16) & am_readData(31 downto 24);
				if counter_reg = 3 then
					counter_next <= 0;
				else
					counter_next <= counter_reg + 1;
				end if;
			when ST_START => 
					start_aes <= '1';
					counter_next <= 0;
			when ST_WAIT => 
					null;
			when ST_WRITE =>
				done <= '0';
				
				am_write <= '1';
				am_address <= std_logic_vector(current_dest_address_reg + ADDR_INC*counter_reg);
				am_writeData <= datas_out(counter_reg)(7 downto 0) & datas_out(counter_reg)(15 downto 8) & datas_out(counter_reg)(23 downto 16) & datas_out(counter_reg)(31 downto 24);

			when ST_WRITE_END => 
					if inc_addr = '1' then 
						current_address_next <= current_address_reg + 4*ADDR_INC;
						current_dest_address_next <= current_dest_address_reg + 4*ADDR_INC;
					end if;
					if counter_reg = 3 then
						counter_next <= 0;
						data_counter_next <= data_counter_reg - 1;
					else
						counter_next <= counter_reg + 1;
					end if;
		end case;
	end process DP;
	
	am_byteEnable <= "1111";
   
END struct;