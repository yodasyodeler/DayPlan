#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "drawLib.h"
#include "FAT16SD.h"
#include "i2cAvalon.h"
#include "TouchI2C.h"
#include "fontWriterSPI.h"
#include "ClockI2C.h"
#include "calendar.h"
#include "altera_avalon_timer_regs.h"
#include "sys/alt_irq.h"

void initDrivers();
//void displayTouch();

volatile int timeNew = 0;
volatile int timeOld = 0;

void handle_timer_interrupt(void*p, alt_u32 param);

int main()
{
	int temp = 0;
	printf("hello from main\n");
	temp = IORD(TIMER_0_BASE, 2);		//Load Period 500ms
	initDrivers();


	temp = IORD(TIMER_0_BASE, 2);		//Load Period 500ms

	for(;;)
		temp = 0;

	return 0;
}

void initDrivers()
{
	IOWR(LCDFRAMEBUFFER_0_BASE, 1, 3);
	initTouchI2C();
	if (sdMount() != 0)
		while(1);
	if (initFontMap("FONT    ", "BMP") != 0)
		while(1);
	  // register the timer irq to be serviced by handle_timer_interrupt() function
	alt_irq_register(TIMER_0_IRQ, 0, handle_timer_interrupt);

	IOWR(TIMER_0_BASE, 2, 500000);		//Load Period 500ms
	IOWR(TIMER_0_BASE, 1, (0x7) );	//Start Timer and Interrupt Enable
}

void handle_timer_interrupt(void*p, alt_u32 param)
{
	IOWR(TIMER_0_BASE, 0, 0 );	//Clear interrupt timer

   // your isr code here
   timeOld = timeNew;
   ++timeNew;
}

//void displayTouch()
//{
//	int hold;
//	uint16_t xCord[2] = {0,1};
//	uint16_t yCord[2] = {0,1};
//	uint8_t numTouch[2] = {0,2};
//	uint8_t invert = 0;
//	uint8_t seconds[2], minutes[2], hours[2];
//	uint8_t weekday[2], day[2], month[2], year[2];
//	uint8_t periodOfTime;
//	char* AMPM[2] = {"AM", "PM"};
//
//	drawFrame(GREY, 0);
//	flipFrame(0);
//
//	changeColorFont(GREEN);
//
//
//	printk("Cory Barney\t\t\tTouch Report  &  Time \t\t\t   CST 451");
//
//
//	while(1){
//		readTime(&seconds[invert], &minutes[invert], &hours[invert]);
//		if (seconds[0]!=seconds[1]){
//			drawRectangleFill(GREY,240,250,350, 266,0);
//			moveCursorFont(200,250,0);
//			periodOfTime = ((*hours & PM) > 0);
//			printk("Time: %x:%x:%x %s", 0x1F&hours[invert], minutes[invert], seconds[invert], AMPM[periodOfTime]);
//		}
//
//		if (hours[0] != hours[1]){
//			readDate(&weekday[invert], &day[invert], &month[invert], &year[invert]);
//			if (day[0]!=day[1]){
//				drawRectangleFill(GREY,240,266,300, 282,0);
//				drawRectangleFill(GREY,240,282,350, 298,0);
//				moveCursorFont(200,266,0);
//				printk("Date: %x/%x/%x", month[invert], day[invert], year[invert]);
//				moveCursorFont(200,282,0);
//				printk("Weekday: %s", weekdays[weekday[invert]-1]);
//			}
//		}
//
//		numTouch[invert] = readNumTouch();
//		if (numTouch[0] != numTouch[1]){
//			drawRectangleFill(GREY,350,120,382, 136,0);
//			moveCursorFont(200,120,0);
//			printk("Number of Touches: %d", numTouch[invert]);
//		}
//
//		readTouchCord(0, &xCord[invert], &yCord[invert]);
//		if (xCord[0]!=xCord[1] || yCord[0]!=yCord[1]){
//			drawRectangleFill(GREY,300,150,340, 182,0);
//			moveCursorFont(200,150,0);
//			printk("X coordinate: %d", xCord[invert]);
//			moveCursorFont(200,166,0);
//			printk("Y coordinate: %d", yCord[invert]);
//		}
//
//		invert ^= 1;
//		while( hold = isBusy()){}
//
//		usleep(2000);
//		refreshScreen();
//
//	}
//}


