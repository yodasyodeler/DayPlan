#include "TouchI2C.h"
void initTouchI2C()
{
	setRSTI2C(0);
	usleep(10000);
	setRSTI2C(1);
	usleep(10000);
	enableByteI2C(0x1);
	setDVSRI2C(FREQ200K);
}

uint8_t readNumTouch()
{
	return (readI2C(TOUCHADDR, 0x02)>>24);
}

uint32_t readTouchData(uint8_t touchPoint)
{
	return readI2C(TOUCHADDR, ((6*touchPoint)+3) );
}
void readTouchCord(uint8_t touchPoint, uint16_t* x, uint16_t* y)
{
	uint32_t data = readTouchData(touchPoint);

	*x = ((data>>16) & 0x0FFF);
	*y = (data & 0x0FFF);
}
