#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "drawLib.h"
#include "FAT16SD.h"
#include "i2cAvalon.h"
#include "TouchI2C.h"
#include "fontWriterSD.h"
#include "ClockI2C.h"
#include "calendar.h"
#include "altera_avalon_timer_regs.h"
#include "sys/alt_irq.h"
#include "priv/alt_legacy_irq.h"
#include "image.h"
#include "clock.h"
#include "DrawApplication.h"



void initDrivers();

volatile int timeNew = 0;
volatile int timeOld = 0;

void handle_timer_interrupt(void* isr_context);
void handle_i2c_interrupt(void* isr_context);

volatile uint8_t eatNextRead = 0;
void (*timerFunc)(void) = NULL;
void (*touchReleaseFunc)(uint16_t x, uint16_t y) = NULL;
void (*touchFivePointFunc)(void) = NULL;
void (*touchContinuousFunc)(uint16_t x, uint16_t y) = NULL;

int main()
{
	//int ID;
	printf("hello from main\n");
	initDrivers();
	//writeDate(0x4, 0x11, 0x05, 0x16);

	flipFrame(0);
	for(;;);

	return 0;
}

void initDrivers()
{
	IOWR(LCDFRAMEBUFFER_0_BASE, 1, 4);
	initTouchI2C();
	if (sdMount() != 0)
		while(1);
	if (initFontMap("NEWROMAN", "BMP") <= 0)
		while(1);
	initClock(0);
	initWhiteboard(3);
	initCal(5);
//	timerFunc = displayTimerClock;
//	touchReleaseFunc = pressEditButton;
	  // register the timer irq to be serviced by handle_timer_interrupt() function
	alt_irq_register(TIMER_0_IRQ, 0, handle_timer_interrupt);
	alt_irq_register(I2C_AVALON_0_IRQ, 0, handle_i2c_interrupt);


	IOWR(TIMER_0_BASE, 3, 20);		//Load Period
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 7);
	IOWR(TIMER_0_BASE, 1, 5 );	//Start Timer and Interrupt Enable
	IOWR(I2CADDR, 1, 3);	//Start Timer and Interrupt Enable
}

/* it is the duty of the timerFunc to start the timer again if desired */
void handle_timer_interrupt(void* isr_context)
{
	IOWR(TIMER_0_BASE, 0, 0 );	//Clear interrupt timer

	if (timerFunc != NULL)
		timerFunc();
}




/* only detects press on release*/
void handle_i2c_interrupt(void* isr_context)
{
	int i;
	uint32_t temp;
	uint16_t x,y;
	IOWR(I2C_AVALON_0_BASE, 1, 3 );	//Clear interrupt timer

	temp = readTouchData(0);


		//5 Touch Gesture
//		if ((readNumTouch()) > 4){
//			//invertDisplay();
//			if (touchFivePointFunc != NULL)
//				touchFivePointFunc();
//			while (readNumTouch() > 0)
//				for(i=0;i<5000;i++);
//		}
	//	//Release Press
	//	else if ((temp & 0xC0000000) == 0x40000000){
	//
	//		if (touchReleaseFunc != NULL){
	//			x = ((temp>>16) & 0x0FFF);
	//			y = (temp & 0x0FFF);
	//			touchReleaseFunc(x,y);
	//		}
	//
	//	}
		//Continued Press
//		else{

		do{
			x = ((temp>>16) & 0x0FFF);
			y = (temp & 0x0FFF);

			if (touchContinuousFunc != NULL){

				touchContinuousFunc(x,y);
			}

			temp = readTouchData(0);
			for(i=0;i<2000;i++);
			x = ((temp>>16) & 0x0FFF);
			y = (temp & 0x0FFF);
		}while ((readNumTouch()) > 0);
		touchReleaseFunc(x,y);


	for(i=0;i<50000;i++);
	IOWR(I2C_AVALON_0_BASE, 1, 3 );	//Clear interrupt timer
}








