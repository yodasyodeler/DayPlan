//`define ENABLE_HPS

module de1_soc_golden_top(
      ///////// ADC /////////
      inout              ADC_CS_N,
      output             ADC_DIN,
      input              ADC_DOUT,
      output             ADC_SCLK,

      ///////// AUD /////////
      input              AUD_ADCDAT,
      inout              AUD_ADCLRCK,
      inout              AUD_BCLK,
      output             AUD_DACDAT,
      inout              AUD_DACLRCK,
      output             AUD_XCK,

      ///////// CLOCK2 /////////
      input              CLOCK2_50,

      ///////// CLOCK3 /////////
      input              CLOCK3_50,

      ///////// CLOCK4 /////////
      input              CLOCK4_50,

      ///////// CLOCK /////////
      input              CLOCK_50,

      ///////// DRAM /////////
      output      [12:0] DRAM_ADDR,
      output      [1:0]  DRAM_BA,
      output             DRAM_CAS_N,
      output             DRAM_CKE,
      output             DRAM_CLK,
      output             DRAM_CS_N,
      inout       [15:0] DRAM_DQ,
      output             DRAM_LDQM,
      output             DRAM_RAS_N,
      output             DRAM_UDQM,
      output             DRAM_WE_N,

      ///////// FAN /////////
      output             FAN_CTRL,

      ///////// FPGA /////////
      output             FPGA_I2C_SCLK,
      inout              FPGA_I2C_SDAT,

      ///////// GPIO /////////
      inout     [35:0]         GPIO_0,
		inout     [35:0]         GPIO_1,
 

      ///////// HEX0 /////////
      output      [6:0]  HEX0,

      ///////// HEX1 /////////
      output      [6:0]  HEX1,

      ///////// HEX2 /////////
      output      [6:0]  HEX2,

      ///////// HEX3 /////////
      output      [6:0]  HEX3,

      ///////// HEX4 /////////
      output      [6:0]  HEX4,

      ///////// HEX5 /////////
      output      [6:0]  HEX5,

`ifdef ENABLE_HPS
      ///////// HPS /////////
//      inout              HPS_CONV_USB_N,
//      output      [14:0] HPS_DDR3_ADDR,
//      output      [2:0]  HPS_DDR3_BA,
//      output             HPS_DDR3_CAS_N,
//      output             HPS_DDR3_CKE,
//      output             HPS_DDR3_CK_N,
//      output             HPS_DDR3_CK_P,
//      output             HPS_DDR3_CS_N,
//      output      [3:0]  HPS_DDR3_DM,
//      inout       [31:0] HPS_DDR3_DQ,
//      inout       [3:0]  HPS_DDR3_DQS_N,
//      inout       [3:0]  HPS_DDR3_DQS_P,
//      output             HPS_DDR3_ODT,
//      output             HPS_DDR3_RAS_N,
//      output             HPS_DDR3_RESET_N,
//      input              HPS_DDR3_RZQ,
//      output             HPS_DDR3_WE_N,
//      output             HPS_ENET_GTX_CLK,
//      inout              HPS_ENET_INT_N,
//      output             HPS_ENET_MDC,
//      inout              HPS_ENET_MDIO,
//      input              HPS_ENET_RX_CLK,
//      input       [3:0]  HPS_ENET_RX_DATA,
//      input              HPS_ENET_RX_DV,
//      output      [3:0]  HPS_ENET_TX_DATA,
//      output             HPS_ENET_TX_EN,
//      inout       [3:0]  HPS_FLASH_DATA,
//      output             HPS_FLASH_DCLK,
//      output             HPS_FLASH_NCSO,
//      inout       [1:0]  HPS_GPIO,
//      inout              HPS_GSENSOR_INT,
//      inout              HPS_I2C1_SCLK,
//      inout              HPS_I2C1_SDAT,
//      inout              HPS_I2C2_SCLK,
//      inout              HPS_I2C2_SDAT,
//      inout              HPS_I2C_CONTROL,
//      inout              HPS_KEY,
//      inout              HPS_LED,
//      inout              HPS_LTC_GPIO,
      output             HPS_SD_CLK,
      inout              HPS_SD_CMD,
      inout       [3:0]  HPS_SD_DATA,
//      output             HPS_SPIM_CLK,
//      input              HPS_SPIM_MISO,
//      output             HPS_SPIM_MOSI,
//      inout              HPS_SPIM_SS,
//      input              HPS_UART_RX,
//      output             HPS_UART_TX,
//      input              HPS_USB_CLKOUT,
//      inout       [7:0]  HPS_USB_DATA,
//      input              HPS_USB_DIR,
//      input              HPS_USB_NXT,
//      output             HPS_USB_STP,
`endif /*ENABLE_HPS*/

      ///////// IRDA /////////
      input              IRDA_RXD,
      output             IRDA_TXD,

      ///////// KEY /////////
      input       [3:0]  KEY,

      ///////// LEDR /////////
      output      [9:0]  LEDR,

      ///////// PS2 /////////
      inout              PS2_CLK,
      inout              PS2_CLK2,
      inout              PS2_DAT,
      inout              PS2_DAT2,

      ///////// SW /////////
      input       [9:0]  SW,

      ///////// TD /////////
      input              TD_CLK27,
      input      [7:0]  TD_DATA,
      input             TD_HS,
      output             TD_RESET_N,
      input             TD_VS,


      ///////// VGA /////////
      output      [7:0]  VGA_B,
      output             VGA_BLANK_N,
      output             VGA_CLK,
      output      [7:0]  VGA_G,
      output             VGA_HS,
      output      [7:0]  VGA_R,
      output             VGA_SYNC_N,
      output             VGA_VS
);


