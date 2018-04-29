-- FULL RGB LEDs 12x8 Avalon Interface
-- author: xavier.jimenez@epfl.ch
-- file: rgb_led96.vhd
-- An avalon slave interface to control 96 rgb leds.
-- full version: every LED can have its own color
-- carefull, the LED are addressed counting horizontaly
-- starting from the top left
LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

package pack is
	function f_ceil_log2 (constant n : natural)
		return natural;
		
	function f_min (constant c1 : natural; constant c2 : natural)
		return natural;
end pack;
 
package body pack is
	-- computes ceil(log2(N))
	function f_ceil_log2 (constant n : natural) return natural is
		variable n_v : natural;
		variable ret : natural;
	begin
		assert n > 0
		report "pack.f_ceil_log2(n): input n should be greater than 0."
		severity error;
		  
		ret := 0;
		n_v := 2*n-1;
		while n_v > 1 loop
			ret := ret + 1;
			n_v := n_v / 2;
		end loop;

		return ret; 
	end f_ceil_log2;
	
	function f_min (constant c1 : natural; constant c2 : natural)
		return natural is
		variable ret : natural;
	begin
		if (c1 < c2) then
			ret := c1;
		else
			ret:= c2;
		end if;
		return ret;
	end f_min;
end pack;


LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

LIBRARY altera_mf;
USE altera_mf.all;

USE work.pack.all;

entity if_fullrgb_led96 is
	generic(
		-- input clock frequency in Hz
		CLK_FREQ   : natural := 50000000;
		-- desired minimum frame rate in Hz
		FRAME_RATE : natural := 100
	);
	port(
		-- Global
		clk   : in std_logic;
		reset : in std_logic;
		
		-- Avalon slave
		as0_read        : in  std_logic;
		as0_write       : in  std_logic;
		as0_address     : in  std_logic_vector(6 downto 0);
		as0_rddata      : out std_logic_vector(31 downto 0);
		as0_wrdata      : in  std_logic_vector(31 downto 0);
		as0_waitrequest : out std_logic;
		
		-- RGB LEDS matrix
		LED_SelC_n : out std_logic_vector(11 downto 0);
		LED_Sel_R  : out std_logic_vector(7 downto 0);
		LED_Sel_G  : out std_logic_vector(7 downto 0);
		LED_Sel_B  : out std_logic_vector(7 downto 0);
		LED_reset  : out std_logic
	);
	
end entity;

--	Architecture Body
architecture arch of if_fullrgb_led96 is
constant C_ROWS : natural := 8;
constant C_COLUMNS : natural := 12;
constant C_PWM_BITS : natural := 15;
-- time spent reseting a line (fixed to 1us), minimum should be 25 cycles
constant C_PERIOD_RST  : natural := f_min(25, CLK_FREQ/1000000);
-- maximum time period per line to satisfy frame rate
constant C_MAX_PERIOD : natural := CLK_FREQ/FRAME_RATE/C_COLUMNS - 1;
-- pwm cycles. We fix the pwm on 2**15 clock cycles and want a display phase aligned to it
-- total refresh line being 2 transitional cyce a reset phase and the display phase we get
constant C_PWM_CYCLES : natural := (C_MAX_PERIOD-C_PERIOD_RST-2)/(2**C_PWM_BITS);
-- display period
constant C_DISPLAY_PERIOD : natural := (2**C_PWM_BITS)*C_PWM_CYCLES;
-- we get the period counter bitwidth 
constant C_PERIOD_WIDTH : natural := f_ceil_log2(C_DISPLAY_PERIOD); 

-- the current led color being read from memory
signal reg_led_addr : std_logic_vector(6 downto 0);
signal s_sram_addr  : std_logic_vector(6 downto 0);
-- a register to store every LED 24-bit color on a column (16 bits per channel and per row)
signal reg_col : std_logic_vector(3*8*16-1 downto 0);
-- period counter
signal reg_period_cnt : std_logic_vector(C_PERIOD_WIDTH-1 downto 0);
-- pwm global counter
signal reg_pwm_cnt : std_logic_vector(9 downto 0);
-- current col index
signal reg_col_cnt : std_logic_vector(3 downto 0);
-- current col vector (single bit active on a 24-bit vector)
signal reg_sel_col : std_logic_vector(11 downto 0);
-- read bus from SRAM
signal s_ram_rddata : std_logic_vector(31 downto 0);
-- register for waitrequest
signal reg_waitrequest : std_logic;

