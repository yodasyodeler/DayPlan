module I2C_AVALON (
	input  wire clk, reset,
	
	input  wire i2c_chipselect, i2c_write,
	input  wire [1:0]i2c_address, 
	input  wire [31:0]i2c_writedata,
	output reg  [31:0]i2c_readdata,
	
	output wire i2c_irq,
	
	input wire  i2c_touch,
	output wire i2c_reset,
	inout  wire i2c_sda, i2c_sclk
);

	wire wr_en, wr_ben, wr_ctrl, wr_dvsr, i2c_idle, wr_data;
	wire [31:0] i2c_out;
	reg  reset_reg, irq_reg;
	reg [1:0] ben_reg;
	reg [15:0] dvsr_reg;
	reg touch_event;
	
	i2cModule i2c(
		.clk(clk), .en(wr_data),
		.data(i2c_writedata), .byteEn(ben_reg),
		.readData(i2c_out), .dvsr(dvsr_reg),
		.busy(), .idle(i2c_idle),
		.sda(i2c_sda), .scl(i2c_sclk)
	);
	
  always @(posedge clk or posedge reset)
    begin
      if (reset)
          touch_event <= 0;
      else
          if (wr_ctrl)
              touch_event <= 0;
          else if (i2c_touch)
              touch_event <= 1;
    end


  assign i2c_irq = irq_reg && touch_event;
	
	always @ (posedge clk or posedge reset)
	if (reset) begin
		reset_reg <= 1'b0;
		irq_reg <= 1'b0;
		ben_reg <= 2'b00;
		dvsr_reg <= 16'b0;
	end
	else begin
		if (wr_ctrl)
			{irq_reg, reset_reg} <= i2c_writedata[1:0];
		if (wr_ben)
			ben_reg <= i2c_writedata[1:0];
		if (wr_dvsr)
			dvsr_reg <= i2c_writedata[15:0];	
	end
	
	assign wr_en 		= i2c_write & i2c_chipselect;
	assign wr_data		= (i2c_address==2'b00) & wr_en;
	assign wr_ctrl		= (i2c_address==2'b01) & wr_en;
	assign wr_ben		= (i2c_address==2'b10) & wr_en;
	assign wr_dvsr		= (i2c_address==2'b11) & wr_en;
	
	assign i2c_reset 	= reset_reg;
	
	always @ (posedge clk) begin
		if (i2c_address==2'b01)
			i2c_readdata = {12'b0,i2c_idle, reset_reg, ben_reg, dvsr_reg};
		else
			i2c_readdata = i2c_out;
	end
endmodule
