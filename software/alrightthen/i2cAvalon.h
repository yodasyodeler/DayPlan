#ifndef I2CAVALON_H_
#define I2CAVALON_H_

#include <unistd.h>
#include <system.h>
#include <stdint.h>
#include <io.h>

#define I2CADDR I2C_AVALON_0_BASE

#define FREQ100K 1000
#define FREQ200K 500
#define FREQ400K 250			//ACTUAL->403,226Hz

void initI2C();

int isReadyI2C();
void setRSTI2C(uint8_t reset);
void enableByteI2C(uint8_t en);
void setDVSRI2C(uint16_t dvsr);

void writeByteI2C(uint8_t address, uint8_t regNum, uint8_t data);
void writeHalfI2C(uint8_t address, uint8_t regNum, uint16_t data);
void writeWordI2C(uint8_t address, uint8_t regNum, uint32_t data);

uint32_t readI2C(uint8_t address, uint8_t regNum);

#endif
