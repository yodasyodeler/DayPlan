module REG_n_26 (
	input  wire 			clk,
	input  wire 			rst,
	input  wire				enable,
	input  wire	[25:0]	d,
	output reg	[25:0]	q
);

	REG_n Reg26 [25:0] (
			.clk(clk),
			.rst(rst),
			.enable(enable),
			.d(d),
			.q(q)
	);
	
endmodule
