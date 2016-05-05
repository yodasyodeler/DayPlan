#ifndef CLOCK_H_
#define CLOCK_H_

#include "common.h"
#include "ClockI2C.h"
#include "FAT16SD.h"
#include "drawLib.h"
#include "image.h"
#include "TouchI2C.h"
#include "fontWriterSD.h"

typedef enum _ClockState{
	DISPLAYCLOCK=0,
	EDITCLOCK

}ClockState;

//char fileName[8], char fileExt[3]
int initClock(uint32_t frame);
void displayTimerClock(void);
void displayEditClock(uint8_t in);

void pressEditButton(uint16_t x, uint16_t y);
void editTouchClock(uint16_t x, uint16_t y);



#endif /* CLOCK_H_ */
