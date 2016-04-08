module sdram_read_fifo (
	input	wire			clock,
	input	wire			aclr,
	input	wire[15:0]	data,
	input	wire			rdreq,
	input	wire			wrreq,
	output wire[15:0]  	q,     
	output wire[5:0]  	usedw,
	output wire			empty,
	output wire			full   
);

	scfifo  scfifo_component (
			.clock 	 (clock),
			.aclr 	 (aclr),
			.data 	 (data),
			.rdreq 	 (rdreq),
			.wrreq 	 (wrreq),
			.q 		 (q),
			.usedw  	 (usedw),
			.empty  	 (empty),
			.full   	 (full)
	);
	defparam 
		scfifo_component.intended_device_family = "Cyclone V",
		scfifo_component.lpm_numwords = 64,
		scfifo_component.lpm_showahead = "OFF",
		scfifo_component.lpm_type = "scfifo",
		scfifo_component.lpm_width = 16,
		scfifo_component.lpm_widthu = 6,
		scfifo_component.overflow_checking = "OFF",
		scfifo_component.rdsync_delaypipe = 4,
		scfifo_component.read_aclr_synch = "OFF",
		scfifo_component.underflow_checking = "ON",
		scfifo_component.use_eab = "ON",
		scfifo_component.write_aclr_synch = "OFF",
		scfifo_component.wrsync_delaypipe = 4;
		
endmodule
