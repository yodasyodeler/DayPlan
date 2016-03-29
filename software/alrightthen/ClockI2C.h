#ifndef CLOCKI2C_H_
#define CLOCKI2C_H_

#include "i2cAvalon.h"

#define CLOCKADDR 0xD0
#define STANDARDTIME 0x40
#define PM 0x20

void readTime(uint8_t* seconds, uint8_t* minutes, uint8_t* hours);
void readDate(uint8_t* weekday, uint8_t* day, uint8_t* month, uint8_t* year);

void writeTime(uint8_t seconds, uint8_t minutes, uint8_t hours);
void writeseconds(uint8_t seconds);
void writeMinutes(uint8_t minutes);
void writeHour(uint8_t hours);

#endif /* CLOCKI2C_H_ */
