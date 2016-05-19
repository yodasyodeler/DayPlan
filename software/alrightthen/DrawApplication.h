#ifndef DRAWAPPLICATION_H_
#define DRAWAPPLICATION_H_
#include "drawLib.h"
#include "common.h"
#include "image.h"

typedef enum _DrawState{
	DRAWING=0,
	EDITTOOLS,
}DrawState;

int initWhiteboard(uint32_t frame);
void onHoldWhiteboard(uint16_t x, uint16_t y);
void onReleaseWhiteboard(uint16_t x, uint16_t y);
void onAllFingerPressWhiteboard(void);


#endif
