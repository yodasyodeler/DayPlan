module i2cModule(
	input wire clk,
	input wire en,
	
	input wire [31:0]data,
	input wire [1:0]byteEn,
	input wire [15:0]dvsr,
	
	output reg [31:0]readData,
	output reg idle,
	
	inout wire sda, scl
);
	
	localparam HALF = 249;
	localparam QATR = 125;
	
	parameter init = 5'd0, ready = 5'd1, start1 = 5'd2, start2 = 5'd3, start3 = 5'd4, readData1 = 5'd5, readData2 = 5'd6, readData3 = 5'd7, 
			  readAck1 = 5'd8, readAck2 = 5'd9, readAck3 = 5'd10, readStart1 = 5'd11, readStart2 = 5'd12, readStart3 = 5'd13, readStart4 = 5'd14, data1 = 5'd15, 
			  data2 = 5'd16, data3 = 5'd17, data4 = 5'd18, Ack1 = 5'd19, Ack2 = 5'd20, Ack3 = 5'd21, stop1 = 5'd22, stop2 = 5'd23, stop3 = 5'd24;
			  
	reg [4:0]state, nstate;
	reg [15:0]Count_reg, Count_next;
	
	reg rwBit_next, rwBit_reg;
	reg scl_next, scl_reg;
	reg sda_next, sda_reg;
	
	reg [2:0] bit_reg, bit_next;
	reg [2:0] byte_reg, byte_next;
	reg [2:0] byteEn_reg, byteEn_next;
	reg [31:0] data_reg, data_next;
	reg [15:0] dvsr_reg;
	
	reg [31:0]readData_next;
	reg [15:0]readAddress_reg, readAddress_next;
	
	assign sda = sda_reg ? 1'bz : 1'b0;
	assign scl = scl_reg ? 1'bz : 1'b0;
	
	
	always @ ( posedge  clk) begin
		state 		<= nstate;
		Count_reg 	<= Count_next;
		data_reg		<= data_next;
		dvsr_reg		<= dvsr;
		readData		<= readData_next;
		bit_reg 		<= bit_next;
		scl_reg		<= scl_next;
		sda_reg		<= sda_next;
		rwBit_reg   <= rwBit_next;
		byte_reg		<= byte_next;
		byteEn_reg  <= byteEn_next;
		readAddress_reg <= readAddress_next;
	end
	
	always @ (*) begin
		idle 		= 1'b0;
		scl_next 	= scl_reg;
		sda_next 		= sda_reg;
		rwBit_next		= rwBit_reg;
		Count_next 	= Count_reg + 1'b1; 
		nstate		= state;
		readData_next 	= readData;
		bit_next	= bit_reg;
		byte_next	= byte_reg;
		data_next 	= data_reg;
		byteEn_next = byteEn_reg;
		readAddress_next = readAddress_reg;
		case (state)
			init: begin
				sda_next		= 1'b1;
				scl_next		= 1'b1;
				Count_next	= 0;
				nstate 		= ready;
			end
			
			ready: begin
				idle		= 1'b1;
				Count_next	= 0;
				sda_next		= 1'b1;
				scl_next		= 1'b1;
				if (en) begin
					bit_next		= 0;
					byte_next	= 0;
					readData_next	= 0;
					rwBit_next		= data[24];
					data_next	= data;
					byteEn_next = byteEn;
					readAddress_next = {data[31:25], 1'b0, data[23:16]};
					nstate		= start1;
				end
			end
			
			start1: begin
				sda_next 		= 1'b1;
				scl_next		= 1'b1;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					nstate 		= start2;
				end
			end
			start2: begin
				sda_next 		= 1'b0;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					nstate 		= start3;
				end
			end
			start3: begin
				scl_next	= 1'b0;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					if (rwBit_reg)
						nstate 	= readData1;
					else
						nstate 	= data1;
				end
			end
			
			/* Write Address for Read */
			readData1: begin
				sda_next	= readAddress_reg[15];
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					 Count_next	= 0;
					 nstate		= readData2;
				end
			end
			readData2: begin
				scl_next		= 1'b1;
				if (Count_reg >=  (dvsr_reg>>1) ) begin
					Count_next	= 0;
					nstate		= readData3;
				end
			end
			readData3: begin
				scl_next 		= 1'b0;
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next 	= 0;
					readAddress_next	= {readAddress_reg[14:0], 1'b0};
					if (bit_reg >= 7) begin
						bit_next 	= 0;
						nstate		= readAck1;
					end
					else begin
						bit_next	= bit_reg + 1'b1;
						nstate		= readData1;
					end
				end
			end
			
			readAck1: begin
				scl_next	= 1'b0;
				sda_next 	= 1'b1;
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next	= 0;
					nstate		= readAck2;
				end
			end
			readAck2: begin
				scl_next		= 1'b1;
				if (Count_reg >  (dvsr_reg>>1) ) begin
					Count_next	= 0;
					nstate		= readAck3;
				end
			end
			readAck3: begin
				scl_next		= 1'b0;
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next	= 0;
					if (byte_reg >= 1) begin
						byte_next	= 0;
						nstate		= readStart1;
					end
					else begin
						byte_next	= byte_reg + 1'b1;
						nstate 		= readData1;
					end
				end
			end

			readStart1: begin
				sda_next 		= 1'b1;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					nstate 		= readStart2;
				end
			end
			readStart2: begin
				scl_next		= 1'b1;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					nstate 		= readStart3;
				end
			end
			readStart3: begin
				sda_next 		= 1'b0;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					nstate 		= readStart4;
				end
			end
			readStart4: begin
				scl_next	= 1'b0;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next  = 0;
					byteEn_next = 4;
					nstate 		= data1;
				end
			end
			
			/* End Read Address Change */
			

			data1: begin
				if (rwBit_reg && byte_reg != 0)
					sda_next 	= 1'b1;
				else
					sda_next	= data_next[31];
					
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next	= 0;
					nstate		= data2;
				end
			end
			data2: begin
				scl_next		= 1'b1;
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					if (rwBit_reg && byte_reg != 0)
						readData_next = {readData[30:0], sda};
					Count_next	= 0;
					nstate		= data3;
				end
			end
			data3: begin
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next	= 0;
					nstate		= data4;
				end
			end
			data4: begin
				scl_next 		= 1'b0;
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next 	= 0;
					data_next	= {data_reg[30:0], 1'b0};
					if (bit_reg >= 7) begin
						bit_next 	= 0;
						nstate		= Ack1;
					end
					else begin
						bit_next		= bit_reg + 1'b1;
						nstate		= data1;
					end
				end
			end
			
			Ack1: begin
				scl_next		= 1'b0;
				if (rwBit_reg && byte_reg != 0 && byte_reg != byteEn_reg)
					sda_next = 1'b0;
				else
					sda_next = 1'b1;
				
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next	= 0;
					nstate		= Ack2;
				end
			end
			Ack2: begin
				scl_next		= 1'b1;
				if (Count_reg >  (dvsr_reg>>1) ) begin
					Count_next	= 0;
					nstate		= Ack3;
				end
			end
			Ack3: begin
				scl_next		= 1'b0;
				if (Count_reg >=  (dvsr_reg>>2) ) begin
					Count_next	= 0;
					if (byte_reg >= byteEn_reg)
						nstate		= stop1;
					else begin
						nstate 		= data1;
						byte_next	= byte_reg + 1'b1;
					end
				end
			end

			stop1: begin
				scl_next = 1'b0;
				sda_next	= 1'b0;
				if (Count_reg >= (dvsr_reg>>2) ) begin
					Count_next	= 0;
					nstate		= stop2;
				end
			end
			stop2: begin
				scl_next	= 1'b1;
				if (Count_reg >= (dvsr_reg>>1) ) begin
					Count_next	= 0;
					nstate		= stop3;
				end
			end
			stop3: begin
			sda_next = 1'b1;
				if (Count_reg >= dvsr_reg>>1) begin
					nstate = idle;
				end
			end
			default: begin
				nstate		= init;
			end
		endcase
	end

endmodule	

// address1: begin
				// sda_next	= data_next[31];
				// if (Count_reg >=  QATR) begin
					// Count_next	= 0;
					// nstate		= address2;
				// end
			// end
			// address2: begin
				// scl_next		= 1'b1;
				// if (Count_reg >=  HALF) begin
					// Count_next	= 0;
					// nstate		= address3;
				// end
			// end
			// address3: begin
				// scl_next 		= 1'b0;
				// if (Count_reg >=  QATR) begin
					// Count_next 	= 0;
					// if (bit_reg >= 7) begin
						// bit_next 	= 0;
						// byte_next	= byte_reg + 1'b1;
						// nstate		= Ack1;
					// end
					// else begin
						// data_next	= {data_reg[30:0], 1'b0};
						// bit_next	= bit_reg + 1'b1;
						// nstate		= Address1;
					// end
			// end