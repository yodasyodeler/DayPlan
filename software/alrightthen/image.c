#include "image.h"

uint8_t imageNum = 0;
uint32_t ImageAddress = 6912000; //Frame 9, zero index in calculation
uint16_t monoChromeColor = BLACK;

node* imageList = NULL;
node* infoList = NULL;

/*  compID
 *    Helper Function for Finding an ID
 *    for imageList
 *    Args: iImage , int ID
 *    returns 0 when equivalent
 */
int compID(void* data1, const void* data2)
{
	return ( ((iImage*)(data1))->ID - (*(int*)(data2)) );
}

/*  compFrame
 *    Helper Function for Finding a Frame
 *    for imageList
 *    Args: iImage , int Frame
 *    returns 0 when equivalent
 */
int compFrame(void* data1, const void* data2)
{
	return ((iImage*)(data1))->frame - (*(int*)(data2));
}

/*  compName
 *    Helper Function for Finding if an Image
 *    has Already been Loaded into Memory
 *    for infoList
 *    Args: iInfo , char* -> filename[8]
 *    returns 0 when equivalent
 */
int compName(void* data1, const void* data2)
{
	return c_strcmp( ((iInfo*)data1)->fileName, data2);
}

/*  deleteDummy
 *    If extra delete is not needed
 */
void deleteDummy(void* data)
{}

/*  deleteWindow()
 *    deletes Window
 */
void deleteWindow(void* data)
{
	free( ((iImage*)data)->w );
}

/*  createImage
 *    Registers an Image
 *    If file is not on ram it is loaded in
 *    returns:
 *      re == 0 :  File Could Not Be Loaded
 *      re == -1: Failure to Allocate Memory
 *      re > 0  : ID of Image
 */
uint32_t createImage(const char fileName[8], const char fileExt[3], iInfo* info)
{
	iImage* imageC = NULL;
	iInfo* infoC = NULL;
	bitmapHeader header;
	node* temp;

	/* Didn't find Image */
	if ( (temp = searchNode(infoList, fileName, compName)) == NULL){
		if (loadBitmap(fileName, fileExt, ImageAddress, &header))
			return 0;

		infoC = malloc(sizeof(iInfo));
		if (infoC == NULL)
			return -1;

		/* Populate New Image File Info */
		c_strcpy(infoC->fileName, fileName);
		infoC->cRes = header.pixelResolution;
		infoC->size = (header.size>>1);
		infoC->height = header.height;
		infoC->width = header.width;
		infoC->padWidth = header.padWidth;
		infoC->address = ImageAddress;
		ImageAddress += infoC->size;

		/* append new info */
		addNode(&infoList, infoC);
	}
	else{
		infoC = (iInfo*)temp->_data;
	}

	imageC = malloc(sizeof(iImage));
	if (imageC == NULL)
		return -1;

	/* Populate New Image */
	imageC->info = infoC;
	imageC->x = 0;
	imageC->y = 0;
	imageC->layer = 0;
	imageC->frame = 0;
	imageC->scale = 0;
	imageC->w = NULL;

	addNode(&imageList, imageC);
	imageC->ID = ++imageNum;

	if (info != NULL)
		*info = *infoC;

	return imageC->ID;
}
uint32_t deleteImage(uint32_t ID)
{
	return removeNode(&imageList, &ID, compID, deleteWindow);
}

void deleteAllImage()
{
	purgeNode(&imageList, deleteWindow);
}

/* displayImage
 *    Displays a Image if ID matches
 *    an existing image In the List
 *    Takes in account
 *      scale, frame, x, y
 *    Does Not Account For Layer
 *    returns:
 *              0: Success
 *             -1: ID Not Found
 *             -2: Color Resolution Not Supported
 *    Calls:
 *      If monochrome: diplayMonochromeImage
 *      If RGB16 & scale 3: displayRGB16FastImage
 *      Else If RGB16: displayRGB16Image
 *
 *      RGB16 only supports 565 Format
 */
