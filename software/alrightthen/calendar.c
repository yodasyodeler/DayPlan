#include "calendar.h"

static uint8_t Cday, Cmonth, Cweekday;
static uint16_t Cyear;



uint8_t daysInMonth[2][13] = {
		{0,31,28,31,30,31,30,31,31,30,31,30,31},
		{0,31,29,31,30,31,30,31,31,30,31,30,31}
};

char* weekdays[7] = { "Sunday",
					  "Monday",
					  "Tuesday",
					  "Wednesday",
					  "Thursday",
					  "Friday",
					  "Saturday"
};

char* months[12] = { "January",
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

static uint32_t calFrame;
static int yearOffset=0, monthOffset=0;

static FontCursor cursor;
static uint32_t nextARRWID[2];

int initCal(uint32_t frame)
{
	calFrame = frame;
	drawFrame(GREY, calFrame);

	cursor.frame = calFrame;
	if ((cursor.ID = initFontMap("FONT    ", "BMP")) <= 0)
				while(1);
	cursor.scale = 0x11;
	cursor.x = 290;
	cursor.y = 20;
	cursor.color = BLACK;

	nextARRWID[0] = createImage("NEXTARW ", "BMP", NULL);
	setScaleImage(nextARRWID[0], 3, 3);
	setWindowImage(nextARRWID[0], 0, 0, 32, 240);
	moveImage(nextARRWID[0], 10, 120);
	setFrameImage(nextARRWID[0], calFrame);

	nextARRWID[1] = createImage("NEXTARW ", "BMP", NULL);
	setScaleImage(nextARRWID[1], 3, 3);
	setWindowImage(nextARRWID[1], 32, 0, 64, 240);
	moveImage(nextARRWID[1], 750, 120);
	setFrameImage(nextARRWID[1], calFrame);

	return 0;
}

/* isLeapYear
 *   leap year is divisible by 4 and not by 100
 *   Unless divisible by 400
 */
int isLeapYear(uint16_t year)
{
	return ( (!((year)%4) && ((year)%100)) || !((year)%400) );
}

/* dayInYear
 *   Returns the number of days into the year
 */
int dayInYear(uint8_t day, uint8_t month, uint16_t year)
{
	int i, leapYear;
	uint16_t re = day;
	leapYear = isLeapYear(year);

	for (i = 1; i < month; ++i)
		re += daysInMonth[leapYear][i];
	return re;
}

/* dayInWeek
 *   Returns Index for Day of the week
 *   Does not support pre-Leap year correction
 *   	year 1752 - 0
 *   	  1 - Sunday
 *   	  2 - Monday
 *   	  3 - Tuesday
 *   	  4 - Wednesday
 *   	  5 - Thursday
 *   	  6 - Friday
 *   	  7 - Saturday
 */
int dayInWeek(uint8_t day, uint8_t month, uint16_t year)
{
	int temp = (year - 1) * 365 + dayInYear(day, month, year) +
				((year-1)/4) - ((year-1)/100) + ((year-1)/400);

	return ((temp - 6) % 7) ? ((temp - 6) % 7) : 7;
}

/* getCurrentDate
 *   Returns nonBCD of current date
 *   Uses RTC values then converts them
 */
void getCurrentDate(uint8_t* weekday, uint8_t* day, uint8_t* month, uint16_t* year)
{
	uint8_t temp;
	readDate(weekday, day, month, &temp );
		*year = 0x2000 + temp;

		*year  = c_BCD(*year);
		*month = c_BCD(*month);
		*day   = c_BCD(*day);
}

/*  displayCal
 *    Displays a 6x7 grid that represents a given month and year.
 *    If the current date is within the month it will be highlighted
 *    with a green date. Displays a header for weekdays, month, and
 *    year.
 */

int displayCal(uint8_t month, uint16_t year, uint8_t frame)
{
	uint8_t weekday;
	uint8_t leapYear = isLeapYear(year);
	uint16_t x;
	uint16_t y;
	int i;
	uint8_t temp;
	uint16_t whiteColors[2] = { WHITE, OFFWHITE };
	uint16_t greyColors[2] = {LIGHTGREY, GREY };

	//FontCursor cursorOld;


	/* Check month is valid */
	if (month < 1 || month > 12)
		return -1;

	/* doesn't support old Calendar */
	if (year < 1753 || year > 9999)
		return -2;

	drawRectangleFill(GREY,290,20,600,36, calFrame);

	weekday = dayInWeek(1, month, year) - 1;

	/* Display Header */
	cursor.x = 290;
	cursor.y = 20;
	setCursor(cursor);
	printk("%s\t-\t%d", months[month-1], year);

//	//debug
//	refreshScreen();

	cursor.y = 60-16;
	cursor.x = 22;
	setCursor(cursor);
	for (i=0; i< 7; ++i)
	{
		printk("%s\t", weekdays[i]);
		cursor.x += 108;
		setCursor(cursor);
	}



	/*draw Calendar */
	y = 60;
	x = 22;
	cursor.y = 60+2;
	cursor.x = 22+2;
	cursor.color = BLACK;
	setCursor(cursor);
	//drawRectangleFill(WHITE, x, y, x+108, y+66, 0);

//	//debug
//	refreshScreen();

	/* display days before */
	temp = daysInMonth[leapYear][month-1];
	for (i=weekday; i > 0; --i){
		drawRectangleFill(greyColors[i&1], x, y, x+108, y+66, frame);
		printk("%d", (temp - i - 2) );
		x+=108;
		cursor.x += 108;
		setCursor(cursor);
	}

//	//debug
//	refreshScreen();

	/* display days after */
	temp = daysInMonth[leapYear][month];
	for (i=1; i <= (temp+7); ++i){
		if (month == Cmonth && year == Cyear && Cday == i)
			changeColorFont(GREEN);
		else
			changeColorFont(BLACK);
		if (i <= temp){
			drawRectangleFill(whiteColors[((i+weekday-1)%7)&1], x, y, x+108, y+66, frame);
			printk( "%d", i);
		}
		else{
			drawRectangleFill(greyColors[((i+weekday-1)%7)&1], x, y, x+108, y+66, frame);
			printk( "%d", i-temp);
		}

//		//debug
//		refreshScreen();

		x+=108;
		cursor.x += 108;

		if (((i+weekday)%7) == 0){
			cursor.y += 66;
			cursor.x = 22;
			y += 66;
			x = 22;
		}
		setCursor(cursor);
	}
	return 0;
}


// void (*timerFunc)(void)
//void touchFivePointFuncCal(void);
//void touchContinuousCal (uint16_t x, uint16_t y)
void touchReleaseCal(uint16_t x, uint16_t y)
{
	if (x<90 && y<360 && y>120){

		//change operation Whiteboard left
		swapToClock();

	}
	else if (y<60 ){
		getCurrentDate(&Cweekday, &Cday, &Cmonth, &Cyear );

		if ( ((--monthOffset)+Cmonth) == 0){
			monthOffset = 0;
			yearOffset -= 1;
		}

		displayCal(Cmonth+monthOffset, Cyear+yearOffset, calFrame);

		displayImage(nextARRWID[0]);
		displayImage(nextARRWID[1]);

		refreshScreen();
	}
	else if (x>710 && y<360 && y>120){
		//change operation Calendar right
		swapToWhiteboard();
	}
	else if (y>420){
		getCurrentDate(&Cweekday, &Cday, &Cmonth, &Cyear );

		if (((++monthOffset)+(Cmonth)) == 13){
			monthOffset = 0;
			yearOffset += 1;
		}

		displayCal(Cmonth+monthOffset, Cyear+yearOffset,calFrame);

		displayImage(nextARRWID[0]);
		displayImage(nextARRWID[1]);

		refreshScreen();
	}
}

void swapToCal(void)
{
	int i;
	flipFrame(calFrame);
	getCurrentDate(&Cweekday, &Cday, &Cmonth, &Cyear );
	displayCal(Cmonth+monthOffset, Cyear+yearOffset, calFrame);

	timerFunc = NULL;
	touchFivePointFunc = NULL;
	touchReleaseFunc = touchReleaseCal;
	touchContinuousFunc = NULL;
	for (i=0; i<5000;++i);

	displayImage(nextARRWID[0]);
	displayImage(nextARRWID[1]);

	refreshScreen();
}
