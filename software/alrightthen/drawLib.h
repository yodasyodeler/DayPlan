#ifndef DRAWLIB_H
#define DRAWLIB_H
#include <system.h>
#include <io.h>
#include <stdint.h>
#include "common.h"


void refreshScreen();
uint8_t isBusyScreenDraw();

void drawRectangleFill(uint16_t color, int x0, int y0, int x1, int y1, uint32_t frame);
void drawCircle(uint16_t color, int x0, int y0, int radius, uint32_t frame);
void drawCircleFill(uint16_t color, int x0, int y0, int radius, uint32_t frame);
int  readDrawFrom(uint32_t address);
void drawTo(uint16_t data, uint32_t address);
void drawToFrame(uint32_t address, uint16_t data, uint32_t frame);
void drawFrame(uint16_t color, uint32_t frame);
void flipFrame(uint32_t frame);
void drawLineH(uint16_t color, uint16_t startx, uint16_t starty, int length, uint32_t frame);

#endif
