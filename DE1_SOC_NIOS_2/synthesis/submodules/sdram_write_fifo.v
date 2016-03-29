module sdram_write_fifo (
	input	  				clock,
	input	  				aclr,
	input		[15:0]	data,
	input	  				rdreq,
	input	  				wrreq,
	output	[15:0]  	q,
	output	[5:0]  	usedw,
	output	  			empty,
	output	  			full
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
