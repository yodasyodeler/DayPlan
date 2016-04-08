module LCD_AVALON (
	input  wire        	clk,              
   input  wire        	rst,              

// Memory-Mapped Master interface -- To SDRAM Controller);
	input  wire          	m0_waitrequest,   
	input  wire          	m0_readdatavalid, 
	input  wire [15:0]   	m0_readdata,
	
	output reg  [25:0] 		m0_address,
	output reg        		m0_read_n,
	output wire		 			m0_write_n,
	output wire [15:0] 		m0_writedata,
	output wire 				m0_chipselect,
	output wire [1:0]			m0_byteenable_n,

// Memory-Mapped Slave interface 
	input  wire [1:0]			s0_address,       
	input  wire        		s0_read,          
	input  wire        		s0_write,   
	input  wire            	s0_chipselect,
	input  wire [31:0] 		s0_writedata,
	output reg  [31:0] 		s0_readdata,      
	
// Export To LCD
	output wire					lcd_wr, 
   output wire 				lcd_cs,
   output wire					lcd_dc,
	output wire 				lcd_rst,
	output wire [23:0]		lcd_data
);
	
	
	/* Avalon Stream for internal communication */
	logic [15:0]readdata;
	logic valid, ready, frame_sync;
	
	/* frame Buffer */
	logic [15:0]fb1_readdata;
	
	framebuffer_arbiter fb(
		.clk(clk),              
		.rst(rst),   
		
		.fb0_waitrequest(m0_waitrequest),  
		.fb0_readdatavalid(m0_readdatavalid),
		.fb0_readdata(m0_readdata),
	
		.fb0_address(m0_address),
		.fb0_read_n(m0_read_n),
		.fb0_write_n(m0_write_n),
		.fb0_writedata(m0_writedata),
		.fb0_chipselect(m0_chipselect),
		.fb0_byteenable_n(m0_byteenable_n),
				
		.fb1_write(wr_frame),        
		.fb1_writedata(s0_writedata),    
		.fb1_readdata(fb1_readdata), 
	
		.frame_sync(frame_sync),
		.ready(ready),  
		.valid(valid),   	
		.readdata(readdata) 			
	);
	
	LCDController LCD(
	.clk(clk), .rst(!rst),
	
	.dvsr(dvsr_reg),
	.wr_lcd((wr_LCD | LCD_reg)), 
	
	.dataIn(readdata),
	.valid(valid),
	.ready(ready),
	
	/*export */
	.lcd_wr(lcd_wr),
	.lcd_cs(lcd_cs),
	.lcd_dc(lcd_dc),
	.lcd_rst(lcd_rst),
	.lcd_data(lcd_data),
	
	.frame_sync(frame_sync)     
);	
	
	/* Handle Addressing from  Memory-Mapped Slave interface */
	logic [7:0]dvsr_reg;	
	logic wr_en, wr_dvsr, wr_LCD, wr_frame;
	
	logic LCD_reg, LCD_busy;
	
	
	
	always_comb begin
		case (s0_address)
			2'b00: s0_readdata = {30'b0,LCD_busy,LCD_reg};
			2'b01: s0_readdata = {24'b0,dvsr_reg};
			2'b10: s0_readdata = 0;
			2'b11: s0_readdata = {16'b0,fb1_readdata};
		endcase
	end
	
	assign LCD_busy = !frame_sync;
	
	always_ff @ (posedge clk or posedge rst)
			if (rst) begin
				dvsr_reg 	<= 8'hff;
				LCD_reg		<= 0;
			end
			else begin
				if (wr_LCD)
					LCD_reg		<= s0_writedata[0];
				if (wr_dvsr)
					dvsr_reg		<= s0_writedata[7:0];
			end
			
		assign wr_en 	= (s0_write & s0_chipselect);
		
		assign wr_LCD  = (s0_address == 2'b00) & wr_en;
		assign wr_dvsr	= (s0_address == 2'b01) & wr_en;
		assign wr_frame = (s0_address == 2'b11) & wr_en;
		
		
endmodule
	