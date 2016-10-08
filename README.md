# DayPlan
Library for driving a 7" TFT touch screen and other peripherals.


DayPlan provides communication to SD card(FAT16), RTC, touch screen, i2c, and parellel interface of the tft screen. 

DayPlan uses a the NIOS II softprocessor to handle all applications. All peripherals communicate through custom system verilog modules 
connected to the NIOS II by the Qsys interconnect. 

## Draw Shapes

Drawing simple shapes can be done to diffrent parts of memory to create a double buffer effect. flipFrame changes what frame, or portion of memory, is being displayed to the screen.

```C
void drawCircle(uint16_t color, int x0, int y0, int radius, uint32_t frame);
void drawCircleFill(uint16_t color, int x0, int y0, int radius, uint32_t frame);
void drawRectangleFill(uint16_t color, int x0, int y0, int x1, int y1, uint32_t frame);

void flipFrame(uint32_t frame);
```

<img src="https://cloud.githubusercontent.com/assets/9373330/19215621/a8cdcbd2-8d57-11e6-817e-1accad653c20.png" width="500" height="350" />

## Font system

Dayplan's Font library uses a Font Bitmap to interpret strings to the screen. 

Below is an example of displaying the time of day in hex(RTC using BCD to store time) with a AM/PM cstring depending on the time of day. The 0x1F is used as the RTC stores other data in more signifacnt bits such as AM/PM.

```C
printk("Time: %x:%x:%x %s", 0x1F&hours[invert], minutes[invert], seconds[invert], AMPM[periodOfTime]);
```

<img src=https://cloud.githubusercontent.com/assets/9373330/19215622/a9f3aa9a-8d57-11e6-946f-e554adf29ea0.JPG width="500" height="350" />

printk (print kerned) is a custom function that is modeled after C's printf to display kerned text to the screen.   
printg (print grid) is an alternative that will maintian equal spacing for all characters.

Below are the supported escape variables '%':   

    '%': Display char '%'   
    
    'd': Display arg as a Decimal Int   
    
    'x': Display arg as a lowercase Hex int   
    
    'X': Display arg as a uppercase Hex int   
    
    'c': Display arg as a char   
    
    's': Display arg as a string  
    
## RTC Clock

ds3231 Real Time CLock provides accurate time and date.

Time and Date can be read using below functions to access the RTC through the GPIO.

```C
void readTime(uint8_t* seconds, uint8_t* minutes, uint8_t* hours);
void readDate(uint8_t* weekday, uint8_t* day, uint8_t* month, uint8_t* year);
```

Image below shows the Time being read at a fixed rate based on an interval timer. Then displays an appropriate image for the time.


<img src=https://cloud.githubusercontent.com/assets/9373330/19215620/a7980642-8d57-11e6-95ec-0353502a00b6.JPG width="500" height="350"/>

## SD card & Images

An SD card using the FAT16 file system can be read to display bitmap images. Bitmap images are loaded into a file system on board the FPGA lessening time needed to display the Image the second time around. This also makes unnessary to read an image more than twice even when displaying the same image twice. The Image system also provides the ability to apply transformations. Moving, fixed scaling(.25,.5,1,2,4,8) in either x or/and y direction, flipping the image in the x or y direction are all supported.

Below is the same image displayed 3 times at .25x, .5x, and 1x scale.

<img src="https://cloud.githubusercontent.com/assets/9373330/19215624/ab9b7ee0-8d57-11e6-8280-b513e17e44dd.JPG" width="500" height="350" />


## Dayplan tested peripherals   
    Real Time Clock (ds3231)   
   
    7inch TFT Touch Screen   
   
    Display (SSD1963)   
   
    Touch (FT5206)   
   
    MicroSDCard   
   
Terasic's de1-SoC board was used for testing libraries.
