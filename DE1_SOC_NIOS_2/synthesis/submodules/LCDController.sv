/*********************************************
 * LCDController.sv
 *
 *  Designed for SSD1963 TFT driver
 *    Runs through initialization for:
 *      16RGB
 *      800x480 screen
 *      7 inch TFT screen
 *    
 *    Code based on initialization sequence
 *    from BuyDisplay.com 
 *
 *********************************************/
 
//clock speed: 100MHz
// 100ms = (.1 	* 100MHz)
// 100us = (.0001 * 100MHz)
// 5ms   = (.005  * 100MHz)
// 384000pix = 480pix * 800pix 	
`define WAIT_100ms 	10000000	
`define WAIT_5ms 	500000
`define WAIT_100us 	10000
`define FRAME_SIZE  384000		

module LCDController (
	input  logic 				clk, rst,

	/* Control Interface */
	input logic [7:0]			dvsr,
	input logic 				wr_lcd, 
	
	/* Avalon Streaming Interface */
	input  logic [15:0]		dataIn,
	input  logic 				valid,
	output logic 				ready,
	
	/* LCD Control Signals */
	output logic				lcd_wr,
	output logic 				lcd_cs,
	output logic				lcd_dc,
	output logic 				lcd_rst,
	output logic [23:0]		lcd_data,
	
	/* Frame Sync for FrameBuffer Reader */
	output logic 				frame_sync      
);

	logic [8:0]	initCode [64] = '{ 
		9'hE2, 9'h123, 9'h102, 9'h154, 
		9'hE0, 9'h101, 
		// 100us wait
		9'hE0, 9'h103,
		// 100us wait
		9'h01,
		// 5ms wait
		9'hE6, 9'h103, 9'h133, 9'h133,
		9'hB0, 9'h120, 9'h100, 9'h103, 9'h11F, 9'h101, 9'h1DF, 9'h100,
		9'hB4, 9'h104, 9'h11f, 9'h100, 9'h1d2, 9'h100, 9'h100, 9'h100, 9'h100,
		9'hB6, 9'h102, 9'h10c, 9'h100, 9'h122, 9'h100, 9'h100, 9'h100,
		9'hB8, 9'h10f, 9'h101,
		9'hBA, 9'h101,
		9'h36, 9'h108,			// 9'h108
		9'h3A, 9'h150,			// Reserved Commands, may be unnessary
		9'hF0, 9'h103,
		9'hBC, 9'h140, 9'h180, 9'h140, 9'h101, 	
		// 5ms wait
		9'h29,
		// 5ms wait
		9'hBE, 9'h106, 9'h180, 9'h101, 9'h1f0, 9'h100, 9'h100,
		9'hd0, 9'h10d
	};

	logic [8:0] drawCode [11] = '{  9'h2A, 9'h100, 9'h100, 9'h103, 9'h11F,	// Start / End X
								    9'h2b, 9'h100, 9'h100, 9'h101, 9'h1DF,  // Start / End Y
								    9'h2c									// Start Color Data
	};
	
	typedef enum logic[3:0] {initRST0, initRST1, initCode0, initCode1, initCode2, 
							 wait100us, wait5ms, idle, drawRec0, drawRec1, drawRec2 } display_state;
	display_state state_reg, state_next;
	
	logic [23:0] c_reg, c_next;
	logic [15:0] data_reg, data_next;
	logic [22:0] index_reg, index_next;
	logic dc_reg, dc_next;
	logic ready_reg, ready_next;
	
	initial begin
	frame_sync 		= '0;
end
	
	always_ff @ (posedge clk or negedge rst) begin	
		if (!rst) begin
			state_reg	<= initRST0;
			c_reg		<= '0;
			index_reg 	<= '0;
			dc_reg		<= '0;
			data_reg	<= '0;
			ready_reg   <= '0;
		end	
		else begin
			state_reg 	<= state_next;
			c_reg 		<= c_next;
			index_reg   <= index_next;
			dc_reg		<= dc_next;
			data_reg 	<= data_next;
			ready_reg   <= ready_next;
		end
	end
	
	always_comb begin 
		state_next = state_reg;
		c_next = c_reg + 1'b1;
		index_next = index_reg;
		dc_next = dc_reg;
		data_next = data_reg;
		ready_next = 0;
		case (state_reg)
			initRST0:
				if (c_reg >= `WAIT_100ms) begin
					c_next = 24'b1;
					state_next = initRST1;
				end
			initRST1:
				if (c_reg >= `WAIT_100ms) begin
					c_next = 24'b1;
					index_next = '0;
					state_next = initCode0;
				end
				
			initCode0: begin
				dc_next = initCode[index_reg][8];
				data_next = {8'h0,initCode[index_reg][7:0]};
				if (c_reg >= (dvsr>>1)) begin
					c_next = 24'b1;
					state_next = initCode1;
				end
			end
			initCode1:
				if (c_reg >= dvsr) begin
					c_next = 24'b1;
					state_next = initCode2;
				end
			initCode2:
				if (c_reg >= (dvsr>>1)) begin
					c_next = 24'b1;
					index_next = index_reg + 1'b1;
					case (index_reg)
						5:	state_next = wait100us;
						7:  state_next = wait100us;
						8:  state_next = wait5ms;
						53: state_next = wait5ms;
						54: state_next = wait5ms;
						63: state_next = idle;
						default: state_next = initCode0;
					endcase
				end
				
			wait100us:
				if (c_reg >= `WAIT_100us) begin
					c_next = 24'b1;
					state_next = initCode0;
				end
			wait5ms:
				if (c_reg >= `WAIT_5ms) begin
					c_next = 24'b1;
					state_next = initCode0;
				end
					
			idle: begin
				c_next = 24'b1;
				index_next = '0;
				if (wr_lcd)
					state_next = drawRec0;
			end
			
			drawRec0: begin
				if (index_reg < 11) begin
					dc_next = drawCode[index_reg][8];
					data_next = {8'h0, drawCode[index_reg][7:0]};
					if (c_reg >= (dvsr>>1)) begin
						c_next = 24'b1;
						index_next = index_reg + 1'b1;
						state_next = drawRec1;
					end	
				end
				else if (valid) begin
					dc_next = 1'b1;
					data_next = dataIn;
					if (c_reg >= (dvsr>>1)) begin
						c_next = 24'b1;
						index_next = index_reg + 1'b1;
						ready_next = 1'b1;
						state_next = drawRec1;
					end	
				end	
			end
			drawRec1:
				if (c_reg >= dvsr) begin
					c_next = 24'b1;
					state_next = drawRec2;
				end
			drawRec2:
			if (c_reg >= (dvsr>>1)) begin
				c_next = 24'b1;
				if (index_reg >= `FRAME_SIZE + 11)
					state_next = idle;
				else
					state_next = drawRec0;
			end
				
			default:
				state_next = initRST0;
		endcase
	end
	
assign ready = ready_reg;

assign lcd_wr = ((state_reg != initCode1) && (state_reg != drawRec1));
assign lcd_cs = 0;
assign lcd_dc = dc_reg;
assign lcd_rst = (state_reg != initRST0);
assign lcd_data = data_reg;

assign frame_sync = (state_reg == idle);

	
	
endmodule
 
