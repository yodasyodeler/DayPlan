module REG_n (
	input  wire		clk,
	input  wire 	rst,
	input  wire		enable,
	input  wire		d,
	output reg		q
);

	always @ (negedge clk or posedge rst) begin
		if (rst)
			q <= 1'b0;
		else if (enable)
			q <= d;
		else
			q <= q;
	end
endmodule