uint32_t displayImage(uint32_t ID)
{
	iImage* ptr = NULL;
	uint16_t scaleX, scaleY;
	uint8_t re = 0;
	node* n = NULL;


	if ( (n = searchNode(imageList, &ID, compID)) == NULL)
			return -1;
	ptr = (iImage*)(n->_data);

	scaleX = (ptr->scale&0xF0)>>4;
	scaleY = (ptr->scale&0x0F);

	if (scaleX && scaleY){
		switch (ptr->info->cRes)
		{
			case 1:
				re = displayMonochromeImage(ID);
				break;
			case 16:
//				if (ptr->scale == 0x33)
//					re = displayFastRGB16Image(ID);
//				else
					re = displayRGB16Image(ID);
				break;
			default:
				re = -2;
		}
	}

	return re;

}

/* displayMonochromeImage
 *    Displays a Image if ID matches
 *    an existing image In the List
 *    Must BE A Monochrome Image
 *    Takes in Account
 *      scale, frame, x, y, Window
 *    Does Not Account For Layer
 *    returns:
 *              0: Success
 *             -1: ID Not Found
 */
uint32_t displayMonochromeImage(uint32_t ID)
{
	int i, j, x, y,k;
		iImage* ptr = NULL;
		uint16_t pixel;

		uint32_t fAddress;
		uint32_t pixelOffset;

		node* n;

		uint8_t divX, divY;
		uint8_t multX = 1;
		uint8_t multY = 1;

		uint32_t endX, endY;
		uint32_t startX, startY;
		uint16_t inX, inY;

		if ((n = searchNode(imageList, &ID, compID)) == NULL)
				return -1;

		ptr = (iImage*)n->_data;

		divX = (ptr->scale&0xF0)>>4;
		divY = (ptr->scale&0x0F);



		fAddress = (FRAMEADDRSIZE*(ptr->frame)) + xyLocation(ptr->x,ptr->y);


		if (divX < 3)
			divX = (4-divX);
		else{
			multX = divX - 2;
			divX = 1;
		}

		if (divY < 3)
			divY = (4-divY);
		else{
			multY = divY - 2;
			divY = 1;
		}

		/* Setup Window */
		if (ptr->w){
			startX = ((ptr->w->x0)>>4);
			startY = (ptr->w->y0);
			endX = ((ptr->w->x1)>>4);
			endY = (ptr->w->y1);
		}
		else{
			startX = 0;
			startY = 0;
			endX = (ptr->info->width>>4);
			endY = ptr->info->height;
		}

		inY = 0;
		for (i=startY; i<endY; ++i){
			inX = 0;
			for (j=startX; j<endX; ++j){
				pixelOffset = ((inY)*(ROWSIZE)) + fAddress;
				pixel = IORD(NEW_SDRAM_CONTROLLER_0_BASE, ptr->info->address + j + (inY*(ptr->info->padWidth)) +  (startY*ptr->info->padWidth) ); //((ptr->info->width>>4)+1)
				for (x=0; x<16; ++x){
					if (((~pixel)<<x) & 0x8000)
						IOWR(NEW_SDRAM_CONTROLLER_0_BASE, pixelOffset + inX,monoChromeColor);
					++inX;
				}

			}
			++inY;
		}

	return 0;
}

/*  displayRGB16Image
 *    Displays a Image if ID matches
 *    an existing image In the List
 *    Must BE A 16 Bit RGB Image
 *    Takes in account
 *      scale, frame, x, y
 *    Does Not Account For Layer
 *    returns:
 *              0: Success
 *             -1: ID Not Found
 */
uint32_t displayRGB16Image(uint32_t ID)
{
	int i = 0;
	int j = 0;
	int x, y;
	iImage* ptr = NULL;
	uint8_t divX, divY;
	uint8_t multX = 1;
	uint8_t multY = 1;
	uint16_t pixel;
	uint16_t inX, inY;

	uint32_t fAddress;
	uint32_t pixelOffset;
	uint32_t endX, endY;
	uint32_t startX, startY;

	node* n;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
		return -1;

	ptr = (iImage*)n->_data;

	divX = (ptr->scale&0xF0)>>4;
	divY = (ptr->scale&0x0F);

	inY = ptr->y;

	fAddress = ((ptr->frame)*FRAMESIZE);

	if (divX < 3)
		divX = (4-divX);
	else{
		multX = divX - 2;
		divX = 1;
	}

	if (divY < 3)
		divY = (4-divY);
	else{
		multY = divY - 2;
		divY = 1;
	}

	/* Setup Window */
	if (ptr->w){
		startX = ptr->w->x0;
		startY = ptr->w->y0;
		endX = ptr->w->x1;
		endY = ptr->w->y1;
	}
	else{
		endX = ptr->info->width;
		endY = ptr->info->height;
	}




	/* Iterate Pixels
	 *   Outer Loops Traverses pixels in Ram
	 *   Scale Affects Traversal if below 3
	 */
	for(i = startY; i < endY && inY < COLSIZE; i += divY){
		//iterate this loop for increase in y
		for(y=0; y<multY; ++y){
			inX = ptr->x;
			pixelOffset = ((inY)*(ROWSIZE)) + fAddress;
			for(j = startX; j < endX && inX < ROWSIZE; j +=divX){
				//Read Pixel
				pixel = IORD(NEW_SDRAM_CONTROLLER_0_BASE, ptr->info->address + (i*ptr->info->width) + j);
				//iterate this loop for increase in x
				for(x=0; x < multX; ++x){
					IOWR(NEW_SDRAM_CONTROLLER_0_BASE, pixelOffset + inX++, pixel);
				}
			}
			++inY;
		}
	}

	return 0;
}

