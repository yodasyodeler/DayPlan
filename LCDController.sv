`define CLK_SPEED 	5000000
`define WAIT_200ms 	4600000		//(.2 	* `CLK_SPEED)
`define WAIT_100ms 	2300000		//(.1 	* `CLK_SPEED)
`define WAIT_100us 	2300			//(.0001 * `CLK_SPEED)
`define WAIT_5ms 		115000		//(.005 	* `CLK_SPEED) 

module LCDController (
	input  logic 					clk,
	input  logic 					rst,

	input  logic [15:0]			dataIn,
	input  logic 					valid,
	output logic 					ready,
	
	output logic					wr,
	output logic 					cs,
	output logic					dc,
	output logic 					rstOut,
	output logic [23:0]			dataOut,
	output logic 					frame_sync

);

	logic [8:0]	initCode [64] = '{ 
		9'hE2, 9'h123, 9'h102, 9'h154, 
		9'hE0, 9'h101, 
		//100 wait
		9'hE0, 9'h103,
		//100 wait
		9'h01,
		//5000 wait
		9'hE6, 9'h103, 9'h133, 9'h133,
		9'hB0, 9'h120, 9'h100, 9'h103, 9'h11F, 9'h101, 9'h1DF, 9'h100,
		9'hB4, 9'h104, 9'h11f, 9'h100, 9'h1d2, 9'h100, 9'h100, 9'h100, 9'h100,
		9'hB6, 9'h102, 9'h10c, 9'h100, 9'h122, 9'h100, 9'h100, 9'h100,
		9'hB8, 9'h10f, 9'h101,
		9'hBA, 9'h101,
		9'h36, 9'h108,			//9'h108
		9'h3A, 9'h150,				//resevered
		9'hF0, 9'h103,
		9'hBC, 9'h140, 9'h180, 9'h140, 9'h101, 	
		//5000 wait
		9'h29,
		//5000 wait
		9'hBE, 9'h106, 9'h180, 9'h101, 9'h1f0, 9'h100, 9'h100,
		9'hd0, 9'h10d
	};

	logic [8:0] recCode [11] = '{  9'h2A, 9'h100, 9'h100, 9'h103, 9'h11F,
											 9'h2b, 9'h100, 9'h100, 9'h101, 9'h1DF,
											 9'h2c
	};
	
	typedef enum logic[1:0] {init, initRST, initCommands, drawRec} display_state;
	display_state current_state;
	
	logic [24:0] wcount;
	logic [8:0]  count;
	
	initial begin
	count 			= '0;
	wcount 			= '0;
	current_state 	= init;
	frame_sync 				= '0;
end
	
	always_ff @ (posedge clk or negedge rst) begin	//frame_sync state logic/ count
		if (!rst) begin
			wcount 					<= '0;				
			count 					<= '0;
			current_state			<= init;
			frame_sync						<= '0;
		end	
		else begin
			case (current_state)
				init: begin
					frame_sync						<= '0;
					wcount 					<= '0;	
					count 					<= '0;
					current_state			<= initRST;
				end
				initRST: begin 
					frame_sync						<= '0;
					if (wcount < `WAIT_200ms) begin		//200ms
						wcount 				<= wcount + 1'b1;
						count					<= '0;
						current_state		<= initRST;
					end
					else begin
						wcount 				<= '0;
						count 				<= '0;
						current_state		<= initCommands;				
					end
				end
				initCommands: begin
					frame_sync						<= '0;
					if (count < 6<<1) begin
						wcount 					<= '0;
						count						<= count + 1'b1;
						current_state 			<= initCommands;
					end
					else if (wcount < `WAIT_100us && count  < (6<<1)+1) begin	//100us
						wcount 					<= wcount + 1'b1;
						count 					<= count;
						current_state 			<= initCommands;
					end
					else if (count  < 8<<1) begin
						wcount 					<= '0;
						count 					<= count + 1'b1;
						current_state 			<= initCommands;
					end
					else if (wcount < `WAIT_100us &&count < (8<<1)+1) begin	//100us
						wcount 					<= wcount + 1'b1;
						count 					<= count;
						current_state 			<= initCommands;
					end
					else if (count < 9<<1) begin
						wcount 					<= '0;
						count 					<= count + 1'b1;
						current_state 			<= initCommands;
					end
					else if (wcount < `WAIT_5ms && count  < (9<<1)+1) begin	//5ms
						wcount 					<= wcount + 1'b1;
						count 					<= count;
						current_state 			<= initCommands;
					end
					else if (count  < 54<<1) begin
						wcount 					<= '0;
						count 					<= count + 1'b1;
						current_state 			<= initCommands;
					end
					else if (wcount < `WAIT_5ms && count  < (54<<1)+1) begin	//5ms
						wcount 					<= wcount + 1'b1;
						count 					<= count;
						current_state 			<= initCommands;
					end
					else if (count  < 55<<1) begin
						wcount 					<= '0;
						count 					<= count + 1'b1;
						current_state 			<= initCommands;
					end
					else if (wcount < `WAIT_5ms && count  < (55<<1)+1) begin	//5ms
						wcount 					<= wcount + 1'b1;
						count 					<= count;
						current_state 			<= initCommands;
					end
					else if (count  < (63<<1)+1) begin
						wcount 					<= '0;
						count 					<= count + 1'b1;
						current_state 			<= initCommands;
					end
					else begin
						wcount					<= '0;
						count						<= '0;
						current_state 			<= drawRec;
