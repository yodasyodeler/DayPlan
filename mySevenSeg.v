module mySevenSeg(
	input wire [3:0]  dataIn,
	output reg [6:0] sevenSegs
);
	always @ (*) begin
		case (dataIn[3:0])
			4'b0000: sevenSegs[6:0] =  7'b1000000;
			4'b0001: sevenSegs[6:0] =  7'b1111001;
			4'b0010: sevenSegs[6:0] =  7'b0100100;
			4'b0011: sevenSegs[6:0] =  7'b0110000;
			4'b0100: sevenSegs[6:0] =  7'b0011001;
			4'b0101: sevenSegs[6:0] =  7'b0010010;
			4'b0110: sevenSegs[6:0] =  7'b0000010;
			4'b0111: sevenSegs[6:0] =  7'b1011000;
			4'b1000: sevenSegs[6:0] =  7'b0000000;
			4'b1001: sevenSegs[6:0] =  7'b0011000;
			4'b1010: sevenSegs[6:0] =  7'b0001000;
			4'b1011: sevenSegs[6:0] =  7'b0000011;
			4'b1100: sevenSegs[6:0] =  7'b1000110;
			4'b1101: sevenSegs[6:0] =  7'b0100001;
			4'b1110: sevenSegs[6:0] =  7'b0000110;
			4'b1111: sevenSegs[6:0] =  7'b0001110;
			default: sevenSegs[6:0] =  7'b1001001;
		endcase
	end
endmodule