/*  displayRGBFast16Image
 *    Displays a Image if ID matches
 *    an existing image In the List
 *    Must BE A 16 Bit RGB Image
 *    Takes in account
 *      frame, x, y
 *    Does Not Account For Layer
 *    Ignores Scale conditions
 *    returns:
 *              0: Success
 *             -1: ID Not Found
 */
uint32_t displayFastRGB16Image(uint32_t ID)
{
	int i, j;
	iImage* ptr = NULL;
	uint16_t pixel;

	uint32_t fAddress;
	uint32_t pixelOffset;

	node* n;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
			return -1;

	ptr = (iImage*)n->_data;

	fAddress = ((ptr->frame)*FRAMESIZE);

	for(i=0; i < ptr->info->height; ++i){
		pixelOffset = (((ptr->y+i)*ROWSIZE)+ ptr->x) + fAddress;
		for(j=0; j < ptr->info->width; ++j){
			pixel = IORD(NEW_SDRAM_CONTROLLER_0_BASE, ptr->info->address + (i*ptr->info->width) + j);
			IOWR(NEW_SDRAM_CONTROLLER_0_BASE, pixelOffset + j, pixel);
		}
	}

	return 0;
}

/*  displayFont16Image
 *    Displays a font of a color and scale
 *    Uses Image system to allow for change of
 *    font
 *    Font Must be 16x16
 *    returns:
 *             0>=: kern value for kerning text
 *             -1: ID Not Found
 *    If Image is not a Font Operation is undefined
 */
uint32_t displayFont16Image(unsigned char c,FontCursor cursor)
{
	int address;
	int fAddress = (FRAMEADDRSIZE*cursor.frame)+ cursor.x;//xyLocation(cursor.x,cursor.y);
	uint16_t kern = 0;
	uint16_t row;
	int i, j, x, y;
	uint8_t scaleX, scaleY;
	uint16_t inX, inY;
	iImage* ptr = NULL;
	uint32_t pixelOffset;

	node* n;
	if ((n = searchNode(imageList, &(cursor.ID), compID)) == NULL)
			return -1;

	ptr = (iImage*)n->_data;
	address = ptr->info->address + (((c-32)/(ptr->info->width>>4))*(ptr->info->width)) + ((c-32)%(ptr->info->width>>4));

	inY = cursor.y;

	scaleX = (cursor.scale&0xF0)>>4;
	scaleY = (cursor.scale&0x0F);

	for (i=0;i<16; ++i){
		row = IORD( NEW_SDRAM_CONTROLLER_0_BASE, address + i*((ptr->info->width)>>4) );
		for(y=0; y<scaleY; ++y){ //scale y
			inX = 0;
			pixelOffset = ((inY)*(ROWSIZE)) + fAddress;
			for (j=0; j<16; ++j ){
				if ((row<<j) &0x8000){
					for(x = 0; x <scaleX; ++x){ //scale x
						IOWR(NEW_SDRAM_CONTROLLER_0_BASE, pixelOffset + inX + x, cursor.color);
						if (j > kern)
							kern = j;
					}
				}
				inX+=scaleX;
			}
			++inY;
		}
	}
	return kern*scaleX;
}

