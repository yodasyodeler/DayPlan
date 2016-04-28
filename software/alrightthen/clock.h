#ifndef CLOCK_H_
#define CLOCK_H_

#include "common.h"
#include "ClockI2C.h"
#include "FAT16SD.h"

#define DIGITSEGADDR (19584000)

//char fileName[8], char fileExt[3]
int initClock();
void displayClock(uint32_t frame);





#endif /* CLOCK_H_ */