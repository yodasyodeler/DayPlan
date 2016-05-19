#include "clock.h"

static uint16_t cDigitColor = BLUE;
static uint16_t cBGColor = GREY;
static FontCursor cClockCursor;
static FontCursor cDateCursor;
static uint8_t invertInterface = 1;

const uint16_t winX[10] = {0,112,224,336,448, 0,112,224,336,448};
const uint16_t winY[10] = {0, 0,0,0,0,210,210,210,210,210};

static uint32_t clockID[4];
static uint32_t nextARRWID[2];

static uint32_t editClockID[4];
static uint32_t editARRWID[8];

static uint32_t dateARRWID[2];
static uint32_t displayFrame;

static uint8_t seconds, minutes, hours;
static uint8_t Cday, Cmonth, Cweekday, Cyear;
static uint16_t Cyearfull;

ClockState cState = DISPLAYCLOCK;

void displayEditDate(uint8_t show);

//Display Functions
void displayBackground(uint8_t frame)
{
	drawFrame(cBGColor, displayFrame+frame);
	//display Colons
	drawRectangleFill(cDigitColor, 363, 144, 397, 188, displayFrame+frame);
	drawRectangleFill(cDigitColor, 363, 232, 397, 276, displayFrame+frame);
}
void displayInterface(uint8_t show)
{
	if (show){
		drawRectangleFill(RED, 10, 430, 80, 470, displayFrame);
		drawRectangleFill(GREEN, 710, 430, 780, 470, displayFrame);
		displayImage(nextARRWID[0]);
		displayImage(nextARRWID[1]);
	}
	else{
		drawRectangleFill(cBGColor, 10, 430, 80, 470, displayFrame);
		drawRectangleFill(cBGColor, 710, 430, 780, 470, displayFrame);
		drawRectangleFill(cBGColor, 10, 120, 50, 360, displayFrame);
		drawRectangleFill(cBGColor, 750, 120, 790, 360, displayFrame);
	}

}
void displayDate(uint32_t show)
{
	if (show){
		FontCursor oldCursor;
		FontCursor newCursor = cClockCursor;

		readDate(&Cweekday, &Cday, &Cmonth, &Cyear);

		oldCursor = getCursor();
		setCursor(newCursor);
		printk("%s\t%x", months[c_BCD(Cmonth-1)], 0x2000+Cyear);

		newCursor = cClockCursor;
		newCursor.y += 64;

		setCursor(newCursor);
		printk("%x,\t%s", Cday, weekdays[c_BCD(Cweekday-1)]);

		setCursor(oldCursor);
	}
	else
		drawRectangleFill(cBGColor, cClockCursor.x,cClockCursor.y, cClockCursor.x+480, cClockCursor.y+128, displayFrame);

}

void invertDisplay(void)
{
	if (cState == DISPLAYCLOCK)
	{
		invertInterface^=1;
		displayInterface(invertInterface);
	}
}


