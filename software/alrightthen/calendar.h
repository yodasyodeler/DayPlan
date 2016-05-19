#ifndef CALENDAR_H_
#define CALENDAR_H_

#include "drawLib.h"
#include "fontWriterSD.h"
#include "ClockI2C.h"
#include "common.h"

extern uint8_t daysInMonth[2][13];
extern char* weekdays[7];
extern char* months[12];

int isLeapYear(uint16_t year);

int initCal(uint32_t frame);
void getCurrentDate(uint8_t* weekday, uint8_t* day, uint8_t* month, uint16_t* year);
int displayCal(uint8_t month, uint16_t year, uint8_t frame);

#endif
