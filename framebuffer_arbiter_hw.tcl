# TCL File Generated by Component Editor 15.0
# Wed Jan 27 01:12:54 PST 2016
# DO NOT MODIFY


# 
# framebuffer_arbiter "framebuffer_arbiter" v1.0
# Cory Barney 2016.01.27.01:12:54
# framebuffer arbiter for the DE1-SoC
# 

# 
# request TCL package from ACDS 15.0
# 
package require -exact qsys 15.0


# 
# module framebuffer_arbiter
# 
set_module_property DESCRIPTION "framebuffer arbiter for the DE1-SoC"
set_module_property NAME framebuffer_arbiter
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP Custom
set_module_property AUTHOR "Cory Barney"
set_module_property DISPLAY_NAME framebuffer_arbiter
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL framebuffer_arbiter
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file framebuffer_arbiter.sv SYSTEM_VERILOG PATH framebuffer_arbiter.sv TOP_LEVEL_FILE
add_fileset_file sdram_read_fifo.v VERILOG PATH sdram_read_fifo.v
add_fileset_file sdram_write_fifo.v VERILOG PATH sdram_write_fifo.v
add_fileset_file REG_n.v VERILOG PATH REG_n.v
add_fileset_file REG_n_26.v VERILOG PATH REG_n_26.v

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL framebuffer_arbiter
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file framebuffer_arbiter.sv SYSTEM_VERILOG PATH framebuffer_arbiter.sv
add_fileset_file sdram_read_fifo.v VERILOG PATH sdram_read_fifo.v
add_fileset_file sdram_write_fifo.v VERILOG PATH sdram_write_fifo.v
add_fileset_file REG_n.v VERILOG PATH REG_n.v
add_fileset_file REG_n_26.v VERILOG PATH REG_n_26.v


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point s0
# 
add_interface s0 avalon end
set_interface_property s0 addressUnits WORDS
set_interface_property s0 associatedClock clock
set_interface_property s0 associatedReset rst
set_interface_property s0 bitsPerSymbol 8
set_interface_property s0 burstOnBurstBoundariesOnly false
set_interface_property s0 burstcountUnits WORDS
set_interface_property s0 explicitAddressSpan 0
set_interface_property s0 holdTime 0
set_interface_property s0 linewrapBursts false
set_interface_property s0 maximumPendingReadTransactions 1
set_interface_property s0 maximumPendingWriteTransactions 0
set_interface_property s0 readLatency 0
set_interface_property s0 readWaitTime 1
set_interface_property s0 setupTime 0
set_interface_property s0 timingUnits Cycles
set_interface_property s0 writeWaitTime 0
set_interface_property s0 ENABLED true
set_interface_property s0 EXPORT_OF ""
set_interface_property s0 PORT_NAME_MAP ""
set_interface_property s0 CMSIS_SVD_VARIABLES ""
set_interface_property s0 SVD_ADDRESS_GROUP ""

add_interface_port s0 s0_address address Input 26
add_interface_port s0 s0_read read Input 1
add_interface_port s0 s0_write write Input 1
add_interface_port s0 s0_writedata writedata Input 16
add_interface_port s0 s0_readdata readdata Output 16
add_interface_port s0 s0_chipselect chipselect Input 1
add_interface_port s0 s0_byteenable byteenable Input 2
add_interface_port s0 s0_waitrequest waitrequest Output 1
add_interface_port s0 s0_readdatavalid readdatavalid Output 1
set_interface_assignment s0 embeddedsw.configuration.isFlash 0
set_interface_assignment s0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point avalon_master
# 
add_interface avalon_master avalon start
set_interface_property avalon_master addressUnits SYMBOLS
set_interface_property avalon_master associatedClock clock
set_interface_property avalon_master associatedReset rst
set_interface_property avalon_master bitsPerSymbol 8
set_interface_property avalon_master burstOnBurstBoundariesOnly false
set_interface_property avalon_master burstcountUnits WORDS
set_interface_property avalon_master doStreamReads false
set_interface_property avalon_master doStreamWrites false
set_interface_property avalon_master holdTime 0
set_interface_property avalon_master linewrapBursts false
set_interface_property avalon_master maximumPendingReadTransactions 0
set_interface_property avalon_master maximumPendingWriteTransactions 0
set_interface_property avalon_master readLatency 0
set_interface_property avalon_master readWaitTime 0
set_interface_property avalon_master setupTime 0
set_interface_property avalon_master timingUnits Cycles
set_interface_property avalon_master writeWaitTime 0
set_interface_property avalon_master ENABLED true
set_interface_property avalon_master EXPORT_OF ""
set_interface_property avalon_master PORT_NAME_MAP ""
set_interface_property avalon_master CMSIS_SVD_VARIABLES ""
set_interface_property avalon_master SVD_ADDRESS_GROUP ""

