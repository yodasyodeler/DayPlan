module direct_connect (
	input  logic        	clk,              
   input  logic        	rst,              

// Memory-Mapped Master interface -- To SDRAM Controller
   input  logic         m0_waitrequest,   
   input  logic         m0_readdatavalid, 
	input  logic [15:0]  m0_readdata,
		
	output logic [25:0] 	m0_address,
	output logic        	m0_read_n,
	output logic			m0_write_n,
	output logic [15:0] 	m0_writedata,
	output logic 			m0_chipselect,
	output logic [1:0]	m0_byteenable,
        
// Memory-Mapped Slave interface -- Base Addresses
	input  logic [25:0]  s0_address,       
	input  logic        	s0_read,          
	input  logic        	s0_write,         
	input  logic [15:0] 	s0_writedata,    
	input  logic			s0_chipselect,
	input  logic [1:0]	s0_byteenable,
	
	output logic         s0_waitrequest,   
   output logic         s0_readdatavalid, 
	output logic [15:0] 	s0_readdata
	);
	
	assign s0_waitrequest   = m0_waitrequest;
	assign s0_readdatavalid = m0_readdatavalid;
	assign s0_readdata		= m0_readdata;
	
	assign m0_address			= s0_address;
	assign m0_read_n			= !s0_read;
	assign m0_write_n			= !s0_write;
	assign m0_writedata		= s0_writedata;
	assign m0_chipselect		= s0_chipselect;
	assign m0_byteenable		= s0_byteenable;
		
endmodule
