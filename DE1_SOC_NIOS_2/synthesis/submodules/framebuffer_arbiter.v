`timescale 1 ps / 1 ps
module framebuffer_arbiter (
	input  wire        	clk,              
   input  wire        	rst,              

// Memory-Mapped Master interface -- To SDRAM Controller
   input  wire          	m0_waitrequest,   
   input  wire          	m0_readdatavalid, 
	input  wire [15:0]   	m0_readdata,
		
	output reg  [25:0] 		m0_address,
	output reg        		m0_read_n,
	output wire		 			m0_write_n,
	output wire [15:0] 		m0_writedata,
	output wire 				m0_chipselect,
	output wire [1:0]			m0_byteenable_n,

// Memory-Mapped Slave interface -- Base Addresses
	input  wire   				s0_address,       
	input  wire        		s0_read,          
	input  wire        		s0_write,         
	input  wire [31:0] 		s0_writedata,     //New 	BaseAddress
	output reg  [31:0] 		s0_readdata,      //Current BaseAddress
	
    /* FIFO Out Export*/
	input  wire        		export_read_clk,
	input  wire 				export_frame_sync,
	input  wire        		export_as0_ready,  
	output wire        		export_as0_valid,   	
	output wire [15:0] 		export_as0_readdata 
);

	localparam BURSTLENGTH	= 6'd32;
	
	localparam 		IDLE = 0, 
						READ = 1,
						READ_DONE = 2; 
						
	assign m0_chipselect			= !m0_read_n;
	assign m0_byteenable_n		= 2'b00;
	assign m0_write_n				= 1'b1;
	assign m0_writedata			= 0;
	
	
	reg  [1:0] shiftValid;
	always @ (negedge export_read_clk) begin
		shiftValid[1] <= shiftValid[0]; 
		shiftValid[0] <= !rdfifo_rdempty;
	end
	
	
	assign export_as0_valid = shiftValid[1];
	
	reg [1:0]	current_state, next_state;


	reg [5:0]	burst_count;
	reg [5:0]	read_count;
	reg [25:0] 	read_Base_Addr;				

	wire [5:0] 	rdfifo_wrusedw;
	wire 			rdfifo_rdempty;        
	
	reg [25:0] 	newReadAddress;
	reg [25:0]  read_offset;
 
	
 //Instantiate Dual Clock FIFO| Read
	sdram_read_fifo  dcfifo_Read (
		.aclr 	(rst | softRst),
		.data 	(m0_readdata),
		.rdclk 	(!export_read_clk),
		.rdreq 	((export_as0_ready & shiftValid[0])),
		.wrclk 	(clk),
		.wrreq 	(m0_readdatavalid),
		.q 		(export_as0_readdata),
		.wrusedw (rdfifo_wrusedw),
		.rdempty (rdfifo_rdempty)
	);
	
	
//--------------------for Changing the Starting Address-------------------
	/*Write Operation*/
	always @(negedge clk) begin						
		if(rst) begin								
				newReadAddress				<= 0;
		end
		else if (s0_write) begin
			case(s0_address)
				1'b0 : 		newReadAddress 		<= s0_writedata[25:0];	
				default : 	newReadAddress 		<= newReadAddress;				
			endcase
		end
	end
	/*Read  Operation*/
	always @ (*) begin										// Read operations 
		if(s0_read) begin
			case(s0_address)
				1'b0 : 		s0_readdata 	= read_Base_Addr; 
				default : 	s0_readdata 	= 0;
			endcase
		end 
		else begin
			s0_readdata 		= 0;
		end
	end
//------------------------------------------------------------------------

	/*State Transistion*/
	always @(negedge clk ) begin
		  if(rst | softRst)
				current_state <= IDLE;
		  else
				current_state <= next_state;
	end
	
	
	/*Count & Offsets*/
	always @(posedge clk ) begin
		if(rst | softRst) begin
			read_count					<= 0;
			burst_count					<= 0;
			read_offset					<= 0;
			read_Base_Addr				<= newReadAddress;
		end 
		else begin
			read_count					<= read_count;
			burst_count 				<= burst_count;
			read_offset					<= read_offset;
			read_Base_Addr				<= read_Base_Addr;
			case(current_state)
				IDLE: begin
					read_count				<= 0;
					burst_count				<= 0;
					if (!read_Base_Addr[0]) begin
						read_Base_Addr 		<= newReadAddress;
						read_offset				<= 0;
					end
				end
				READ : begin
					if (m0_readdatavalid)
						read_count				<= read_count + 1'b1;
					if ((!m0_read_n) & (!m0_waitrequest))
						burst_count 			<= burst_count + 1'b1;
				end
				READ_DONE : begin
					read_offset				<= read_offset + (BURSTLENGTH<<1);
				end
			endcase
		 end
	 end
    
	/*State Transition & Data*/
	always @ (*) begin									
		m0_address 				= {read_Base_Addr[25:1],1'b0} + read_offset + {(burst_count),1'b0};
		
		m0_read_n					= 1'b1;		
		
		next_state 					= current_state;
		case(current_state)
			IDLE : begin	
				if((rdfifo_wrusedw[5] == 1'b0 ) & read_Base_Addr[0])										// Half Full Read
					next_state 				= READ;
			end
			READ : begin 			
				if ( burst_count < BURSTLENGTH ) begin
					m0_read_n						= 1'b0;
				end
				if ( read_count >= (BURSTLENGTH))
					next_state 					= READ_DONE;
			end
			READ_DONE: begin
				next_state  					= IDLE;
			end
		endcase
	end
	
	wire softRst;
	reg  [4:0]sync_reg;
	assign softRst = (sync_reg[0] & sync_reg[1] & !sync_reg[2] & !sync_reg[3] & !sync_reg[4]);
	
	always @ (posedge clk) begin
			sync_reg[0] <= export_frame_sync;
			sync_reg[1] <= sync_reg[0];
			sync_reg[2] <= sync_reg[1];
			sync_reg[3]	<= sync_reg[2];
			sync_reg[4]	<= sync_reg[3];
	end
endmodule
