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

#define xyLocation(x,y) ((x)+((y)*(ROWSIZE)))

#define GREEN 0x07E0
#define RED 0xF800
#define BLUE 0x001F
#define PURPLE 0xF81F
#define YELLOW 0xFFE0
#define SKYBLUE 0x07FF
#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY 0x4A49

#endif
