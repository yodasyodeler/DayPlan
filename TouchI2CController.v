/*-------------------------------------------------
/	File: 		TouchI2CController.v
/	Purpose: 	Hardware I2C module
/	Author:		Cory Barney
/------------------------------------------------*/

module TouchI2CController (
	input wire clk,
	input wire en,
	input wire reset,
	input wire busy,
	output reg enI2C, rw,
	output wire [7:0] data,
	output wire [6:0] address,
	output reg [5:0]load,
	output reg valid,
	inout wire resetTouch
);
	assign data 	= 8'h02;
	assign address = 7'h38;

	parameter initRST = 3'b000, waitRST = 3'b001, ready = 3'b010,  busyWait = 3'b011, NextOperation = 3'b100, stop = 3'b101;
			 
	reg [2:0]state;
	reg [15:0]count;
	reg resetEn;
	
	
	assign resetTouch = resetEn ? 1'bz : 1'b0;

	initial begin
			valid 				= 1'b0;
			resetEn 				= 1'b0;
			enI2C 				= 1'b0;
			rw 					= 1'b0;
			count					= 1'b0;
			state 				= initRST;
	end
	
	always @ (posedge clk) begin
		if (count == 0)
			valid = 1'b1;
		else
			valid = 1'b0;
	end
			
			
	always @ (posedge clk or negedge reset) begin
		if (!reset) begin
			resetEn 				= 1'b0;
			enI2C 				= 1'b0;
			rw 					= 1'b0;
			count					= 1'b0;
			state 				= initRST;
		end
		else begin
			case (state)
				initRST: begin
					resetEn 				= 1'b0;
					load					= 6'b000000;
					enI2C 				= 1'b0;
					rw 					= 1'b0;
					if (count < 8000) begin
						count 				= count + 1'b1;
						state 				= initRST;
					end
					else begin
						count 				= 16'b0;
						state 				= waitRST;
					end
				end
				waitRST: begin
					resetEn				= 1'b1;
					enI2C 				= 1'b0;
					load					= 6'b000000;
					rw 					= 1'b0;
					if (count < 48000) begin
						count 				= count + 1'b1;
						state 				= waitRST;
					end
					else begin
						count 				= 16'b0;
						state 				= ready;
					end
				end
				ready: begin
					resetEn				= 1'b1;
					load					= 6'b000000;
					enI2C 				= 1'b0;
					rw 					= 1'b0;
					count 				= 16'b0;
					if (en & !busy)
						state 				= busyWait;
					else
						state 				= ready;
				end
				busyWait: begin
					resetEn				= 1'b1;
					enI2C 				= 1'b1;
					rw 					= rw;
					count					= count;
					casex (count)
						16'b000:  load 			= 6'b000000;
						16'b001:  load 			= 6'b000001;
						16'b010:  load 			= 6'b000010;
						16'b011:  load 			= 6'b000100;
						16'b100:  load 			= 6'b001000;
						16'b101:  load 			= 6'b010000;
						16'b110:  load 			= 6'b100000;
						default:  load 			= 6'b000000;
					endcase
					if (busy)
						state 				= NextOperation;
					else	
						state 				= busyWait;
				end
				NextOperation: begin
					resetEn				= 1'b1;
					load					= 6'b000000;
					if (!busy)
//						if (count == 0)
//							enI2C 				= 1'b1;
//							rw 					= 1'b0;
//							count 				= count;
//							state 				= resetEn;
//						else
						if( count < 16'b111 ) begin
							enI2C 				= 1'b1;
							rw 					= 1'b1;
							count 				= count + 1'b1;
							state 				= busyWait;
						end
						else begin
							enI2C 				= 1'b0;
							rw						= 1'b1;
							count 				= 16'b0;
							state 				= stop;
						end
					else begin
						rw						= rw;
						enI2C 				= 1'b1;
						count 				= count;
						state					= NextOperation;
					end
				end
				stop: begin
					resetEn				= 1'b1;
					load					= 6'b000000;
					enI2C 				= 1'b0;
					rw 					= 1'b0;
					if (!en) begin
						//count 				= count + 1'b1;
						state 				= stop;
						end
					else begin
						//count 				= 16'b0;
						state 				= ready;
					end
				end
				default: begin
					resetEn				= 1'b1;
					load					= 6'b000000;
					enI2C 				= 1'b0;
					rw 					= 1'b0;
					count 				= 16'b0;
					state 				= initRST;
				end
			endcase
		end
	end
endmodule
