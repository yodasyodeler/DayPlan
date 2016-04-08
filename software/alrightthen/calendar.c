#include "calendar.h"

static uint8_t Cday, Cmonth, Cweekday;
static uint16_t Cyear;

static uint8_t daysInMonth[2][13] = {
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

	leapYear = isLeapYear(year);

	for (i = 1; i < month; ++i)
		day += daysInMonth[leapYear][i];
	return day;
}

/* dayInWeek
 *   Returns Index for Day of the week
 *   Does not support pre-Leap year correction
 *   	pre 1752
 */
int dayInWeek(uint8_t day, uint8_t month, uint16_t year)
{
	int temp = (year - 1) * 365 + dayInYear(day, month, year) +
				((year-1)/4) - (((year-1)/100)-17) + (((year-1)-1600)/400);

	return ((temp - 6) % 7)+1;
}


int displayCal(uint8_t month, uint16_t year, uint8_t frame)
{
	uint8_t weekday;
	uint8_t leapYear = isLeapYear(year);
	int i;
	uint8_t temp;
	FontCursor cursorOld;
	FontCursor cursorNew = {200, 50, frame, WHITE};

	if (month < 1 || month > 12)
		return -1;

	/* doesn't support old Calendar */
	if (year < 1753 || year > 9999)
		return -2;

	readDate(&Cweekday, &Cday, &Cmonth, &temp );
	Cyear = 0x2000 + temp;

	Cyear = convertBCD(Cyear);
	Cmonth = convertBCD(Cmonth);
	Cday = convertBCD(Cday);


	weekday = dayInWeek(1, month, year);

	cursorOld = getCursor();

	setCursor(cursorNew);
	printg(" %s ", months[month-1]);
	IOWR(LCDFRAMEBUFFER_0_BASE, 0, 0);

	cursorNew.y += 16;




	setCursor(cursorNew);
	changeColorFont(GREY);

	temp = daysInMonth[leapYear][month-1];

	//display days before
	if (weekday != 1){
		for (i=weekday-2; i >= 0; --i){
			printg(" %d ", (temp - i) );

			IOWR(LCDFRAMEBUFFER_0_BASE, 0, 0);
		}
	}


	changeColorFont(WHITE);
	temp = daysInMonth[leapYear][month];

	//display days after
	for (i=1; i <= temp; ++i){
		if (month == Cmonth && year == Cyear && Cday == i)
			changeColorFont(GREEN);
		else
			changeColorFont(WHITE);

		printg(" %d ", i);
		IOWR(LCDFRAMEBUFFER_0_BASE, 0, 0);
		if (((weekday+i-1) % 7) == 0){
			cursorNew.y += 16;
			setCursor(cursorNew);
		}
	}

	setCursor(cursorOld);
	return 0;
}
