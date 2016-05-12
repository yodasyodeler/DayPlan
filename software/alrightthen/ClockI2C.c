#include "ClockI2C.h"

void readTime(uint8_t* seconds, uint8_t* minutes, uint8_t* hours)
{
	uint32_t data = readI2C(CLOCKADDR, 0);
	*seconds = ((data>>24)&0xFF);
	*minutes = ((data>>16)&0xFF);
	*hours = ((data>>8)&0xFF);
}

void readDate(uint8_t* weekday, uint8_t* day, uint8_t* month, uint8_t* year)
{
	uint32_t data = readI2C(CLOCKADDR, 3);
	*weekday = ((data>>24)&0xFF);
	*day = ((data>>16)&0xFF);
	*month = ((data>>8)&0xFF);
	*year = (data&0xFF);
}

void writeTime(uint8_t seconds, uint8_t minutes, uint8_t hours)
{
	writeHalfI2C(CLOCKADDR, 0, (seconds<<8)|minutes );
	writeByteI2C(CLOCKADDR, 2,  hours);
}

void writeseconds(uint8_t seconds)
{
	writeByteI2C(CLOCKADDR, 0, seconds);
}

void writeMinutes(uint8_t minutes)
{
	writeByteI2C(CLOCKADDR, 1, minutes);
}

void writeHour(uint8_t hours)
{
	writeByteI2C(CLOCKADDR, 2, hours);
}


void writeDate(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year)
{
	writeWordI2C(CLOCKADDR, 3, ((weekday<<24) | (day<<16) | (month<<8) | (year)) );
}
