#include "drawLib.h"

uint8_t isBusyScreenDraw()
{
	return (IORD(LCDFRAMEBUFFER_0_BASE, 0) & 0x2);
}
void refreshScreen()
{
	while(isBusyScreenDraw());
	IOWR(LCDFRAMEBUFFER_0_BASE, 0, 0);
}

void drawRectangleFill(uint16_t color, int x0, int y0, int x1, int y1, uint32_t frame)
{
	uint16_t height = ((y0<y1)? y1-y0 : y0-y1);
	uint16_t length = ((x0<x1)? x1-x0 : x0-x1);
	uint16_t x		= ((x0<x1)?    x0 : x1);
	uint16_t y 		= ((y0<y1)?    y0 : y1);

	while (height--)
		drawLineH(color, x, y++, length, frame);
}

void drawCircle(uint16_t color, int x0, int y0, int radius, uint32_t frame)
{
  int shift = 0;
  uint32_t offset = FRAMEADDRSIZE*frame;
  int decisionOver2 = 1 - radius;

  while( shift <= radius )
  {
	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( shift +x0,-radius+y0))+offset, color); //Clockwise
	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( radius+x0,-shift +y0))+offset, color);

	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( radius+x0, shift +y0))+offset, color);
	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation( shift +x0, radius+y0))+offset, color);

	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-shift +x0, radius+y0))+offset, color);
	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-radius+x0, shift +y0))+offset, color);

	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-radius+x0,-shift +y0))+offset, color);
	  IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (xyLocation(-shift +x0,-radius+y0))+offset, color);


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
void drawCircleFill(uint16_t color, int x0, int y0, int radius, uint32_t frame)
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
int  readDrawFrom(uint32_t address)
{
	return IORD(NEW_SDRAM_CONTROLLER_0_BASE, address);
}
void drawTo(uint16_t data, uint32_t address)
{
	IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address, data);
}
void drawToFrame(uint32_t address, uint16_t data, uint32_t frame)
{
	IOWR(NEW_SDRAM_CONTROLLER_0_BASE, (address)+(FRAMEADDRSIZE*frame), data);
}
void drawFrame(uint16_t color, uint32_t frame)
{
	int i;
	uint32_t offset = frame*FRAMEADDRSIZE;
	for (i=0; i<FRAMEADDRSIZE; ++i)
		IOWR(NEW_SDRAM_CONTROLLER_0_BASE, i+offset, color);
}
void flipFrame(uint32_t frame)
{
	IOWR(LCDFRAMEBUFFER_0_BASE, 3, (((FRAMEADDRSIZE*frame)<<2)|0x01) );
}
void drawLineH(uint16_t color, uint16_t startx, uint16_t starty, int length, uint32_t frame)
{
	int i;
	uint32_t offset = 0;

	offset += xyLocation(startx,starty);
	offset += FRAMEADDRSIZE*frame;

	for (i=0; i<length; ++i){
		IOWR(NEW_SDRAM_CONTROLLER_0_BASE, offset+i, color);
	}
}