//init
int initClock(uint32_t frame)
{
	int i;
	int x[4] = {100, 220, 430, 550};
	displayFrame = frame;

	setMonoColorImage(GREEN);
	flipFrame(displayFrame);
	/*   For general   */
	displayBackground(displayFrame);
	for (i=0; i<4; ++i){
		clockID[i] = createImage("NUMBERS ", "BMP", NULL);
		setScaleImage(clockID[i], 3, 3);
		moveImage(clockID[i], x[i], 100);
		setFrameImage(clockID[i], displayFrame);
	}


	nextARRWID[0] = createImage("NEXTARW ", "BMP", NULL);
	setScaleImage(nextARRWID[0], 3, 3);
	setWindowImage(nextARRWID[0], 0, 0, 32, 240);
	moveImage(nextARRWID[0], 10, 120);
	setFrameImage(nextARRWID[0], displayFrame);

	nextARRWID[1] = createImage("NEXTARW ", "BMP", NULL);
	setScaleImage(nextARRWID[1], 3, 3);
	setWindowImage(nextARRWID[1], 32, 0, 64, 240);
	moveImage(nextARRWID[1], 750, 120);
	setFrameImage(nextARRWID[1], displayFrame);

	displayInterface(1);

	/* init font and cursor */
	if ((cClockCursor.ID = initFontMap("NEWROMAN", "BMP")) <= 0)
			while(1);

	cClockCursor.x = x[1];
	cClockCursor.y = 310;
	cClockCursor.color = cDigitColor;
	cClockCursor.scale = 0x11;
	cClockCursor.frame = displayFrame;

	displayDate(1);


	/*   For editing time   */
	displayBackground(displayFrame+1);
	for (i=0; i<4; ++i){
		editClockID[i] = createImage("NUMBERS ", "BMP", NULL);
		setScaleImage(editClockID[i], 3, 3);
		moveImage(editClockID[i], x[i], 100);
		setFrameImage(editClockID[i], displayFrame+1);
	}
	for (i=0; i<4; ++i){
		editARRWID[i] = createImage("ARRWBTN ", "BMP", NULL);
		setScaleImage(editARRWID[i], 3, 3);
		setWindowImage(editARRWID[i], 0, 0, 112, 100);
		moveImage(editARRWID[i], x[i], 0);
		setFrameImage(editARRWID[i], displayFrame+1);
		displayImage(editARRWID[i]);
	}
	for (i=4; i<8; ++i){
		editARRWID[i] = createImage("ARRWBTN ", "BMP", NULL);
		setScaleImage(editARRWID[i], 3, 3);
		setWindowImage(editARRWID[i], 112, 0, 224, 100);
		moveImage(editARRWID[i], x[i-4], 310);
		setFrameImage(editARRWID[i], displayFrame+1);
		displayImage(editARRWID[i]);
	}

	drawRectangleFill(RED, 10, 430, 80, 470, displayFrame+1);
	drawRectangleFill(GREEN, 710, 430, 780, 470, displayFrame+1);

	/*   For date editing   */
	drawFrame(cBGColor, displayFrame+2);
	cDateCursor.x = x[1];
	cDateCursor.y = 100;
	cDateCursor.color = cDigitColor;
	cDateCursor.scale = 0x11;
	cDateCursor.frame = displayFrame+2;
	cDateCursor.ID = cClockCursor.ID;

	drawRectangleFill(RED, 10, 430, 80, 470, displayFrame+2);
	drawRectangleFill(GREEN, 710, 430, 780, 470, displayFrame+2);

	dateARRWID[0] = createImage("ARRWBTN ", "BMP", NULL);
	setScaleImage(dateARRWID[0], 3, 3);
	setWindowImage(dateARRWID[0], 224, 0, 324, 100);
	moveImage(dateARRWID[0], x[0], 100);
	setFrameImage(dateARRWID[0], displayFrame+2);
	displayImage(dateARRWID[0]);

	dateARRWID[1] = createImage("ARRWBTN ", "BMP", NULL);
	setScaleImage(dateARRWID[1], 3, 3);
	setWindowImage(dateARRWID[1], 324, 0, 424, 100);
	moveImage(dateARRWID[1], x[3], 100);
	setFrameImage(dateARRWID[1], displayFrame+2);
	displayImage(dateARRWID[1]);

	drawRectangleFill(RED, 10, 430, 80, 470, displayFrame+2);
	drawRectangleFill(GREEN, 710, 430, 780, 470, displayFrame+2);
//	moveCursorFont(10, 430,3);
//	printk("Clock");
//	moveCursorFont(710, 430, 3);
//	printk("Date");

	cState = DISPLAYCLOCK;

	touchFivePointFunc = invertDisplay;
	touchReleaseFunc = pressEditButton;
	touchContinuousFunc = NULL;

	timerFunc = displayTimerClock;

	return 0;
}

