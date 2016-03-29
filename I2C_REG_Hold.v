module I2C_REG_Hold (
	input  wire 			clk,
	input  wire [5:0] 	load,
	input  wire [7:0] 	dataIn,
	output wire [47:0] 	dataOut    
);


	my8BitReg r0(
		.clk(clk),
		.rst(1), 
		.en(load[0]), 
		.D(dataIn),
		.Q(dataOut[7:0])
	);
	
	my8BitReg r1(
		.clk(clk),
		.rst(1), 
		.en(load[1]), 
		.D(dataIn),
		.Q(dataOut[15:8])
	);

	my8BitReg r2(
		.clk(clk),
		.rst(1), 
		.en(load[2]), 
		.D(dataIn),
		.Q(dataOut[23:16])
	);
	
		my8BitReg r3(
		.clk(clk),
		.rst(1), 
		.en(load[3]), 
		.D(dataIn),
		.Q(dataOut[31:24])
	);

	my8BitReg r4(
		.clk(clk),
		.rst(1), 
		.en(load[4]), 
		.D(dataIn),
		.Q(dataOut[39:32])
	);
	
		my8BitReg r5(
		.clk(clk),
		.rst(1), 
		.en(load[5]), 
		.D(dataIn),
		.Q(dataOut[47:40])
	);
	
endmodule