add_interface_port avalon_master m0_waitrequest waitrequest Input 1
add_interface_port avalon_master m0_readdatavalid readdatavalid Input 1
add_interface_port avalon_master m0_readdata readdata Input 16
add_interface_port avalon_master m0_read_n read_n Output 1
add_interface_port avalon_master m0_write_n write_n Output 1
add_interface_port avalon_master m0_writedata writedata Output 16
add_interface_port avalon_master m0_address address Output 26
add_interface_port avalon_master m0_byteenable byteenable Output 2
add_interface_port avalon_master m0_chipselect chipselect Output 1


# 
# connection point rst
# 
add_interface rst reset end
set_interface_property rst associatedClock clock
set_interface_property rst synchronousEdges DEASSERT
set_interface_property rst ENABLED true
set_interface_property rst EXPORT_OF ""
set_interface_property rst PORT_NAME_MAP ""
set_interface_property rst CMSIS_SVD_VARIABLES ""
set_interface_property rst SVD_ADDRESS_GROUP ""

add_interface_port rst rst reset Input 1


# 
# connection point s1
# 
add_interface s1 avalon end
set_interface_property s1 addressUnits WORDS
set_interface_property s1 associatedClock clock
set_interface_property s1 associatedReset rst
set_interface_property s1 bitsPerSymbol 8
set_interface_property s1 burstOnBurstBoundariesOnly false
set_interface_property s1 burstcountUnits WORDS
set_interface_property s1 explicitAddressSpan 0
set_interface_property s1 holdTime 0
set_interface_property s1 linewrapBursts false
set_interface_property s1 maximumPendingReadTransactions 0
set_interface_property s1 maximumPendingWriteTransactions 0
set_interface_property s1 readLatency 0
set_interface_property s1 readWaitTime 1
set_interface_property s1 setupTime 1
set_interface_property s1 timingUnits Cycles
set_interface_property s1 writeWaitStates 1
set_interface_property s1 writeWaitTime 1
set_interface_property s1 ENABLED true
set_interface_property s1 EXPORT_OF ""
set_interface_property s1 PORT_NAME_MAP ""
set_interface_property s1 CMSIS_SVD_VARIABLES ""
set_interface_property s1 SVD_ADDRESS_GROUP ""

add_interface_port s1 s1_address address Input 1
add_interface_port s1 s1_read read Input 1
add_interface_port s1 s1_write write Input 1
add_interface_port s1 s1_writedata writedata Input 32
add_interface_port s1 s1_readdata readdata Output 32
set_interface_assignment s1 embeddedsw.configuration.isFlash 0
set_interface_assignment s1 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s1 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s1 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point conduit_end
# 
add_interface conduit_end conduit end
set_interface_property conduit_end associatedClock clock
set_interface_property conduit_end associatedReset rst
set_interface_property conduit_end ENABLED true
set_interface_property conduit_end EXPORT_OF ""
set_interface_property conduit_end PORT_NAME_MAP ""
set_interface_property conduit_end CMSIS_SVD_VARIABLES ""
set_interface_property conduit_end SVD_ADDRESS_GROUP ""