-- gamma 2.2 array to translate RGB to pwm duty cycle (for a period of 2**15 cycles)
type t_array16 is array (0 to 255) of std_logic_vector(15 downto 0);
signal gamma_table : t_array16 :=
	 (X"0000", X"0001", X"0002", X"0003", X"0004", X"0005", X"0006", X"0007", X"0008", X"0009", X"000A", X"000B", X"000C", X"000D", X"000E", X"000F", X"0010"
	, X"0011", X"0012", X"0013", X"0014", X"0015", X"0016", X"0017", X"0018", X"0019", X"001B", X"001D", X"0020", X"0022", X"0025", X"0028", X"002B"
	, X"002E", X"0031", X"0034", X"0037", X"003B", X"003E", X"0042", X"0046", X"0049", X"004D", X"0052", X"0056", X"005A", X"005F", X"0063", X"0068"
	, X"006D", X"0072", X"0077", X"007C", X"0081", X"0087", X"008C", X"0092", X"0098", X"009E", X"00A4", X"00AA", X"00B0", X"00B6", X"00BD", X"00C4"
	, X"00CA", X"00D1", X"00D8", X"00E0", X"00E7", X"00EE", X"00F6", X"00FE", X"0105", X"010D", X"0115", X"011E", X"0126", X"012E", X"0137", X"0140"
	, X"0149", X"0152", X"015B", X"0164", X"016D", X"0177", X"0181", X"018A", X"0194", X"019E", X"01A8", X"01B3", X"01BD", X"01C8", X"01D3", X"01DD"
	, X"01E9", X"01F4", X"01FF", X"020A", X"0216", X"0222", X"022D", X"0239", X"0246", X"0252", X"025E", X"026B", X"0277", X"0284", X"0291", X"029E"
	, X"02AC", X"02B9", X"02C6", X"02D4", X"02E2", X"02F0", X"02FE", X"030C", X"031B", X"0329", X"0338", X"0346", X"0355", X"0365", X"0374", X"0383"
	, X"0393", X"03A2", X"03B2", X"03C2", X"03D2", X"03E2", X"03F3", X"0403", X"0414", X"0425", X"0436", X"0447", X"0458", X"046A", X"047B", X"048D"
	, X"049F", X"04B1", X"04C3", X"04D6", X"04E8", X"04FB", X"050D", X"0520", X"0533", X"0547", X"055A", X"056D", X"0581", X"0595", X"05A9", X"05BD"
	, X"05D1", X"05E6", X"05FA", X"060F", X"0624", X"0639", X"064E", X"0664", X"0679", X"068F", X"06A4", X"06BA", X"06D1", X"06E7", X"06FD", X"0714"
	, X"072A", X"0741", X"0758", X"0770", X"0787", X"079E", X"07B6", X"07CE", X"07E6", X"07FE", X"0816", X"082F", X"0847", X"0860", X"0879", X"0892"
	, X"08AB", X"08C5", X"08DE", X"08F8", X"0912", X"092C", X"0946", X"0960", X"097B", X"0995", X"09B0", X"09CB", X"09E6", X"0A01", X"0A1D", X"0A38"
	, X"0A54", X"0A70", X"0A8C", X"0AA8", X"0AC5", X"0AE1", X"0AFE", X"0B1B", X"0B38", X"0B55", X"0B73", X"0B90", X"0BAE", X"0BCC", X"0BEA", X"0C08"
	, X"0C26", X"0C45", X"0C63", X"0C82", X"0CA1", X"0CC0", X"0CDF", X"0CFF", X"0D1F", X"0D3E", X"0D5E", X"0D7E", X"0D9F", X"0DBF", X"0DE0", X"0E01"
	, X"0E22", X"0E43", X"0E64", X"0E85", X"0EA7", X"0EC9", X"0EEB", X"0F0D", X"0F2F", X"0F51", X"0F74", X"0F97", X"0FBA", X"0FDD", X"1000");
