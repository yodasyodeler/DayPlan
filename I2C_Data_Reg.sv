module I2C_Data_Reg (
	input  wire					clk,
	input  wire					rst,
	/*	Slave MM Interface	*/
	input  wire [1:0] 		s0_address,     	
	input  wire        		s0_read,          
	input  wire        		s0_write,         
	input  wire [31:0] 		s0_writedata,     //New 	BaseAddress
	output reg  [31:0] 		s0_readdata,      //Current BaseAddress
	
	/*	Export Interface	*/
	input  wire 				export_valid,
	input  reg	[63:0]		export_data
);

	reg [7:0] data [8]; 
	reg [7:0] writeData [8];

	always_ff @ (negedge clk or posedge rst) begin
		if (rst)
			writeData = '{'0,'0,'0,'0,'0,'0,'0,'0};
		else begin
		writeData = writeData;
			if (s0_write) begin
				case (s0_address)
					2'b00: {writeData[2],writeData[3],writeData[4],writeData[5]} <= s0_writedata; 	//to do: fix registers when doing write interface
					2'b01: {writeData[7],writeData[6],writeData[0],writeData[1]} <= s0_writedata;
					default: writeData = writeData;
				endcase
			end
		end
	end
	
	always_comb begin
		case (s0_address)
			2'b00: 	s0_readdata = {data[2],data[3],data[4],data[5]};	//Touch Point 1 x, y 
			2'b01: 	s0_readdata = {data[7],data[6],data[0],data[1]}; 	//# of Touch Points
			default: s0_readdata = '0;
		endcase
	end
	
	always_ff @ (posedge clk or posedge rst) begin
		if (rst)
			data = '{'0,'0,'0,'0,'0,'0,'0,'0};
		else if (export_valid)
			data = '{export_data[7:0],export_data[15:8],export_data[23:16],export_data[31:24],export_data[39:32],export_data[47:40],export_data[55:48],export_data[63:56]};
		else
			data = data;
	end
		


endmodule
