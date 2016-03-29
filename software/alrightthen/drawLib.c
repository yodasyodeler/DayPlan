#include "drawLib.h"



void drawRectangleFill(uint16_t color, int x0, int y0, int x1, int y1, uint8_t frame)
{
	uint16_t height = ((y0<y1)? y1-y0 : y0-y1);
	uint16_t length = ((x0<x1)? x1-x0 : x0-x1);
	uint16_t x		= ((x0<x1)?    x0 : x1);
	uint16_t y 		= ((y0<y1)?    y0 : y1);

	while (height--)
		drawLineH(color, x, y++, length, frame);
}

void drawCircle(uint16_t color, int x0, int y0, int radius, uint8_t frame)
{
  int shift = 0;
  int offset = FRAMEADDRSIZE*frame;
  int decisionOver2 = 1 - radius;

  while( shift <= radius )
  {
	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( shift +x0,-radius+y0)<<2)+offset, color); //Clockwise
	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( radius+x0,-shift +y0)<<2)+offset, color);

	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( radius+x0, shift +y0)<<2)+offset, color);
	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( shift +x0, radius+y0)<<2)+offset, color);

	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-shift +x0, radius+y0)<<2)+offset, color);
	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-radius+x0, shift +y0)<<2)+offset, color);

	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-radius+x0,-shift +y0)<<2)+offset, color);
	  IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-shift +x0,-radius+y0)<<2)+offset, color);


    ++shift;
    if (decisionOver2<=0)
    {
      decisionOver2 += 2 * shift + 1;
    }
    else
    {
      --radius;
      decisionOver2 += 2 * (shift - radius) + 1;   // Change for y -> y+1, x -> x-1
    }
  }
}
void drawCircleFill(uint16_t color, int x0, int y0, int radius, uint8_t frame)
{
  int shift = 0;
  int decisionOver2 = 1 - radius;
  int length;

  while( shift <= radius )
  {
	  length =  (radius<<1);
	  drawLineH( color, -radius+x0,  shift +y0, length, frame);
	  drawLineH( color, -radius+x0, -shift +y0, length, frame);

    ++shift;
    if (decisionOver2<=0)
    {
    	decisionOver2 += 2 * shift + 1;
    }
    else
    {
    	length = shift<<1;
    	drawLineH(color, -shift +x0, -radius+y0, length, frame);
    	drawLineH(color, -shift +x0,  radius+y0, length, frame);
		--radius;
    	decisionOver2 += 2 * (shift - radius) + 1;
    }
  }
}
int  readDrawFrom(int address)
{
	return IORD_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, address);
}
void drawTo(uint16_t data, int address)
{
	IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, address<<2, data);
}
void drawToFrame(int address, uint16_t data, uint8_t frame)
{
	IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, (address<<2)+(FRAMEADDRSIZE*frame), data);
}
void drawFrame(uint16_t color, uint8_t frame)
{
	int i;
	int offset = frame*FRAMEADDRSIZE;
	for (i=0; i<FRAMEADDRSIZE; i+=4)
		IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, i+offset, color);
}
void flipFrame(int frame)
{
	IOWR_32DIRECT(FRAMEBUFFER_SDRAM_0_BASE, 0, ((FRAMEADDRSIZE*frame)|0x01) );
}
void drawLineH(uint16_t color, uint16_t startx, uint16_t starty, int length, uint8_t frame)
{
	int i;
	int offset = 0;

	offset += xyLocation(startx,starty)<<2;
	offset += FRAMEADDRSIZE*frame;

	length = length<<2;
	for (i=0; i<length; i+=4)
		IOWR_16DIRECT(NEW_SDRAM_CONTROLLER_0_BASE, offset+i, color);
}
