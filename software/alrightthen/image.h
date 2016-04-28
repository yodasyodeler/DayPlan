#ifndef IMAGE_H_
#define IMAGE_H_

#include "common.h"
#include "c_stl.h"
#include "bitmapSD.h"
#include <stdlib.h>
#include "c_list.h"


/* iInfo
 *   size:     Half-Word(16bit) Size
 *   cRes:     Color Resolution (MONO1, RGB16)
 *   width:    Image Half-Word Width
 *   height:   Image Pixel Height
 *   address:  SDRAM Storage Location
 *   fileName: Name of file
 */
typedef struct  _iInfo{
	uint32_t size;
	uint8_t cRes;
	uint16_t width;
	uint16_t height;
	uint16_t padWidth;
	uint32_t address;
	char fileName[8];
} iInfo;

typedef struct _window{
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
} window;

/*  iImage   *Needs Fixing on Scale*
 *    ID:    Unique Image Number for Identification
 *    x:     X position of Top Left Corner of Image In (RGB16)Pixel from Top Left Corner of Frame
 *    y:     Y position of Top Left Corner of Image In (RGB16)Pixel from Top Left Corner of Frame
 *    scale: Multiplier of Current Scale
 *             XXXX_0000: Don't Display
 *             XXXX_0001: 0.25x Quarter Size Height
 *             XXXX_0010: 0.5x  Half Size Height
 *             XXXX_0011: 1x    Normal Size Height
 *             XXXX_0101: 2x    Double Size Height
 *             XXXX_0110: 4x    Four Times Size Height
 *             XXXX_0111: 8x    Eight Times Size Height
 *             XXXX_1XXX: reserved
 *
 *             0000_XXXX: Don't Display
 *             0001_XXXX: 0.25x Quarter Size Width
 *             0010_XXXX: 0.5x  Half Size Width
 *             0011_XXXX: 1x    Normal Size Width
 *             0101_XXXX: 2x    Double Size Width
 *             0110_XXXX: 4x    Four Times Size Width
 *             0111_XXXX: 8x    Eight Times Size Width
 *             1XXX_XXXX: reserved
 *             Default size is 0000_0000 Not Shown
 *             Scaling Originates From The Top Left Of The Image
 *    frame: Screen Frame to Display Image on, for Buffering Images on RAM
 *    layer: Determines Order of Images being displayed For displayFrameImage()
 *           Defaults to layer 0, not specifying layer on overlapping images
 *           creates undefined behavior of which image is on top
 */
typedef struct _iImage{
	uint32_t ID;
	iInfo* info;
	uint16_t x;
	uint16_t y;
	uint8_t scale;
	uint8_t frame;
	uint8_t layer;
	window* w;
} iImage;

typedef struct Font_Cursor {
	uint16_t x;
	uint16_t y;
	uint8_t scale;
	uint16_t frame;
	uint16_t color;
	uint32_t ID;
} FontCursor;

uint32_t createImage(const char fileName[8],const char fileExt[3], iInfo* info);
uint32_t deleteImage(uint32_t ID);

void deleteAllImage();
void closeImage();

uint32_t displayImage(uint32_t ID);
void displayFrameImage(uint8_t frame);

uint32_t displayFont16Image(unsigned char c, FontCursor cursor);
uint32_t displayFont64Image(unsigned char c,FontCursor cursor);

uint32_t displayMonochromeImage(uint32_t ID);
uint32_t displayRGB16Image(uint32_t ID);
uint32_t displayFastRGB16Image(uint32_t ID);

void setMonoColorImage(uint16_t color);

uint32_t setLayerImage(uint32_t ID, uint8_t layer);
uint32_t setScaleImage(uint32_t ID, uint8_t scaleX, uint8_t scaleY);
uint32_t setFrameImage(uint32_t ID, uint8_t frame);
uint32_t setWindowImage(uint32_t ID, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
uint32_t moveImage(uint32_t ID, uint16_t x, uint16_t y);

uint32_t getInfoImage(uint32_t ID, const iInfo* info);


#endif
