	component nios_system is
		port (
			clk_clk                           : in    std_logic                     := 'X';             -- clk
			key_export                        : out   std_logic_vector(7 downto 0);                     -- export
			leds_new_signal                   : out   std_logic_vector(25 downto 0);                    -- new_signal
			new_sdram_controller_0_wire_addr  : out   std_logic_vector(11 downto 0);                    -- addr
			new_sdram_controller_0_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
			new_sdram_controller_0_wire_cas_n : out   std_logic;                                        -- cas_n
			new_sdram_controller_0_wire_cke   : out   std_logic;                                        -- cke
			new_sdram_controller_0_wire_cs_n  : out   std_logic;                                        -- cs_n
			new_sdram_controller_0_wire_dq    : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			new_sdram_controller_0_wire_dqm   : out   std_logic_vector(3 downto 0);                     -- dqm
			new_sdram_controller_0_wire_ras_n : out   std_logic;                                        -- ras_n
			new_sdram_controller_0_wire_we_n  : out   std_logic;                                        -- we_n
			pwm_out_new_signal                : out   std_logic_vector(1 downto 0);                     -- new_signal
			reset_reset_n                     : in    std_logic                     := 'X';             -- reset_n
			sdram_clk_clk                     : out   std_logic;                                        -- clk
			encoder_in_new_signal             : in    std_logic_vector(1 downto 0)  := (others => 'X')  -- new_signal
		);
	end component nios_system;

	u0 : component nios_system
		port map (
			clk_clk                           => CONNECTED_TO_clk_clk,                           --                         clk.clk
			key_export                        => CONNECTED_TO_key_export,                        --                         key.export
			leds_new_signal                   => CONNECTED_TO_leds_new_signal,                   --                        leds.new_signal
			new_sdram_controller_0_wire_addr  => CONNECTED_TO_new_sdram_controller_0_wire_addr,  -- new_sdram_controller_0_wire.addr
			new_sdram_controller_0_wire_ba    => CONNECTED_TO_new_sdram_controller_0_wire_ba,    --                            .ba
			new_sdram_controller_0_wire_cas_n => CONNECTED_TO_new_sdram_controller_0_wire_cas_n, --                            .cas_n
			new_sdram_controller_0_wire_cke   => CONNECTED_TO_new_sdram_controller_0_wire_cke,   --                            .cke
			new_sdram_controller_0_wire_cs_n  => CONNECTED_TO_new_sdram_controller_0_wire_cs_n,  --                            .cs_n
			new_sdram_controller_0_wire_dq    => CONNECTED_TO_new_sdram_controller_0_wire_dq,    --                            .dq
			new_sdram_controller_0_wire_dqm   => CONNECTED_TO_new_sdram_controller_0_wire_dqm,   --                            .dqm
			new_sdram_controller_0_wire_ras_n => CONNECTED_TO_new_sdram_controller_0_wire_ras_n, --                            .ras_n
			new_sdram_controller_0_wire_we_n  => CONNECTED_TO_new_sdram_controller_0_wire_we_n,  --                            .we_n
			pwm_out_new_signal                => CONNECTED_TO_pwm_out_new_signal,                --                     pwm_out.new_signal
			reset_reset_n                     => CONNECTED_TO_reset_reset_n,                     --                       reset.reset_n
			sdram_clk_clk                     => CONNECTED_TO_sdram_clk_clk,                     --                   sdram_clk.clk
			encoder_in_new_signal             => CONNECTED_TO_encoder_in_new_signal              --                  encoder_in.new_signal
		);

