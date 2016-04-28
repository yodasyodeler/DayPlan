#include "clock.h"

static uint16_t cDigitColor = BLUE;
static uint16_t cBGColor = GREY;

const uint16_t winX[10] = {0,112,224,336,448, 0,112,224,336,448};
const uint16_t winY[10] = {0, 0,0,0,0,210,210,210,210,210};

uint32_t ID[4];
uint32_t IDedit[8];
uint32_t frameDisplay;

uint8_t seconds, minutes, hours;

ClockState cState = DISPLAYCLOCK;

int initClock(uint32_t frame)
{
	int i;
	int x[4] = {100, 220, 430, 550};

	drawFrame(cBGColor, 0);
	drawRectangleFill(RED, 10, 430, 80, 470, frame);

	setMonoColorImage(cDigitColor);

	/* Create Clock Images */
	frameDisplay = frame;
	for (i=0; i<4; ++i){
		ID[i] = createImage("NUMBERS ", "BMP", NULL);
		setScaleImage(ID[i], 3, 3);
		moveImage(ID[i], x[i], 100);
	}

	drawRectangleFill(BLUE, 363, 144, 397, 188, frameDisplay);
	drawRectangleFill(BLUE, 363, 232, 397, 276, frameDisplay);


	/* Create Clock Edit Images */
	for (i=0; i<4; ++i){
		IDedit[i] = createImage("ARRWBTN ", "BMP", NULL);
		setScaleImage(IDedit[i], 3, 3);
		moveImage(IDedit[i], x[i], 0);
	}

	for (i=4; i<8; ++i){
		IDedit[i] = createImage("ARRWBTN ", "BMP", NULL);
		setScaleImage(IDedit[i], 3, 3);
		moveImage(IDedit[i], x[i-4], 310);
	}

	return 0;
}


void displayTimerClock(void)
{

	if (cState == DISPLAYCLOCK){
		drawFrame(GREY, 0);
		drawRectangleFill(RED, 10, 430, 80, 470, frameDisplay);

		readTime(&seconds, &minutes, &hours);
		hours &= 0x1F;


		setWindowImage(ID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
		displayImage(ID[0]);

		setWindowImage(ID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
		displayImage(ID[1]);


		drawRectangleFill(BLUE, 363, 144, 397, 188, frameDisplay);
		drawRectangleFill(BLUE, 363, 232, 397, 276, frameDisplay);


		setWindowImage(ID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
		displayImage(ID[2]);

		setWindowImage(ID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
		displayImage(ID[3]);

	   refreshScreen();
	}

   //timerFunc = displayTimerClock;
}


void displayEditClock(void)
{
	int i;



	drawFrame(GREY, 0);
	drawRectangleFill(RED, 10, 430, 80, 470, frameDisplay);


	setWindowImage(ID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
	displayImage(ID[0]);

	setWindowImage(ID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
	displayImage(ID[1]);


	drawRectangleFill(BLUE, 363, 144, 397, 188, frameDisplay);
	drawRectangleFill(BLUE, 363, 232, 397, 276, frameDisplay);


	setWindowImage(ID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
	displayImage(ID[2]);

	setWindowImage(ID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
	displayImage(ID[3]);

	for (i=0; i<8; ++i){
		displayImage(IDedit[i]);
	}

   refreshScreen();
}

void pressEditButton(uint16_t x, uint16_t y)
{
	if (x>10 && y>430 && x<80 && y<470 ){
		cState = EDITCLOCK;
		displayEditClock();
		touchFunc = editTouchClock;
	}
}

void editTouchClock(uint16_t x, uint16_t y)
{
	uint8_t changemade = 0;

	if (y>0 && y < 128){

		if (x>100 && x<100+112){
			hours ^= 0x10;
			if (hours &0x10)
				hours &= (0x10 | ((hours&0xF)%3));
			changemade = 1;
		}
		else if (x>220 && x<220+112){
			if (hours &0x10)
				hours = (0x10 | (((hours&0xF)+1)%3));
			else
				hours =  (hours&0xF0) | (((hours&0xF)+1)%10);
			changemade = 1;
		}
		else if (x>430 && x<430+112){
			minutes = ((((minutes>>4)+1)%6)<<4) | (minutes&0xF);
			changemade = 1;
		}
		else if (x>550 && x<550+112){
			minutes = (minutes&0xF0) | (((minutes&0xF)+1)%10);
			changemade = 1;
		}
	}

	else if (y>310 && y < 310+128){

			if (x>100 && x<100+112){
				hours ^= 0x10;
				if (hours &0x10)
					hours &= (0x10 | ((hours&0xF)%3));
				changemade = 1;
			}
			else if (x>220 && x<220+112){
				if ((hours&0xF)-1 < 0)
					hours = (hours&0xF0) | (hours&0x10 ? 2:9);
				else
					hours =  (hours&0xF0) | (((hours&0xF)-1)%10);
				changemade = 1;
			}
			else if (x>430 && x<430+112){
				if ((minutes>>4)-1 < 0)
					minutes = (0x50 | (minutes&0xF));
				else
					minutes = ((((minutes>>4)-1))<<4) | (minutes&0xF);
				changemade = 1;
			}
			else if (x>550 && x<550+112){
				if ((minutes&0xF)-1 < 0)
					minutes = (minutes&0xF0) | 9;
				else
					minutes = (minutes&0xF0) | (((minutes&0xF)-1)%10);
				changemade = 1;
			}
		}

	if (changemade)
		displayEditClock();




	if (x>10 && y>430 && x<80 && y<470 ){
		cState = DISPLAYCLOCK;
		writeTime( seconds, minutes, hours);
		displayTimerClock();
		touchFunc = pressEditButton;
	}
}
