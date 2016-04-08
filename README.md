# DayPlan
Project written in System Verilog and C to drive a Clock, Calendar, and Drawing applications to a TFT touch screen.

The purpose of DayPlan is to manage one's life through a digital screen. This stationary device will use input through 
the touch screen to create events, take notes, and set reminders. DayPlan will act as a robust calendar for an individual 
or family.

DayPlan uses a the NIOS II softprocessor to handle all applications. All peripherals interface through system verilog and 
then connect to the NIOS II using the Qsys interconnect. 

Dayplans peripherals include:

  -Real Time Clock (ds3231)
  
  -7inch TFT Touch Screen (http://www.buydisplay.com/default/7-tft-screen-touch-lcd-display-module-w-ssd1963-controller-board-mcu)
  
    -Display (SSD1963)
    
    -Touch (FT5206)
    
    -MicroSDCard
    

The system uses the de1-SoC on board sdram and a 2GB FAT16 micro SDcard for memory.

DayPlan is in development for my Senior Computer Engineering Project.
