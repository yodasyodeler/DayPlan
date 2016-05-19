#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <system.h>
#include <io.h>

#define ROWSIZE (800)
#define COLSIZE (480)
#define FRAMESIZE (ROWSIZE*COLSIZE)
#define FRAMEADDRSIZE (FRAMESIZE)
#define NUMCOLORPICKER (5)
#define COLORPICKERSIZE ((ROWSIZE)/(NUMCOLORPICKER))

#define NULL ((void *)0)

#define xyLocation(x,y) ((x)+((y)*(ROWSIZE)))

#define GREEN (0x07E0)
#define RED (0xF800)
#define BLUE (0x001F)
#define PURPLE (0xF81F)
#define YELLOW (0xFFE0)
#define SKYBLUE (0x07FF)
#define BLACK (0x0000)
#define WHITE (0xFFFF)
#define OFFWHITE (0xEF5D)
#define LIGHTGREY (0x8C71)
#define GREY (0x4A49)
#define DARKGREY (0x39E7)

extern void (*timerFunc)(void);
extern void (*touchReleaseFunc)(uint16_t x, uint16_t y);
extern void (*touchFivePointFunc)(void);
extern void (*touchContinuousFunc)(uint16_t x, uint16_t y);

void swapToWhiteboard(void);
void swapToClock(void);
void swapToCal(void);

#endif
