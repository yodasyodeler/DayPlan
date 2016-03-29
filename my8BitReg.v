module my8BitReg (
	input wire clk,
	input wire rst, en, 
	input wire [7:0]D,
	output reg [7:0]Q
);

	always @ (posedge clk or negedge rst) begin
		if (!rst)
			Q = 7'b0;
		else if (en)
			Q = D;
		else
			Q = Q;
	end
endmodule

	
