module REG_n_6 (
	input  wire 			clk,
	input  wire 			rst,
	input  wire				enable,
	input  wire	[5:0]		d,
	output reg	[5:0]		q
);

	REG_n Reg6 [5:0] (
			.clk(clk),
			.rst(rst),
			.enable(enable),
			.d(d),
			.q(q)
	);
	
endmodule
