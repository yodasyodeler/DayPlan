#include "DrawApplication.h"
//void testTouch()
//{
//	uint32_t x,y;
//	uint16_t dotColor = GREEN;
//	//uint32_t numTouchs = 0;
//	while(1)
//	{
//
////		numTouchs = (IORD_32DIRECT(I2C_DATA_REG_0_BASE,1));
////		dotColor = color[1];
//		x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//		if (x & 0x80000000)
//		{
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//			drawFrame(BLACK, 1);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 1);
//
//			usleep(10);
//			flipFrame(1);
//
//			x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//			drawFrame(BLACK, 0);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 0);
//
//			usleep(10);
//			flipFrame(0);
//
//
//			x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//
//			drawFrame(BLACK, 1);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 1);
//
//
//			usleep(10);
//			flipFrame(1);
//
//
//			x = IORD_32DIRECT(I2C_DATA_REG_0_BASE,0);
//			y =  (x & 0x0FFF);
//			x = ((x>>16));
//
//			drawFrame(BLACK, 0);
//			drawCircleFill(dotColor, (x&0x0FFF), y, 30, 0);
//			flipFrame(0);
//			usleep(10);
//			//printf("Touch x: %lX\t\tTouch y: %lX\n", x, y);
//		}
//		else
//		{
//			drawFrame(BLACK, 1);
//			flipFrame(1);
//		}
//	}
//}
//
//void DrawApplication()
//{
//	int x, y;
//	uint16_t dotColor = RED;
//	uint16_t dotSize = 18;
//
//	drawFrame(WHITE,0);
//	DrawColorPicker(0);
//	flipFrame(0);
//	drawFrame(WHITE,1);
//	DrawColorPicker(1);
//	while (1)
//	{
//		x = grabTouchData(0);
//		y = (x & 0x0FFF);
//		x = ((x>>16));
//
//		//draw
//		while (x & 0x8000){
//			if (y > 40){
////				while (x & 0x8000){
//					x = x & 0x0FFF;
////					drawCircleFill(dotColor, x, y, dotSize, 1);
////					DrawColorPicker(1);
////					flipFrame(1);
//
//					drawCircleFill(dotColor, x, y, dotSize, 0);
//					DrawColorPicker(0);
////					flipFrame(0);
//
//					x = grabTouchData(0);
//					y = (x & 0x0FFF);
//					x = ((x>>16));
////				}
//			}
//			else{
//				switch ((x & 0x0FFF)/COLORPICKERSIZE){
//					case 0:
//						dotColor = COLORS[0];
//						break;
//					case 1:
//						dotColor = COLORS[1];
//						break;
//					case 2:
//						dotColor = COLORS[2];
//						break;
//					case 3:
//						dotColor = WHITE;
//						break;
//					case 4:
//						drawFrame(WHITE,0);
//						DrawColorPicker(0);
//						break;
//					default:
//						dotColor = COLORS[4];
//				}
//				/*	Touch Lockout   */
//				while (x & 0x80000000)
//					x = grabTouchData(0);
//			}
//		}
//	}
//}
//
//void DrawColorPicker(uint8_t frame)
//{
//	drawRectangleFill(COLORS[0], 0, 0, COLORPICKERSIZE, 40, frame);
//	drawRectangleFill(COLORS[1], COLORPICKERSIZE, 0, (COLORPICKERSIZE)*2, 40, frame);
//	drawRectangleFill(COLORS[2], COLORPICKERSIZE*2, 0, (COLORPICKERSIZE)*3, 40, frame);
//	drawRectangleFill(WHITE, COLORPICKERSIZE*3, 0, (COLORPICKERSIZE)*4, 40, frame);
//	drawRectangleFill(GREY, COLORPICKERSIZE*4, 0, (COLORPICKERSIZE)*5, 40, frame);
//}

/* display variables */
static uint32_t wFrame;

/* Image IDs */
static uint32_t nextARRWID[2];

/* draw variables */
static uint8_t wBrush = 0;
static uint16_t wColor = RED;

/* operation variables */
static DrawState dState = DRAWING;


int initWhiteboard(uint32_t frame)
{
	wFrame = frame;
	// set-up frame one
	drawFrame(OFFWHITE,wFrame);
	//drawRectangleFill(GREY, 10, 10, 50, 50, wFrame);
	nextARRWID[0] = createImage("NEXTARW ", "BMP", NULL);
	setScaleImage(nextARRWID[0], 3, 3);
	setWindowImage(nextARRWID[0], 0, 0, 32, 240);
	moveImage(nextARRWID[0], 10, 120);
	setFrameImage(nextARRWID[0], wFrame);

	nextARRWID[1] = createImage("NEXTARW ", "BMP", NULL);
	setScaleImage(nextARRWID[1], 3, 3);
	setWindowImage(nextARRWID[1], 32, 0, 64, 240);
	moveImage(nextARRWID[1], 750, 120);
	setFrameImage(nextARRWID[1], wFrame);

	displayImage(nextARRWID[0]);
	displayImage(nextARRWID[1]);


	dState = DRAWING;

	return 0;
}

void swapToWhiteboard()
{
	int i;
	flipFrame(wFrame);
	timerFunc = NULL;
	touchContinuousFunc = onHoldWhiteboard;
	touchReleaseFunc = onReleaseWhiteboard;
	touchFivePointFunc = onAllFingerPressWhiteboard;

	for(i=0;i<5000;++i);

	refreshScreen();
}

void onHoldWhiteboard(uint16_t x, uint16_t y)
{
	//extra restrictions can go here
	if (x > 10 && y > 10 && x < 790 && y < 470)
	{
		switch(wBrush)
		{
			case 0:
				drawCircleFill(wColor, x, y, 15, wFrame);
				break;
			case 1:
				drawRectangleFill(wColor, x-10, y-10, x+10, y+10, wFrame);
		}
	}
	refreshScreen();
}

void onReleaseWhiteboard(uint16_t x, uint16_t y)
{
	int i;
	displayImage(nextARRWID[0]);
	displayImage(nextARRWID[1]);


	if (x<90){
		if (y<360 && y>120){

			//change operation Calendar left
			swapToCal();

		}
	}
	else if (x>710){
		if (y<360 && y>120){
				//change operation Clock right
			swapToClock();
		}
	}

	refreshScreen();
}

void onAllFingerPressWhiteboard(void)
{

	drawFrame(OFFWHITE,wFrame);

	displayImage(nextARRWID[0]);
	displayImage(nextARRWID[1]);


//	refreshScreen();
}
