module sdram_read_fifo (
	input	  				aclr,
	input	 [15:0]  	data,
	input	  				rdclk,
	input	  				rdreq,
	input	  				wrclk,
	input	  				wrreq,
	output [15:0]  	q,
	output	  			rdempty,
	output [5:0]  		rdusedw,
	output	  			wrfull,
	output [5:0]  		wrusedw
	);
	
	dcfifo	dcfifo_component (
				.rdclk 	(rdclk),
				.wrclk 	(wrclk),
				.wrreq 	(wrreq),
				.aclr 	(aclr),
				.data 	(data),
				.rdreq 	(rdreq),
				.wrfull 	(wrfull),
				.q 		(q),
				.rdempty (rdempty),
				.wrusedw (wrusedw),
				.rdusedw (rdusedw),
				.rdfull 	(),
				.wrempty ());
	defparam 
		dcfifo_component.intended_device_family = "Cyclone V",
		dcfifo_component.lpm_numwords = 64,
		dcfifo_component.lpm_showahead = "OFF",
		dcfifo_component.lpm_type = "dcfifo",
		dcfifo_component.lpm_width = 16,
		dcfifo_component.lpm_widthu = 6,
		dcfifo_component.overflow_checking = "OFF",
		dcfifo_component.rdsync_delaypipe = 4,
		dcfifo_component.read_aclr_synch = "OFF",
		dcfifo_component.underflow_checking = "ON",
		dcfifo_component.use_eab = "ON",
		dcfifo_component.write_aclr_synch = "OFF",
		dcfifo_component.wrsync_delaypipe = 4;
endmodule
