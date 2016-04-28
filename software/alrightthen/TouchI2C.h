#ifndef TOUCHI2C_H_
#define TOUCHI2C_H_

#include "i2cAvalon.h"

#define TOUCHADDR 0x70

void initTouchI2C();

uint8_t readNumTouch();
uint32_t readTouchData(uint8_t touchPoint);
void readTouchCord(uint8_t touchPoint, uint16_t* x, uint16_t* y);

#endif