//						wcount					<= wcount;
//						count 					<= count;
//						current_state			<= initCommands;
					end
				end
				drawRec: begin
					current_state				<= drawRec;
//					if (wcount < `WAIT_100us && count < 1) begin
//						count 					<= '0;
//						wcount					<= wcount + 1'b1;
//					end
//					else 
					if (count < 11<<1) begin
						frame_sync				<= '0;
						count 					<= count + 1'b1;
						wcount					<= '0;
					end
					else if (wcount < 768000) begin
						if (valid) begin
							frame_sync				<= '0;
							count						<= count;
							wcount 					<= wcount + 1'b1;
						end
						else begin
							frame_sync				<= '0;
							count						<= count;
							wcount 					<= wcount;
						end
					end
					else begin 
						frame_sync			<= 1'b1;
						count					<= '0;
						wcount				<= '0;
					end
				end
				default: begin
					count					<= count;
					wcount				<= '0;
					current_state		<= init;
					frame_sync			<= '0;
				end
			endcase
		end
	end

 
	always_ff @ (negedge clk) begin
		case (current_state)
			initCommands: begin
				if (count < 6<<1) begin
					wr			<= count[0];
				end
				else if (wcount < `WAIT_100us && count  < (6<<1)+1) begin	//100us
					wr			<= 1'b1;
				end
				else if (count  < 8<<1) begin
					wr			<= count[0];
				end
				else if (wcount < `WAIT_100us &&count < (8<<1)+1) begin //100us
					wr			<= 1'b1;
				end
				else if (count < 9<<1) begin
					wr			<= count[0];
				end
				else if (wcount < `WAIT_5ms && count  < (9<<1)+1) begin	//5ms
					wr			<= 1'b1;
				end
				else if (count  < 54<<1) begin
					wr			<= count[0];
				end
				else if (wcount < `WAIT_5ms && count  < (54<<1)+1) begin //5ms
					wr			<= 1'b1;
				end
				else if (count  < 55<<1) begin
					wr			<= count[0];
				end
				else if (wcount < `WAIT_5ms && count  < (55<<1)+1) begin //5ms
					wr			<= 1'b1;
				end
				else begin 
					wr			<= count[0];
				end
			end
			drawRec: begin
//				if (wcount < `WAIT_100us && count  < 1) begin	//100us
//					wr			<= 1'b1;
//				end
//				else 
				if (count < 11<<1)
					wr			<= count[0];
				else if (valid && wcount < 768000)
					wr			<= wcount[0];
				else
					wr			<= 1'b1;
			end
			default: wr <= 1'b1;
		endcase
	end
	
	always_comb begin					//DC/CS/rst/data
		dc						= 1'b1;
		cs						= 1'b0;
		rstOut				= 1'b1;
		dataOut				= '0;
		ready					= 1'b0;
		case (current_state)
			initRST: begin
				if (wcount < `WAIT_100ms)	//100000
					rstOut			= 1'b0;
			end
			initCommands: begin
					dc 					= initCode[count[8:1]][8];
					dataOut 				= initCode[count[8:1]][7:0];
			end
			drawRec: begin
				if (count < 11<<1) begin
					dc 					= recCode[count[8:1]][8];
					dataOut 				= recCode[count[8:1]][7:0];
				end
				else begin
					dc 					= 1'b1;
					dataOut				= dataIn;
					ready					= 1'b1;
				end
			end
			default: begin
			end
		endcase
	end
endmodule
 
