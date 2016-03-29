#ifndef FONTWRITERSPI_H_
#define FONTWRITERSPI_H_

#include "stdarg.h"
#include "FAT16SD.h"
#include "io.h"
#include "system.h"
#include "drawLib.h"
#include "c_stl.h"

#define CHARWIDTH (16)
#define CHARHEIGHT (16)
#define FONTADDR (1920000)

int initFontMap(char fileName[8], char fileExt[3]);
void moveCursorFont(uint16_t x, uint16_t y, uint8_t page);
void changeColorFont(uint16_t color);


uint16_t writeCharFont(unsigned char c);
int writeStringFont(char* str, uint8_t isKern);

int vprints(const char* fmt, uint8_t isKern, va_list args);
int printk(const char* fmt, ... );
int printg(const char* fmt, ... );

#endif
