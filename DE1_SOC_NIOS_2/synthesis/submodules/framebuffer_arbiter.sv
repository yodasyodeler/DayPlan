`timescale 1 ps / 1 ps
module framebuffer_arbiter (
	input  wire        	clk,              
	input  wire        	read_clk,
   input  wire        	rst,              

// Memory-Mapped Master interface -- To SDRAM Controller
   input  wire         m0_waitrequest,   
   input  wire         m0_readdatavalid, 
	input  wire [15:0]  m0_readdata,
		
	output reg [25:0] 	m0_address,
	output wire        	m0_read_n,
	output wire		 		m0_write_n,
	output reg  [15:0] 	m0_writedata,
	output wire 			m0_chipselect,
	output wire [1:0]		m0_byteenable,
        
// Memory-Mapped Slave interface -- Base Addresses
	input  wire [25:0]  	s0_address,       
	input  wire        	s0_read,          
	input  wire        	s0_write,         
	input  wire [15:0] 	s0_writedata,    
	input  wire				s0_chipselect,
	input  wire [1:0]	   s0_byteenable,
	
	output wire          s0_waitrequest,   
   output wire          s0_readdatavalid, 
	output wire [15:0] 	s0_readdata,      
	
// Memory-Mapped Slave interface -- Base Addresses
	input  wire   			s1_address,       
	input  wire        	s1_read,          
	input  wire        	s1_write,         
	input  wire [31:0] 	s1_writedata,     //New 	BaseAddress
	output reg  [31:0] 	s1_readdata,      //Current BaseAddress
	
    /* FIFO Out Export*/
	input  wire        	aso_out0_ready,   
	output wire        	aso_out0_valid,   	
	output wire [15:0] 	aso_out0_readdata  		//Data to read module
);

	localparam FIFO_DEPTH  = 64;
	localparam BURST_COUNT = 32;
	localparam FRAME_PIX_SIZE  = 480*800;
	
	localparam IDLE = 0, 
				  START_WRITE = 1, 
				  WRITE = 2,  
				  READ = 3,
				  MASTER_READ = 4, 
				  MASTER_READ_DONE = 5; 

	assign s0_waitrequest		=		wrfifo_wrfull | (write_Base_Addr != newWriteAddress);
	assign s0_readdatavalid		=		shiftValid[1];
	assign s0_readdata			=		m0_readdata;
	assign m0_read_n				=		read_n | m0_waitrequest;
	assign m0_chipselect			=		!m0_write_n | !m0_read_n;
	assign m0_byteenable			=		2'b11;
	assign m0_write_n				= 		write_n | m0_waitrequest;
	assign aso_out0_valid		= 		wrfifo_rdreq;


	REG_n current_burstcount [5:0] (
			.clk(clk),
			.rst(burstRst),
			.enable(burstEn),
			.d(burstIn),
			.q(burstOut)
	);
	
	REG_n_26 rdoffset (
			.clk(clk),
			.rst(rdoffRst),
			.enable(rdoffEn),
			.d(rdoffIn),
			.q(rdoffOut)
	);
	
	REG_n_26 wroffset (
			.clk(clk),
			.rst(wroffRst),
			.enable(wroffEn),
			.d(wroffIn),
			.q(wroffOut)
	);
	
	
	reg [2:0]		current_state, next_state;
	reg 				write_n, read_n;

	reg [5:0]  burstOut, burstIn;			// Current Burst Transaction
	reg 			 burstRst, burstEn;
	
	reg [25:0]  	rdoffOut, rdoffIn;			// Current Burst Transaction
	reg 		 		rdoffRst, rdoffEn;
	
	reg [25:0]  	wroffOut, wroffIn;			// Current Burst Transaction
	reg 		 		wroffRst, wroffEn;
	
	reg [5:0]	 read_count;
	reg [25:0] read_Base_Addr;				// Base address of framebuffer
	reg [25:0] write_Base_Addr;				// Base address of framebuffer

//	// Data and control signals for FIFO
	wire rdfifo_wrreq;           // Write requests
	wire rdfifo_rdempty;                     // Read FIFO empty
	wire [5:0] rdfifo_wrusedw;               // Write FIFO fill level
	wire wrfifo_rdreq;           // Read/Write requests
	wire wrfifo_wrfull;                     // Read FIFO empty
	wire wrfifo_empty;

	reg [25:0] newReadAddress;
	reg [25:0] newWriteAddress;

// Instantiate Dual Clock FIFO| Writes
	sdram_write_fifo scfifo_Write (
		.aclr 	 (rst),
		.data 	 (s0_writedata),
		.clock 	 (clk),
		.rdreq 	 (wrfifo_rdreq),
		.wrreq 	 (s0_write),
		.q 		 (m0_writedata),
		.empty  	 (wrfifo_empty),
		.full   	 (wrfifo_wrfull)
	);

	
// Instantiate Dual Clock FIFO| Read
	sdram_read_fifo  dcfifo_Read (
		.aclr 	(rst),
		.data 	(m0_readdata),
		.rdclk 	(read_clk),
		.rdreq 	(aso_out0_ready & aso_out0_valid),
		.wrclk 	(clk),
		.wrreq 	(rdfifo_wrreq),
		.q 		(aso_out0_readdata),
		.rdempty (rdfifo_rdempty),
		.wrusedw (rdfifo_wrusedw)
	);
	
	/*Align readdatavalid*/
	reg [1:0] shiftValid;
	always @(negedge clk) begin
		shiftValid[0] <= m0_readdatavalid;
		shiftValid[1] <= shiftValid[0];
	end
	
	
//--------------------for Changing the Starting Address-------------------
	/*Write Operation*/
	always @(posedge clk) begin						
		if(rst) begin								
				newWriteAddress 			<= 0;
				newReadAddress				<= 0;
		end
		else if (s1_write) begin
			case(s1_address)
				1'b0 : 		begin	newReadAddress 		<= s1_writedata[25:0];		newWriteAddress		<= newWriteAddress;	end
				1'b1 : 		begin	newWriteAddress		<= s1_writedata[25:0];		newReadAddress 		<= newReadAddress;	end
				default : 	begin newReadAddress 		<= newReadAddress;	newWriteAddress		<= newWriteAddress;	end
			endcase
		end
	end
	/*Read  Operation*/
	always @ (*) begin										// Read operations 
		if(s1_read) begin
			case(s1_address)
				1'b0 : 		s1_readdata 	= read_Base_Addr; 
				1'b1 :		s1_readdata		= write_Base_Addr + wroffOut;
				default : 	s1_readdata 	= 0;
			endcase
		end 
		else begin
			s1_readdata 		= 0;
		end
	end
//------------------------------------------------------------------------

	/*State Transistion*/
	always @(negedge clk) begin
		  if(rst)
				current_state <= IDLE;
		  else
				current_state <= next_state;
	end
	
	
	/*Count & Offsets*/
	always @(negedge clk) begin
		if(rst) begin
			read_count					<= 0;
			read_Base_Addr				<= 0;
			write_Base_Addr			<= 0;
		end 
		else begin
			read_Base_Addr				<= read_Base_Addr;
			write_Base_Addr			<= write_Base_Addr;
			read_count					<= 0;
			case(current_state)
				IDLE: begin
					if (wrfifo_empty & (write_Base_Addr != newWriteAddress)) begin	//Change Address
						write_Base_Addr 	<= newWriteAddress;
					end
					if ( rdoffOut >= (FRAME_PIX_SIZE<<1) ) begin							//Wrap Read Operations										
						read_Base_Addr				<= newReadAddress;
					end
				end
				READ : begin
					if (shiftValid[1])
						read_count				<= read_count + 1'b1;
				end
			endcase
		 end
	 end
    
	/*State Transition & Data*/
	always @ (*) begin									
		m0_address					= 0;
		write_n						= 1'b1;
		read_n						= 1'b1;
		
		rdfifo_wrreq 				= 1'b0;
		wrfifo_rdreq				= 1'b0;
		
		burstIn						= burstOut + 1'b1;
		burstEn						= 1'b0;
		burstRst						= 1'b1;
		
		rdoffIn						= rdoffOut + 2'b10;
		rdoffEn						= 1'b0;
		rdoffRst						= 1'b0;
		
		wroffIn						= wroffOut + 2'b10;
		wroffEn						= 1'b0;
		wroffRst						= 1'b0;
		
		next_state 					= current_state;		
		case(current_state)
			IDLE : begin												
				if((rdfifo_wrusedw[5] == 1'b0 ))										// Half Full Read
					next_state 				= READ;
				else 
				if (!wrfifo_empty)													// Data Ready
					next_state				= START_WRITE;
//				else if (read_Back_Address[26] & !m0_waitrequest) begin		// M Read Request
//					read_n					=	1'b0;
//					next_state				= MASTER_READ;
//				end
				if ( rdoffOut >= (FRAME_PIX_SIZE<<1) ) 							//Wrap Read Operations
					rdoffRst					= 1'b1;
				if (wrfifo_empty & (write_Base_Addr != newWriteAddress))
					wroffRst					= 1'b1;
			end
			READ : begin 
				burstRst					= 1'b0;			
				m0_address 				= read_Base_Addr + rdoffOut;	
				if (!m0_waitrequest & burstOut < BURST_COUNT) begin
					read_n						= 1'b0;
					burstEn						= 1'b1;
					rdoffEn						= 1'b1;
				end
				if(shiftValid[1])
					rdfifo_wrreq 				= 1'b1;  
				if(read_count >= BURST_COUNT)
					next_state 					= IDLE;
			end
			START_WRITE : begin
				burstRst					= 1'b0;
				m0_address 				= write_Base_Addr + wroffOut;	
				wrfifo_rdreq 			= 1'b1;
				next_state				= WRITE;
			end
			WRITE : begin
				burstRst					= 1'b0;
				m0_address 				= write_Base_Addr + wroffOut;	
				write_n					= 1'b0;
				if(!m0_waitrequest & !wrfifo_empty)
					wroffEn					= 1'b1;
					burstEn					= 1'b1;
					wrfifo_rdreq 			= 1'b1;  	
				if (wrfifo_empty || (burstOut >= BURST_COUNT))
					next_state 				= IDLE;
			end
//			MASTER_READ : begin
//				m0_address				= read_Back_Address[25:0];
//				if (m0_readdatavalid)
//					next_state				= MASTER_READ_DONE;
//			end
//			MASTER_READ_DONE : begin
//					next_state				= IDLE;
//			end
		endcase
	end
//------------------------------------------------------------------------
	
	
//----------------------------Slave Pending Reads-------------------------
//	reg [26:0] 	read_Back_Address;
//	always @ (negedge clk) begin
//		read_Back_Address				<= read_Back_Address;
//		if (s0_read)
//			read_Back_Address				<= {1'b1,s0_address};
//		case (current_state)
//			MASTER_READ_DONE:	read_Back_Address 			<= 0;
//		endcase
//	end	
//------------------------------------------------------------------------
endmodule
