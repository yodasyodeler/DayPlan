	component DE1_SOC_NIOS_2 is
		port (
			clk_clk                           : in    std_logic                     := 'X';             -- clk
			i2c_rst                           : out   std_logic;                                        -- rst
			i2c_sda                           : inout std_logic                     := 'X';             -- sda
			i2c_sclk                          : inout std_logic                     := 'X';             -- sclk
			lcd_lcd_cs                        : out   std_logic;                                        -- lcd_cs
			lcd_lcd_data                      : out   std_logic_vector(23 downto 0);                    -- lcd_data
			lcd_lcd_dc                        : out   std_logic;                                        -- lcd_dc
			lcd_lcd_rst                       : out   std_logic;                                        -- lcd_rst
			lcd_lcd_wr                        : out   std_logic;                                        -- lcd_wr
			new_sdram_controller_0_wire_addr  : out   std_logic_vector(12 downto 0);                    -- addr
			new_sdram_controller_0_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
			new_sdram_controller_0_wire_cas_n : out   std_logic;                                        -- cas_n
			new_sdram_controller_0_wire_cke   : out   std_logic;                                        -- cke
			new_sdram_controller_0_wire_cs_n  : out   std_logic;                                        -- cs_n
			new_sdram_controller_0_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			new_sdram_controller_0_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
			new_sdram_controller_0_wire_ras_n : out   std_logic;                                        -- ras_n
			new_sdram_controller_0_wire_we_n  : out   std_logic;                                        -- we_n
			reset_reset_n                     : in    std_logic                     := 'X';             -- reset_n
			sd_sd_cs                          : out   std_logic;                                        -- sd_cs
			sd_sd_clk                         : out   std_logic;                                        -- sd_clk
			sd_sd_di                          : out   std_logic;                                        -- sd_di
			sd_sd_do                          : in    std_logic                     := 'X';             -- sd_do
			sdram_clk_clk                     : out   std_logic                                         -- clk
		);
	end component DE1_SOC_NIOS_2;

	u0 : component DE1_SOC_NIOS_2
		port map (
			clk_clk                           => CONNECTED_TO_clk_clk,                           --                         clk.clk
			i2c_rst                           => CONNECTED_TO_i2c_rst,                           --                         i2c.rst
			i2c_sda                           => CONNECTED_TO_i2c_sda,                           --                            .sda
			i2c_sclk                          => CONNECTED_TO_i2c_sclk,                          --                            .sclk
			lcd_lcd_cs                        => CONNECTED_TO_lcd_lcd_cs,                        --                         lcd.lcd_cs
			lcd_lcd_data                      => CONNECTED_TO_lcd_lcd_data,                      --                            .lcd_data
			lcd_lcd_dc                        => CONNECTED_TO_lcd_lcd_dc,                        --                            .lcd_dc
			lcd_lcd_rst                       => CONNECTED_TO_lcd_lcd_rst,                       --                            .lcd_rst
			lcd_lcd_wr                        => CONNECTED_TO_lcd_lcd_wr,                        --                            .lcd_wr
			new_sdram_controller_0_wire_addr  => CONNECTED_TO_new_sdram_controller_0_wire_addr,  -- new_sdram_controller_0_wire.addr
			new_sdram_controller_0_wire_ba    => CONNECTED_TO_new_sdram_controller_0_wire_ba,    --                            .ba
			new_sdram_controller_0_wire_cas_n => CONNECTED_TO_new_sdram_controller_0_wire_cas_n, --                            .cas_n
			new_sdram_controller_0_wire_cke   => CONNECTED_TO_new_sdram_controller_0_wire_cke,   --                            .cke
			new_sdram_controller_0_wire_cs_n  => CONNECTED_TO_new_sdram_controller_0_wire_cs_n,  --                            .cs_n
			new_sdram_controller_0_wire_dq    => CONNECTED_TO_new_sdram_controller_0_wire_dq,    --                            .dq
			new_sdram_controller_0_wire_dqm   => CONNECTED_TO_new_sdram_controller_0_wire_dqm,   --                            .dqm
			new_sdram_controller_0_wire_ras_n => CONNECTED_TO_new_sdram_controller_0_wire_ras_n, --                            .ras_n
			new_sdram_controller_0_wire_we_n  => CONNECTED_TO_new_sdram_controller_0_wire_we_n,  --                            .we_n
			reset_reset_n                     => CONNECTED_TO_reset_reset_n,                     --                       reset.reset_n
			sd_sd_cs                          => CONNECTED_TO_sd_sd_cs,                          --                          sd.sd_cs
			sd_sd_clk                         => CONNECTED_TO_sd_sd_clk,                         --                            .sd_clk
			sd_sd_di                          => CONNECTED_TO_sd_sd_di,                          --                            .sd_di
			sd_sd_do                          => CONNECTED_TO_sd_sd_do,                          --                            .sd_do
			sdram_clk_clk                     => CONNECTED_TO_sdram_clk_clk                      --                   sdram_clk.clk
		);