//=======================================================
//  REG/WIRE declarations
//=======================================================


wire clk;
wire ready;
wire valid;
wire [15:0] pixel;
//wire [15:0] color[2] = '{16'hF800, 16'h07E0};
wire frame_sync;

wire clock_6400k;
wire clk_1600K;

wire dataReady;
wire busy;
wire enI2C;
wire rw;
wire [7:0]data;
wire [7:0]readData;
wire [6:0]address;
wire [5:0]load;
wire validDATAREG;

wire [63:0] dataRegBus; 

assign GPIO_1[2] = 1'bz;

//=======================================================
//  Structural coding
//=======================================================
    DE1_SOC_NIOS_2 u0 (
        .clk_clk                                               (CLOCK_50),             //                         clk.clk
        .clock_23m_clk                                         (clk),                  //                   clock_23m.clk
        .clock_6400k_clk                                       (clock_6400k),                     //                 clock_6400k.clk
        
			.pixel_readdata                        (pixel),                        				//                       pixel.data
			.pixel_ready                       		(ready),                       					//                            .ready
			.pixel_valid                       		(valid),                       					//                            .valid
			.pixel_read_clk                          (clk),
			.pixel_frame_sync                  		(frame_sync),
		  
		  .new_sdram_controller_0_wire_addr                      (DRAM_ADDR),            //                     new_sdram_controller_0_wire.addr
        .new_sdram_controller_0_wire_ba                        (DRAM_BA),              //                                                .ba
        .new_sdram_controller_0_wire_cas_n                     (DRAM_CAS_N),           //                                                .cas_n
        .new_sdram_controller_0_wire_cke                       (DRAM_CKE),             //                                                .cke
        .new_sdram_controller_0_wire_cs_n                      (DRAM_CS_N),            //                                                .cs_n
        .new_sdram_controller_0_wire_dq                        (DRAM_DQ),              //                                                .dq
        .new_sdram_controller_0_wire_dqm                       (DRAM_LDQM),            //                                                .dqm
        .new_sdram_controller_0_wire_ras_n                     (DRAM_RAS_N),           //                                                .ras_n
        .new_sdram_controller_0_wire_we_n                      (DRAM_WE_N),            //                                                .we_n
        
		  .reset_reset_n                                         (1'b1),                 //                                           reset.reset_n
		  .sdram_clk_clk                                         (DRAM_CLK),   				//                                       sdram_clk.clk
		  
//		  .i2c_data_valid                    							(validDATAREG),         //                    i2c_data.valid
//        .i2c_data_data                     							(dataRegBus),
		  .sd_sd_cs                          (GPIO_1[0]),                          	//                          sd.sd_cs
        .sd_sd_di                          (GPIO_1[1]),                          		//  
		  .sd_sd_clk                         (GPIO_1[2]),                         		//                            .sd_clk
        .sd_sd_do                          (GPIO_1[3]),                        		//                            .sd_do 
		  
		  .i2c_rst                           (GPIO_1[32]),                           //                         i2c.rst
        .i2c_sda                           (GPIO_1[35]),                           //                            .sda
        .i2c_sclk                          (GPIO_1[34])                          //                            .sclk
	 );
	 

	assign GPIO_0[24] = 1'b1;
	LCDController lcdCon (
		.clk(clk),
		.rst(1'b1),
		
		.dataIn(pixel),
		.valid(valid),
		.ready(ready),
		
		.wr(GPIO_0[25]),
		.cs(GPIO_0[27]),
		.dc(GPIO_0[26]),
		.rstOut(GPIO_0[28]),
		.dataOut(GPIO_0[23:0]),
		.frame_sync(frame_sync)
	);
	
//	framebuffer_arbiter a1();
	
//	clkDiv u1 (
//			.clkIn(clock_6400k),
//			.clkOut(clk_1600K)
//	);
	 
//	 TouchI2CController t0(
//		.clk(clk_1600K),
//		.en(!GPIO_1[2]),
//		.reset(1),
//		.busy(busy),
//		.enI2C(enI2C), 
//		.rw(rw),
//		.data(data),
//		.address(address),
//		.load(load),
//		.valid(validDATAREG),
//		.resetTouch(GPIO_1[3])
//	);
	 
//	 i2cModule i0(
////		.clk(clk_1600K),	
////		.en(enI2C), 
////		//.rw(rw),
////		//.addressBits(address),
////		.data(data),
////		.readData(readData),
////		//.busy(busy),
////		.sda(GPIO_1[0]), 
////		.scl(GPIO_1[1])
//	);
	
//.clk(CLOCK_50), 
//.reset(1'b1),
//.din(),
//.wr(),
//
//.sdat(GPIO_1[0]),
//.sclk(GPIO_1[1]), 
//.idle(), 
//.fail(), 
//.done()
	
//	I2C_REG_Hold REG_H(
//		.clk 			(clk_1600K),
//		.load			(load),
//		.dataIn  	(readData),
//		.dataOut 	(dataRegBus)
//	);

endmodule
