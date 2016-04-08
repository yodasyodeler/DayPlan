`timescale 1 ps / 1 ps
module framebuffer_arbiter (
	input  wire        	clk,              
   input  wire        	rst,              

// Memory-Mapped Master interface -- To SDRAM Controller
	input  wire          	fb0_waitrequest,   
	input  wire          	fb0_readdatavalid, 
	input  wire [15:0]   	fb0_readdata,
	
	output reg  [25:0] 		fb0_address,
	output reg        		fb0_read_n,
	output wire		 			fb0_write_n,
	output wire [15:0] 		fb0_writedata,
	output wire 				fb0_chipselect,
	output wire [1:0]			fb0_byteenable_n,

// Memory-Mapped Slave interface -- Base Addresses                
	input  wire        		fb1_write,         
	input  wire [31:0] 		fb1_writedata,     //New 	BaseAddress
	output wire [31:0] 		fb1_readdata,      //Current BaseAddress
	
    /* FIFO Out Export */
	input  wire 				frame_sync,
	input  wire        		ready,  
	output wire        		valid,   	
	output wire [15:0] 		readdata 			
);

	localparam BURSTLENGTH	= 6'd32;
	
	localparam 	IDLE = 0, 
					READ = 1,
					READ_DONE = 2; 
						
	assign fb0_chipselect		= !fb0_read_n;
	assign fb0_byteenable_n		= 2'b00;
	assign fb0_write_n			= 1'b1;
	assign fb0_writedata			= 0;
	
	
	reg [1:0]	current_state, next_state;

	reg [5:0]	burst_count;
	reg [5:0]	read_count;
	reg [25:0] 	read_Base_Addr;				

	wire [5:0] 	rdfifo_wrusedw;
	wire 			rdfifo_rdempty;        
	
	reg [25:0] 	newReadAddress;
	reg [25:0]  read_offset;
 
 	reg  [1:0] shiftValid;
	always @ (negedge clk) begin
		shiftValid[1] <= shiftValid[0]; 
		shiftValid[0] <= !rdfifo_rdempty;
	end
	
	
	assign valid = shiftValid[1];
	
 //Instantiate Dual Clock FIFO| Read
	sdram_read_fifo  scfifo_Read (
		.clock(clk),
		.aclr((rst | frame_sync)),
		.data(fb0_readdata),
		.rdreq((ready & shiftValid[0])),
		.wrreq(fb0_readdatavalid),
		.q(readdata),
		.usedw(rdfifo_wrusedw),
		.empty(rdfifo_rdempty)
	);
	
	
	
//--------------------for Changing the Starting Address-------------------
	/*Write Operation*/
	always @(negedge clk) begin						
		if(rst)								
			newReadAddress	<= 0;
		else if (fb1_write)
			newReadAddress <= fb1_writedata[25:0];	
		else
			newReadAddress <= newReadAddress;
	end
	
	/*Read  Operation*/
	assign fb1_readdata 	= read_Base_Addr; 
//------------------------------------------------------------------------

	/*State Transistion*/
	always @(negedge clk) begin
		  if(rst | frame_sync)
				current_state <= IDLE;
		  else
				current_state <= next_state;
	end
	
	
	/*Count & Offsets*/
	always @(posedge clk ) begin
		if(rst | frame_sync) begin
			read_count				<= 0;
			burst_count				<= 0;
			read_offset				<= 0;
			read_Base_Addr			<= newReadAddress;
		end 
		else begin
			read_count				<= read_count;
			burst_count 			<= burst_count;
			read_offset				<= read_offset;
			read_Base_Addr			<= read_Base_Addr;
			case(current_state)
				IDLE: begin
					read_count			<= 0;
					burst_count			<= 0;
					if (!read_Base_Addr[0]) begin
						read_Base_Addr 	<= newReadAddress;
						read_offset			<= 0;
					end
				end
				READ : begin
					if (fb0_readdatavalid)
						read_count				<= read_count + 1'b1;
					if ((!fb0_read_n) & (!fb0_waitrequest))
						burst_count 			<= burst_count + 1'b1;
				end
				READ_DONE : begin
					read_offset				<= read_offset + (BURSTLENGTH<<2);	//16 bit accesses
				end
			endcase
		 end
	 end
    
	/*State Transition & Data*/
	always @ (*) begin									
		fb0_address 				= {read_Base_Addr[25:1],1'b0} + read_offset + {(burst_count),2'b0};
		fb0_read_n				= 1'b1;		
		next_state 				= current_state;
		case(current_state)
			IDLE : begin	
				if((rdfifo_wrusedw[5] == 1'b0 ) & read_Base_Addr[0])										// Half Full Read
					next_state 		= READ;
			end
			READ : begin 			
				if ( burst_count < BURSTLENGTH ) begin
					fb0_read_n			= 1'b0;
				end
				if ( read_count >= (BURSTLENGTH))
					next_state 		= READ_DONE;
			end
			READ_DONE: begin
				next_state  		= IDLE;
			end
		endcase
	end
endmodule
