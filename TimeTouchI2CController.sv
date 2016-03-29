module TimeTouchI2CController(
	input wire  clk,
	input wire  touchDetected,
	input wire  reset,
	input wire  busy,
	input wire  writeClock,
	input wire  [7:0]dataIn,
	
	output reg 	enI2C, rw,
	output wire [7:0] data,
	output wire [6:0] address,
	output reg  [11:0]	load,
	output reg  valid,
	
	inout wire  resetTouch
);

localparam INITRST 			= 4'b0000, 
		     WAITRST 			= 4'b0001, 
			  READY 				= 4'b0010,  
 
			  TOUCHNEXTOP 		= 4'b0100, 
			  TOUCHSTOP			= 4'b0101,

			  CLOCKNEXTOP		= 4'b0111,
			  CLOCKSTOP			= 4'b1000,

			  WRITEOP			= 4'b1010,
			  WRITESTOP			= 4'b1011;
			  
localparam touchAddress = 2'b10;

	reg [3:0] state;
	reg [15:0]count;
	reg [15:0]timeCount;
	reg resetEn;
	
	
	assign resetTouch = resetEn ? 1'bz : 1'b0;  
	
	reg [2:0]stableBusy;
	wire transition;
	assign transition = ~stableBusy[1] & stableBusy[2];

	always_ff @ (negedge clk) begin
		stableBusy[0] <= busy;
		stableBusy[1] <= stableBusy[0];
		stableBusy[2] <= stableBusy[1];
	end
	
			  
	always_ff @ (posedge clk  or negedge reset) begin
		count 						<= count;
		case (state)
			INITRST: begin
				if (count < 8000) begin
						count 				<= count + 1'b1;
						state 				<= INITRST;
					end
					else begin
						count 				<= 16'b0;
						state 				<= WAITRST;
					end
			end
			WAITRST: begin
				if (count < 48000) begin
						count 				<= count + 1'b1;
						state 				<= WAITRST;
				end
				else begin
					count 				<= 16'b0;
					state 				<= READY;
				end
			end	
			READY: begin
				count 				<= '0;
				if (!busy) begin
					if (writeClock)
						state					<= WRITEOP;
					else if (timeCount > 100)
						state 				<= CLOCKNEXTOP;
					else if (touchDetected)
						state 				<= TOUCHNEXTOP;
					else
						state 				<= READY;
				end
				else
					state					<= READY;
			end
			
			TOUCHNEXTOP: begin
				if (transition) begin
					if( count < 16'b111 ) begin
						count 				= count + 1'b1;
						state 				= TOUCHNEXTOP;
					end
					else begin
						count 				<= '0;
						state 				<= TOUCHSTOP;
					end
				end
				else begin
					count 				<= count;
					state					<= TOUCHNEXTOP;
				end
			end	
			TOUCHSTOP:	begin
				if (!touchDetected) begin
					state 				<= TOUCHSTOP;
				end
				else begin
					state 				<= READY;
				end
			end
			
			CLOCKNEXTOP: begin
				if (transition) begin
					if( count < 16'b111 ) begin
						count 				= count + 1'b1;
						state 				= WRITEOP;
					end
					else begin
						count 				<= '0;
						state 				<= WRITESTOP;
					end
				end
				else begin
					count 				<= count;
					state					<= WRITEOP;
				end
			end
			CLOCKSTOP: begin
				state 				<= READY;
			end	
			
			WRITEOP:	begin
				if (transition)
					state 				<= WRITESTOP;
				else 
					state 				<= READY;
			end
			WRITESTOP:	begin
				state 				<= WRITESTOP;
			end
			default: begin
				state 				<= READY;
				count					<= '0;
			end
		endcase
	end
			  
	always_comb begin
		resetEn  = 1'b1;
		valid    = 1'b0;
		enI2C    = 1'b0;
		data     = 8'b0;
		address  = 7'b0;
		load     = 12'b0;
		case (state)
			INITRST: begin
				resetEn 				= 1'b0;
			end		
			
			TOUCHNEXTOP: begin
				enI2C 				= 1'b1;
				if (count == '0)
					rw 					= 1'b0;
				else
					rw						= 1'b1;
				data = dataIn;
				casex (count)
					16'b000:  load 			= 12'b000000;
					16'b001:  load 			= 12'b000001;
					16'b010:  load 			= 12'b000010;
					16'b011:  load 			= 12'b000100;
					16'b100:  load 			= 12'b001000;
					16'b101:  load 			= 12'b010000;
					16'b110:  load 			= 12'b100000;
					default:  load 			= 12'b000000;
				endcase
			end

			CLOCKNEXTOP: begin
				enI2C 				= 1'b1;
				if (count == '0)
					rw 					= 1'b0;
				else
					rw						= 1'b1;

				casex (count)
					16'b000:  load 			= 12'b000000000000;
					16'b001:  load 			= 12'b000001000000;
					16'b010:  load 			= 12'b000010000000;
					16'b011:  load 			= 12'b000100000000;
					16'b100:  load 			= 12'b001000000000;
					16'b101:  load 			= 12'b010000000000;
					16'b110:  load 			= 12'b100000000000;
					default:  load 			= 12'b000000000000;
				endcase
			end
			WRITEOP: begin
				rw					= 1'b0;
				data				= dataIn;
			end
		endcase
	end
	
endmodule			  
