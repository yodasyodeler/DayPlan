#include "i2cAvalon.h"

int isReadyI2C()
{
	return ((IORD(I2CADDR, 1) & 0x80000) );
}
void setRSTI2C(uint8_t reset)
{
	IOWR(I2CADDR, 1, reset);
}
void enableByteI2C(uint8_t en)
{
	IOWR(I2CADDR, 2, en);
}
void setDVSRI2C(uint16_t dvsr)
{
	IOWR(I2CADDR, 3, dvsr);
}

void writeByteI2C(uint8_t address, uint8_t regNum, uint8_t data)
{
	enableByteI2C(2);
	IOWR(I2CADDR, 0, (address<<24)|(regNum<<16) | (data<<8));
	while(!isReadyI2C());
	enableByteI2C(1);
}

void writeHalfI2C(uint8_t address, uint8_t regNum, uint16_t data)
{
	enableByteI2C(3);
	IOWR(I2CADDR, 0, (address<<24)|(regNum<<16) | (data) );
	while(!isReadyI2C());
	enableByteI2C(1);
}

void writeWordI2C(uint8_t address, uint8_t regNum, uint32_t data)
{
	writeHalfI2C(address, regNum, data);
	writeHalfI2C(address, regNum+2, (data>>16));
}

uint32_t readI2C(uint8_t address, uint8_t regNum)
{
	IOWR(I2CADDR, 0, (address|1)<<24|(regNum<<16));
	while(!isReadyI2C());
	return IORD(I2CADDR, 0);
}
