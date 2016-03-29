
module DE1_SOC_NIOS_2 (
	clk_clk,
	clock_23m_clk,
	clock_6400k_clk,
	i2c_rst,
	i2c_sda,
	i2c_sclk,
	new_sdram_controller_0_wire_addr,
	new_sdram_controller_0_wire_ba,
	new_sdram_controller_0_wire_cas_n,
	new_sdram_controller_0_wire_cke,
	new_sdram_controller_0_wire_cs_n,
	new_sdram_controller_0_wire_dq,
	new_sdram_controller_0_wire_dqm,
	new_sdram_controller_0_wire_ras_n,
	new_sdram_controller_0_wire_we_n,
	pixel_read_clk,
	pixel_ready,
	pixel_valid,
	pixel_readdata,
	pixel_frame_sync,
	reset_reset_n,
	sd_sd_cs,
	sd_sd_clk,
	sd_sd_di,
	sd_sd_do,
	sdram_clk_clk);	

	input		clk_clk;
	output		clock_23m_clk;
	output		clock_6400k_clk;
	output		i2c_rst;
	inout		i2c_sda;
	inout		i2c_sclk;
	output	[12:0]	new_sdram_controller_0_wire_addr;
	output	[1:0]	new_sdram_controller_0_wire_ba;
	output		new_sdram_controller_0_wire_cas_n;
	output		new_sdram_controller_0_wire_cke;
	output		new_sdram_controller_0_wire_cs_n;
	inout	[15:0]	new_sdram_controller_0_wire_dq;
	output	[1:0]	new_sdram_controller_0_wire_dqm;
	output		new_sdram_controller_0_wire_ras_n;
	output		new_sdram_controller_0_wire_we_n;
	input		pixel_read_clk;
	input		pixel_ready;
	output		pixel_valid;
	output	[15:0]	pixel_readdata;
	input		pixel_frame_sync;
	input		reset_reset_n;
	output		sd_sd_cs;
	output		sd_sd_clk;
	output		sd_sd_di;
	input		sd_sd_do;
	output		sdram_clk_clk;
endmodule