//Normal operations
void displayTimerClock(void)
{
	int i;
	int x[4] = {100, 220, 430, 550};

	if (cState == DISPLAYCLOCK){
		for (i=0; i<4; ++i){
			drawRectangleFill( GREY, x[i], 100, x[i]+112, 310, displayFrame);
		}

		readTime(&seconds, &minutes, &hours);
		hours &= 0x1F;

		setWindowImage(clockID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
		displayImage(clockID[0]);

		setWindowImage(clockID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
		displayImage(clockID[1]);

		setWindowImage(clockID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
		displayImage(clockID[2]);

		setWindowImage(clockID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
		displayImage(clockID[3]);

		if (minutes==0){
			displayDate(0);
			displayDate(1);
		}

	   refreshScreen();
	   IOWR(TIMER_0_BASE, 1, 5 );	//Start Timer and Interrupt Enable
	}
	else if (cState == EDITCLOCK)
	{
		for (i=0; i<4; ++i){
					drawRectangleFill( GREY, x[i], 100, x[i]+112, 310, displayFrame+1);
				}

				readTime(&seconds, &minutes, &hours);
				hours &= 0x1F;

				setWindowImage(editClockID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
				displayImage(editClockID[0]);

				setWindowImage(editClockID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
				displayImage(editClockID[1]);

				setWindowImage(editClockID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
				displayImage(editClockID[2]);

				setWindowImage(editClockID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
				displayImage(editClockID[3]);

				if (minutes==0){
					displayDate(0);
					displayDate(1);
				}

			   refreshScreen();
	}

   //timerFunc = displayTimerClock;
}
void pressEditButton(uint16_t x, uint16_t y)
{

	if (invertInterface==1){
		if (x<90){
			if (y<360 && y>120){

				//change operation Whiteboard left
				swapToWhiteboard();

			}
			else if (y>420 ){
				cState = EDITCLOCK;
				flipFrame(displayFrame+1);
				touchReleaseFunc = editTouchClock;
				refreshScreen();
			}
		}
		else if (x>710){
			if (y<360 && y>120){
				//change operation Calendar right
				swapToCal();
			}
			else if (y>420){
				cState = EDITDATE;
				getCurrentDate(&Cweekday, &Cday, &Cmonth, &Cyearfull);
				Cweekday--;
				displayEditDate(0);
				displayEditDate(1);
				flipFrame(displayFrame+2);
				touchReleaseFunc = editTouchDate;
				refreshScreen();
			}
		}
	}
}


//Edit time
void displayEditClock(uint8_t in, uint8_t show)
{
	int x[4] = {100, 220, 430, 550};

	drawRectangleFill( GREY, x[in], 100, x[in]+112, 310, displayFrame+1);

	if (show){
		switch (in)
		{
			case 0:
				setWindowImage(editClockID[0], winX[hours>>4], winY[hours>>4], winX[hours>>4]+112, winY[hours>>4]+210);
				displayImage(editClockID[0]);
				drawRectangleFill( GREY, x[in+1], 100, x[in+1]+112, 310, displayFrame+1);
				/* no break */
			case 1:
				setWindowImage(editClockID[1], winX[hours&0xF], winY[hours&0xF], winX[hours&0xF]+112, winY[hours&0xF]+210);
				displayImage(editClockID[1]);
				break;

			case 2:
				setWindowImage(editClockID[2], winX[minutes>>4], winY[minutes>>4], winX[minutes>>4]+112, winY[minutes>>4]+210);
				displayImage(editClockID[2]);
				break;
			case 3:
				setWindowImage(editClockID[3], winX[minutes&0xF], winY[minutes&0xF], winX[minutes&0xF]+112, winY[minutes&0xF]+210);
				displayImage(editClockID[3]);
				break;
		}
	}
	else{
		drawRectangleFill( GREY, x[0], 100, x[4]+112, 310, displayFrame+1);
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
	if (changemade){
		displayEditClock(changemade-1, 1);
		refreshScreen();
	}




	if (y>420 && x<90 ){
		cState = DISPLAYCLOCK;
		writeTime( seconds, minutes, hours);
		flipFrame(displayFrame);
		touchReleaseFunc = pressEditButton;
		//for(i=40000; i>0; --i);

		IOWR(TIMER_0_BASE, 1, 5 );	//Start Timer and Interrupt Enable
	}
	else if (y>420 && x>710){
		cState = EDITDATE;
		writeTime( seconds, minutes, hours);
		getCurrentDate(&Cweekday, &Cday, &Cmonth, &Cyearfull);
		Cweekday--;
		displayEditDate(0);
		displayEditDate(1);
		flipFrame(displayFrame+2);
		touchReleaseFunc = editTouchDate;
		refreshScreen();
	}
}



//Edit Date
void displayEditDate( uint8_t show)
{
	if (show){
			FontCursor oldCursor;
			FontCursor newCursor = cDateCursor;

			oldCursor = getCursor();
			setCursor(newCursor);
			printk("%s\t%d", months[Cmonth-1], Cyearfull);

			newCursor = cDateCursor;
			newCursor.y += 64;

			setCursor(newCursor);
			printk("%d, %s", Cday, (weekdays[Cweekday]));

			setCursor(oldCursor);
		}
		else
			drawRectangleFill(cBGColor, cDateCursor.x,cDateCursor.y, cDateCursor.x+335, cDateCursor.y+128, displayFrame+2);
}
void editTouchDate(uint16_t x, uint16_t y)
{
	int i;
	for(i=30000; i>0; --i);

	//left Arrow
	if (y>100 && y<200){
		if (x>550&&x<672){
			if(++Cday > daysInMonth[isLeapYear(Cyearfull)][Cmonth])
			{
				Cday = 1;

				if (++Cmonth > 12)
				{
					Cmonth = 0;
					++Cyearfull;
				}
			}
			Cweekday = ((++Cweekday) % 7);
			displayEditDate(0);
			displayEditDate(1);
			refreshScreen();
		}
		else if (x>100&&x<212){
			Cweekday = (Cweekday > 0 ? Cweekday-1: 6);
			if(--Cday <= 0 && Cyearfull>1800)
			{
				if (--Cmonth <= 0)
				{
					Cmonth = 12;
					--Cyearfull;
				}

				Cday = daysInMonth[isLeapYear(Cyearfull)][Cmonth];
			}
			displayEditDate(0);
			displayEditDate(1);
			refreshScreen();
		}
	}

	if (y>420 && x<90 ){
		cState = EDITCLOCK;
		++Cweekday;
		writeDate(Cweekday, c_toBCD(Cday), c_toBCD(Cmonth),c_toBCD(Cyearfull)&0xFF);
		flipFrame(displayFrame+1);
		displayDate(0);
		displayDate(1);
		displayTimerClock();
		touchReleaseFunc = editTouchClock;
	}
	else if (y>420 && x>710){
		cState = DISPLAYCLOCK;
		++Cweekday;
		writeDate(Cweekday, c_toBCD(Cday), c_toBCD(Cmonth), c_toBCD(Cyearfull)&0xFF);
		flipFrame(displayFrame);
		displayDate(0);
		displayDate(1);
		touchReleaseFunc = pressEditButton;
		IOWR(TIMER_0_BASE, 1, 5 );	//Start Timer and Interrupt Enable
	}
}




void swapToClock(void)
{
	timerFunc = displayTimerClock;
	touchFivePointFunc = invertDisplay;
	touchReleaseFunc = pressEditButton;
	touchContinuousFunc = NULL;
	flipFrame(displayFrame);
	refreshScreen();
}















