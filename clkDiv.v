module clkDiv (
	input wire clkIn,
	output reg clkOut
);

	reg [2:0]count;

	always @ (posedge clkIn) begin
		if (count > 3'b100) begin
			clkOut = clkOut ^ 1'b1;
			count = 0;
		end
		else begin
			clkOut = clkOut;
			count = count + 1'b1;
		end
	end
endmodule