signal reg_gamma_addr : std_logic_vector(7 downto 0);
signal reg_gamma_data : std_logic_vector(15 downto 0);
-- which gamma are we reading (0->2)
signal reg_gamma_cnt  : std_logic_vector(1 downto 0);
-- FSM
type t_state is (TRANS1, RST, TRANS2, DISPLAY);
signal state, next_state: t_state;

component altsyncram
generic (
	address_reg_b		: STRING;
	clock_enable_input_a		: STRING;
	clock_enable_input_b		: STRING;
	clock_enable_output_a		: STRING;
	clock_enable_output_b		: STRING;
	intended_device_family		: STRING;
	lpm_type		: STRING;
	numwords_a		: NATURAL;
	numwords_b		: NATURAL;
	operation_mode		: STRING;
	outdata_aclr_b		: STRING;
	outdata_reg_b		: STRING;
	power_up_uninitialized		: STRING;
	read_during_write_mode_mixed_ports		: STRING;
	widthad_a		: NATURAL;
	widthad_b		: NATURAL;
	width_a		: NATURAL;
	width_b		: NATURAL;
	width_byteena_a		: NATURAL
);
port (
		address_a	: IN STD_LOGIC_VECTOR (6 DOWNTO 0);
		clock0	: IN STD_LOGIC ;
		data_a	: IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		q_b	: OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
		wren_a	: IN STD_LOGIC ;
		address_b	: IN STD_LOGIC_VECTOR (6 DOWNTO 0)
);
end component;

