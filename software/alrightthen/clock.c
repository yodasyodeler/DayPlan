#include "clock.h"

static uint16_t cDigitColor = BLUE;
static uint16_t cBGColor = GREY;
static FontCursor cFontCursor;

const uint16_t winX[10] = {0,112,224,336,448, 0,112,224,336,448};
const uint16_t winY[10] = {0, 0,0,0,0,210,210,210,210,210};

static const char* weekdays[8] = { "",
					  "Sunday",
					  "Monday",
					  "Tuesday",
					  "Wednesday",
					  "Thursday",
					  "Friday",
					  "Saturday"
};

static const char* months[13] = { "",
					 "January",
					 "February",
					 "March",
					 "April",
					 "May",
					 "June",
					 "July",
					 "August",
					 "September",
					 "October",
					 "November",
					 "December"
};

uint32_t ID[4];
uint32_t IDedit[8];
uint32_t frameDisplay;

uint8_t seconds, minutes, hours;
static uint8_t Cday, Cmonth, Cweekday;
static uint8_t Cyear;

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

	if ((cFontCursor.ID = initFontMap("NEWROMAN", "BMP")) <= 0)
			while(1);

	cFontCursor.x = x[0];
	cFontCursor.y = 300;
	cFontCursor.color = cDigitColor;
	cFontCursor.scale = 0x11;
	cFontCursor.frame = 0;

	return 0;
}

void displayTimerClock(void)
{
	int i;
	int x[4] = {100, 220, 430, 550};
	FontCursor oldCursor;
	FontCursor newCursor = cFontCursor;


	if (cState == DISPLAYCLOCK){
		//drawFrame(GREY, 0);
		for (i=0; i<4; ++i){
			drawRectangleFill( GREY, x[i], 100, x[i]+112, 310, 0);
		}

		readTime(&seconds, &minutes, &hours);
		hours &= 0x1F;


		setWindowImage(ID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
		displayImage(ID[0]);

		setWindowImage(ID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
		displayImage(ID[1]);

		setWindowImage(ID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
		displayImage(ID[2]);

		setWindowImage(ID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
		displayImage(ID[3]);

		readDate(&Cweekday, &Cday, &Cmonth, &Cyear);

		oldCursor = getCursor();
		setCursor(newCursor);
		printk("Month\t%s", months[c_BCD(Cmonth)]);

		newCursor = cFontCursor;
		newCursor.y += 64;

		setCursor(newCursor);
		printk("Day\t%x, %s", Cday, (weekdays[c_BCD(Cweekday)]));

		setCursor(oldCursor);

	   refreshScreen();
	   IOWR(TIMER_0_BASE, 1, 5 );	//Start Timer and Interrupt Enable
	}

   //timerFunc = displayTimerClock;
}

void displayEditClock(uint8_t in)
{
	int x[4] = {100, 220, 430, 550};

	drawRectangleFill( GREY, x[in], 100, x[in]+112, 310, 0);

	switch (in)
	{
		case 0:
			setWindowImage(ID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
			displayImage(ID[0]);
			drawRectangleFill( GREY, x[in+1], 100, x[in+1]+112, 310, 0);
			displayImage(IDedit[in+1]);
			displayImage(IDedit[in+5]);
			/* no break */
		case 1:
			setWindowImage(ID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
			displayImage(ID[1]);
			break;

		case 2:
			setWindowImage(ID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
			displayImage(ID[2]);
			break;
		case 3:
			setWindowImage(ID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
			displayImage(ID[3]);
			break;
	}

	displayImage(IDedit[in]);
	displayImage(IDedit[in+4]);

   refreshScreen();
}

void pressEditButton(uint16_t x, uint16_t y)
{
	int i;
	if (y>420 && x<90 ){
		cState = EDITCLOCK;
		for (i=0; i<8; ++i)
			displayImage(IDedit[i]);
		touchFunc = editTouchClock;
		refreshScreen();
	}
}

void editTouchClock(uint16_t x, uint16_t y)
{
uint8_t changemade = 0;

	if (y>0 && y < 128){

		if (x<100+112 && x>100){
			hours ^= 0x10;
			if (hours &0x10)
				hours &= (0x10 | ((hours&0xF)%3));
			changemade = 1;
		}
		else if (x<220+112 && x>220){
			if (hours &0x10)
				hours = (0x10 | (((hours&0xF)+1)%3));
			else
				hours =  (hours&0xF0) | ((((hours&0xF))%9)+1);
			changemade = 2;
		}
		else if (x>430 && x<430+112){
			minutes = ((((minutes>>4)+1)%6)<<4) | (minutes&0xF);
			changemade = 3;
		}
		else if (x>550 && x<550+112){
			minutes = (minutes&0xF0) | (((minutes&0xF)+1)%10);
			changemade = 4;
		}
	}

	else if (y>310 && y < 310+128){
			if (x>100 && x<100+112){
				hours ^= 0x10;
				if (hours & 0x10)
					hours &= (0x10 | ((hours&0xF)%3));
				changemade = 1;
			}
			else if (x>220 && x<220+112){
				if ((hours&0xF)-1 < 0)
					hours = (hours&0xF0) | (hours&0x10 ? 2:9);
				else
					hours =  (hours&0xF0) | ((hours&0xF)-1);
				changemade = 2;
			}
			else if (x>430 && x<430+112){
				if ((minutes>>4)-1 < 0)
					minutes = (0x50 | (minutes&0xF));
				else
					minutes = ((((minutes>>4)-1))<<4) | (minutes&0xF);
				changemade = 3;
			}
			else if (x>550 && x<550+112){
				if ((minutes&0xF)-1 < 0)
					minutes = (minutes&0xF0) | 9;
				else
					minutes = (minutes&0xF0) | (((minutes&0xF)-1)%10);
				changemade = 4;
		}
	}

	if ((hours&0x1F) == 0)
		hours = 0x09;
	if (changemade)
		displayEditClock(changemade-1);




	if (y>420 && x<90 ){
		cState = DISPLAYCLOCK;
		writeTime( seconds, minutes, hours);
		drawFrame(cBGColor, frameDisplay);
		drawRectangleFill(BLUE, 363, 144, 397, 188, frameDisplay);
		drawRectangleFill(BLUE, 363, 232, 397, 276, frameDisplay);
		drawRectangleFill(RED, 10, 430, 80, 470, frameDisplay);

		displayTimerClock();
		touchFunc = pressEditButton;
		IOWR(TIMER_0_BASE, 1, 5 );	//Start Timer and Interrupt Enable
	}
}
