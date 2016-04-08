#ifndef DRAWLIB_H
#define DRAWLIB_H
#include <system.h>
#include <io.h>
#include <stdint.h>
#include "common.h"


void refreshScreen();
uint8_t isBusyScreenDraw();

void drawRectangleFill(uint16_t color, int x0, int y0, int x1, int y1, uint8_t frame);
void drawCircle(uint16_t color, int x0, int y0, int radius, uint8_t frame);
void drawCircleFill(uint16_t color, int x0, int y0, int radius, uint8_t frame);
int  readDrawFrom(int address);
void drawTo(uint16_t data, int address);
void drawToFrame(int address, uint16_t data, uint8_t frame);
void drawFrame(uint16_t color, uint8_t frame);
void flipFrame(int frame);
void drawLineH(uint16_t color, uint16_t startx, uint16_t starty, int length, uint8_t frame);

#endif