begin
	-- col activation
	LED_SelC_n <= not reg_sel_col when state=DISPLAY else (others => '1');
	
	-- row activation
	g_row : for i in 0 to 7 generate
		process (reg_col, reg_pwm_cnt, state)
			variable v_pwm : std_logic_vector(C_PWM_BITS-1 downto 0);
		begin
			-- by default, each channel at 0
			LED_Sel_R(i) <= '0';
			LED_Sel_G(i) <= '0';
			LED_Sel_B(i) <= '0';
			v_pwm := reg_period_cnt(C_PWM_BITS-1 downto 0);
			if (state=DISPLAY) then
				-- red
				if (reg_col((i*3+2+1)*16-1 downto (i*3+2)*16) > v_pwm) then
					LED_Sel_R(i) <= '1';
				end if;
				-- green
				if (reg_col((i*3+1+1)*16-1 downto (i*3+1)*16) > v_pwm) then
					LED_Sel_G(i) <= '1';
				end if;
				-- blue
				if (reg_col((i*3+1)*16-1 downto (i*3)*16) > v_pwm) then
					LED_Sel_B(i) <= '1';
				end if;
			end if;
		end process;
	end generate;
	
	reg_gamma_data <= gamma_table(conv_integer(reg_gamma_addr));
	-- loading each row colors
	process(clk) -- no need for reset
	begin
		if (rising_edge(clk)) then
			-- the address to the gamma table
			case reg_gamma_cnt is
				when "00" => reg_gamma_addr <= s_ram_rddata(7 downto 0);
				when "01" => reg_gamma_addr <= s_ram_rddata(15 downto 8);
				when "10" => reg_gamma_addr <= s_ram_rddata(23 downto 16);
				when others =>
			end case;
			-- the reg column
			if (state=RST) then
				if (reg_period_cnt <= 25) then
					-- for green and blue the value must be doubled
					case reg_gamma_cnt is
						when "01" | "10" => 
							reg_col <= reg_gamma_data(14 downto 0) & '0' & reg_col(3*8*16-1 downto 16);
						when others => 
							reg_col <= reg_gamma_data & reg_col(3*8*16-1 downto 16);
					end case;
				end if;
			end if;
		end if;
	end process;
	
	-- registers and counters
	process (reset, clk)
	begin
		if (reset = '1') then
			state          <= TRANS1;
			reg_period_cnt <= (others => '0');
			reg_col_cnt    <= "0000";
			reg_sel_col    <= (11 => '1', others => '0');
			reg_led_addr   <= (others => '0');
			reg_gamma_cnt  <= (others => '0');
		elsif rising_edge(clk) then
			state <= next_state;
			-- period counter
			case state is 
				when TRANS1 | TRANS2 =>
					reg_period_cnt <= (others => '0');
				when RST | DISPLAY =>
					reg_period_cnt <= reg_period_cnt + 1;
				when others =>
			end case;
			-- new column
			if (state=TRANS1) then
				-- we activate the next column
				reg_sel_col <= reg_sel_col(10 downto 0) & reg_sel_col(11);
			end if;
			
			-- gamma counter
			if (state=RST) then
				if (reg_gamma_cnt = 2) then
					reg_gamma_cnt <= "00";
				else
					reg_gamma_cnt <= reg_gamma_cnt+1;
				end if;
			else
				reg_gamma_cnt <= "00";
			end if;
			
			-- column index
			if (state=TRANS2) then
				-- we update the column index after a reset
				if (reg_col_cnt = 11) then
					reg_col_cnt <= (others => '0');
				else
					reg_col_cnt <= reg_col_cnt + 1;
				end if;
			end if;
			
			-- read address to sram
			if (reg_period_cnt<24) then
				if (reg_gamma_cnt=1) then
					-- increment 7 times to load a full column
					-- only when we are done reading the gamma table
					reg_led_addr <= reg_led_addr + 12;
				end if;
			else
				-- reset to the column index
				reg_led_addr <= "000" & reg_col_cnt;
			end if;
		end if;
	end process;
	
	-- Simple state machine to switch between reset and display periods.
	-- Between each transition we deactivate every signal for 1 cycle to
	-- ensure that the RGB_reset signal cannot be activated along with a
	-- column selection (shortcut)
	process(state, reg_period_cnt)
	begin
		LED_reset  <= '0';
		next_state <= state;
		case state is
			when TRANS1 =>
				next_state <= RST;
	
			when RST =>
				LED_reset <= '1';
				if (reg_period_cnt = C_PERIOD_RST-1) then
					next_state <= TRANS2;
				end if;
	
			when TRANS2 =>
				next_state <= DISPLAY;
	
			when DISPLAY => 
				if (reg_period_cnt = C_DISPLAY_PERIOD-1) then
					next_state <= TRANS1;
				end if;
			when others =>
		end case;
	end process;
	
	-- Avalon interface
	as0_rddata <= s_ram_rddata;
	process(reg_period_cnt, reg_led_addr, as0_address)
	begin
		if (reg_period_cnt<25) then
			-- from cycles 0 to 24 the sram is reserved for internal read
			s_sram_addr <= reg_led_addr;
		else
			-- otherwise it can be accessed from the avalon bus
			s_sram_addr <= as0_address;
		end if;
	end process;
	
	process(reset, clk)
	begin
		if (reset='1') then
			reg_waitrequest <= '1';
		elsif (rising_edge(clk)) then
			-- sram cannot be accessed from avalon during
			-- cycles 0 to 24.
			if (as0_read = '1' and reg_period_cnt >= 25) then
				reg_waitrequest <= '0';
			else
				reg_waitrequest <= '1';
			end if;
		end if;
	end process;
	
	process(as0_write, reg_waitrequest)
	begin
		if (as0_write='1') then
			as0_waitrequest <= '0';
		else
			as0_waitrequest <= reg_waitrequest;
		end if;
	end process;
	
	-- avalon interface
	-- we do not read for now. It requires simple arbitration. Maybe later.
	-- as0_rddata <= (others => '0');
	-- writes are directly mapped to the sram:
	-- SRAM instance
	altsyncram_component : altsyncram
	GENERIC MAP (
		address_reg_b => "CLOCK0",
		clock_enable_input_a => "BYPASS",
		clock_enable_input_b => "BYPASS",
		clock_enable_output_a => "BYPASS",
		clock_enable_output_b => "BYPASS",
		intended_device_family => "Cyclone IV E",
		lpm_type => "altsyncram",
		numwords_a => 128,
		numwords_b => 128,
		operation_mode => "DUAL_PORT",
		outdata_aclr_b => "NONE",
		outdata_reg_b => "UNREGISTERED",
		power_up_uninitialized => "FALSE",
		read_during_write_mode_mixed_ports => "DONT_CARE",
		widthad_a => 7,
		widthad_b => 7,
		width_a => 32,
		width_b => 32,
		width_byteena_a => 1
	)
	PORT MAP (
		address_a => as0_address,
		clock0 => clk,
		data_a => as0_wrdata,
		wren_a => as0_write,
		address_b => s_sram_addr,
		q_b => s_ram_rddata
	);
end architecture;