/*  displayFont64Image
 *    Displays a font of a color and scale
 *    Uses Image system to allow for change of
 *    font
 *    Font Must be 16x16
 *    returns:
 *             0>=: kern value for kerning text
 *             -1: ID Not Found
 *    If Image is not a Font Operation is undefined
 */
uint32_t displayFont64Image(unsigned char c, FontCursor cursor)
{
	int address;
	int fAddress = (FRAMEADDRSIZE*cursor.frame)+cursor.x;//xyLocation(cursor.x,cursor.y);
	uint16_t kern = 0;
	int i, j, x, y;
	uint8_t scaleX, scaleY;
	uint16_t inX, inY;
	iImage* ptr = NULL;
	uint32_t pixelOffset;
	uint64_t pixel;

	node* n;
	if ((n = searchNode(imageList, &(cursor.ID), compID)) == NULL)
			return -1;

	ptr = (iImage*)n->_data;
	address = ptr->info->address + (((c-32)/(ptr->info->width>>6))*((ptr->info->width)<<2)) + (((c-32)%(ptr->info->width>>6))<<2);

	inY = cursor.y;

	scaleX = (cursor.scale&0xF0)>>4;
	scaleY = (cursor.scale&0x0F);

	for (i=0; i<(64<<2); i+=4){
		pixel  =  ((uint64_t)IORD( NEW_SDRAM_CONTROLLER_0_BASE, address + i*((ptr->info->width)>>6))<< 48);
		pixel |= ((uint64_t)IORD( NEW_SDRAM_CONTROLLER_0_BASE, address + i*((ptr->info->width)>>6) + 1 ) << 32);
		pixel |= ((uint64_t)IORD( NEW_SDRAM_CONTROLLER_0_BASE, address + i*((ptr->info->width)>>6) + 2 ) << 16);
		pixel |= ((uint64_t)IORD( NEW_SDRAM_CONTROLLER_0_BASE, address + i*((ptr->info->width)>>6) + 3 ) );
		for(y=0; y<scaleY; ++y){ //scale y
			inX = 0;
			pixelOffset = ((inY)*(ROWSIZE)) + fAddress;
			for (j=0; j<64; ++j ){
				if ((pixel<<j) & 0x8000000000000000){
					for(x = 0; x <scaleX; ++x){ //scale x
						IOWR(NEW_SDRAM_CONTROLLER_0_BASE, pixelOffset + inX + x, cursor.color);
						if (j > kern)
							kern = j;
					}
				}
				inX+=scaleX;
			}
			++inY;
		}
	}
	return kern*scaleX;
}

void displayFrameImage(uint8_t frame)
{

}

void setMonoColorImage(uint16_t color)
{
	monoChromeColor = color;
}
uint16_t getMonoColorImage()
{
	return monoChromeColor;
}


uint32_t setLayerImage(uint32_t ID, uint8_t layer)
{
	node* n;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
				return -1;

	((iImage*)(n->_data))->layer = layer;

	return 0;
}
uint32_t setScaleImage(uint32_t ID, uint8_t scaleX, uint8_t scaleY)
{
	node* n;
		if ((n = searchNode(imageList, &ID, compID)) == NULL)
					return -1;

		((iImage*)(n->_data))->scale = (scaleX<<4)|scaleY;

		return 0;
}
uint32_t setFrameImage(uint32_t ID, uint8_t frame)
{
	node* n;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
				return -1;

	((iImage*)(n->_data))->frame = frame;

	return 0;
}

uint32_t setWindowImage(uint32_t ID, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	node* n;
	window** ptr;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
		return -1;

	ptr = &(((iImage*)(n->_data))->w);
	if ( (*ptr) == NULL)
	{
		(*ptr) = malloc(sizeof(window));
		if ((*ptr) == NULL)
			return -2;
	}
	(*ptr)->x0 = x0;
	(*ptr)->x1 = x1;
	(*ptr)->y0 = y0;
	(*ptr)->y1 = y1;

	return 0;
}
uint32_t moveImage(uint32_t ID, uint16_t x, uint16_t y)
{
	node* n;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
				return -1;

	((iImage*)(n->_data))->x = x;
	((iImage*)(n->_data))->y = y;

	return 0;
}

iInfo* getInfoImage(uint32_t ID)
{
	node* n;
	if ((n = searchNode(imageList, &ID, compID)) == NULL)
				return NULL;
	return ((iImage*)(n->_data))->info;
}
