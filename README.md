# DayPlan
Library for driving a 7" TFT touch screen and other peripherals.


DayPlan provides communication to SD card(FAT16), RTC, touch screen, i2c, and parellel interface of the tft screen. 

DayPlan uses a the NIOS II softprocessor to handle all applications. All peripherals communicate through custom system verilog modules 
connected to the NIOS II by the Qsys interconnect. 


## Font system
Dayplan's Font library uses a Font Bitmap to interpret strings to the screen. 
```C
printk("Time: %x:%x:%x %s", 0x1F&hours[invert], minutes[invert], seconds[invert], AMPM[periodOfTime]);
```
<img srcf=https://cloud.githubusercontent.com/assets/9373330/19215622/a9f3aa9a-8d57-11e6-946f-e554adf29ea0.JPG width="500" height="350" />

printk (print kerned) is a custom function that is modeled after C's printf to display kerned text to the screen.   
printg (print grid) is an alternative that will maintian equal spacing for all characters.

Below are the supported escape variables '%':   

    '%': Display char '%'   
    
    'd': Display arg as a Decimal Int   
    
    'x': Display arg as a lowercase Hex int   
    
    'X': Display arg as a uppercase Hex int   
    
    'c': Display arg as a char   
    
    's': Display arg as a string  

## Dayplan tested peripherals   
    Real Time Clock (ds3231)   
   
    7inch TFT Touch Screen (http://www.buydisplay.com/default/7-tft-screen-touch-lcd-display-module-w-ssd1963-controller-board-mcu)   
   
    Display (SSD1963)   
   
    Touch (FT5206)   
   
    MicroSDCard   
   
Terasic's de1-SoC board was used for testing libraries.
