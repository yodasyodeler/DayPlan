#ifndef CALENDAR_H_
#define CALENDAR_H_

#include "drawLib.h"
#include "fontWriterSPI.h"
#include "ClockI2C.h"
#include "common.h"

int displayCal(uint8_t month, uint16_t year, uint8_t frame);

#endif