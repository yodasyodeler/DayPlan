#include "fontWriterSPI.h"

static int fFileSize = 0;
static int fBitmapSize = 0;
static int fBitmapPos = 0;
static int fBitmapWidth = 0;
static int fBitmapHeight = 0;

static uint16_t cursorX = 0;
static uint16_t cursorY = 0;
static uint16_t cursorPage = 0;
static uint16_t fontColor = 0xFFFF;

/* int initFontMap(char fileName[8], char fileExt[3])
 *    Brief:
 *        Loads a Bitmap to use as a font into SDRAM
 *
 *    Populates:
 *        fFileSize 	= Size of file in bytes
 *        fBitmapSize 	= Size of data in bytes
 *        fBitmapPos 	= Byte position start
 *        fBitmapWidth 	= Width  in bits
 *        fBitmapHeight = Height in bits
 *
 *    Before use:
 *        CHARWIDTH 	- Width  of a Character
 *        CHARHEIGHT 	- Height of a Character
 *        FONTADDR		- SDRAM placement of font
 *
 *	    CHARWIDTH & CHARHEIGHT needed for actual
 *	    display functions
 *
 *    Summary:
 *        Loads the monochrome font bitmap into
 *        SDRAM at address FONTADDR. It also
 *        flps the bitmap so it is read top down
 *        versus bottom up. FONTADDR is needed
 *        before running.
 */
int initFontMap(char fileName[8], char fileExt[3])
{
	int ramIndex = 0;
	int i = 0;

	if (sdOpen(fileName, fileExt) != 0)
		return -1;

	if (sdBuffer[0] != 0x42 || sdBuffer[1] != 0x4D)
		return -2;
	else{
			fFileSize = grabWord(sdBuffer, 2);
			fBitmapPos  = grabWord(sdBuffer, 10);
			fBitmapSize = fFileSize - fBitmapPos;
			fBitmapHeight = grabWord(sdBuffer, 22);
			fBitmapWidth  = grabWord(sdBuffer, 18);


			for(i=fBitmapPos; i<bData._sectorSize; i+=2){
				IOWR(NEW_SDRAM_CONTROLLER_0_BASE, FONTADDR + (32*((fBitmapHeight) - ((ramIndex/32)+1))) + (ramIndex%32), ((sdBuffer[i])<<8)|(sdBuffer[i+1]) );
				ramIndex += 2;
			}
			while (sdReadNext() == 0){
				for(i=0;  i <bData._sectorSize && ramIndex < fBitmapSize ; i+=2){
					IOWR(NEW_SDRAM_CONTROLLER_0_BASE, FONTADDR + (32*((fBitmapHeight) - ((ramIndex/32)+1))) + (ramIndex%32), ((sdBuffer[i])<<8)|(sdBuffer[i+1]) );
					ramIndex += 2;
				}
			}
	}
	return 0;
}

/*   moveCursorFont(uint16_t x, uint16_t y, uint8_t page)
 *     Moves the cursor to x, y, page position
 */
void moveCursorFont(uint16_t x, uint16_t y, uint8_t page)
{
	cursorX = x;
	cursorY = y;
	cursorPage = page;
}

/*   changeColor(uint16_t color)
 *    Changes font color
 */
void changeColorFont(uint16_t color)
{
	fontColor = color;
}

/* uint16_t writeCharFont(unsigned char c)
 *     Displays char c to the screen
 *     based on the loaded bitmap.
 *     Returns the actual length of the
 *     char for kerning.
 */
uint16_t writeCharFont(unsigned char c)
{
	int address = FONTADDR +(((c-32)/16) *CHARHEIGHT*(fBitmapWidth/8)) + (((c-32)%16)*CHARWIDTH)/8 ;
	int frameAddr = (FRAMEADDRSIZE*cursorPage)+xyLocation(cursorX,cursorY);
	uint16_t kern = 0;
	uint16_t row;
	int i, j;

	for (i=0;i<CHARHEIGHT; i++){
		row = IORD( NEW_SDRAM_CONTROLLER_0_BASE, address + i*(fBitmapWidth/8) );
		for (j=0; j<CHARWIDTH; j++ ){
			if ( (row<<j ) &0x8000){
				IOWR(NEW_SDRAM_CONTROLLER_0_BASE, frameAddr + xyLocation(j,i), fontColor);
				if (j > kern)
					kern = j;
			}
		}
	}
	return kern;
}

/* int writeStringFont(char* str, uint8_t isKern)
 *     Writes a string to the screen using
 *     writeCharFont(). isKern determines
 *     whether to kern each character or
 *     not.
 *
 *     return:
 *         Returns number of chars
 *         outputed to the screen.
 */
int writeStringFont(char* str, uint8_t isKern)
{
	uint16_t kern;
	int i=0;

	while (str[i]){
		if (str[i] == '\n'){
			cursorX = 0;
			cursorY += CHARHEIGHT;
		}
		else if (str[i] == '\t'){
			cursorX += CHARWIDTH*4;
		}
		else{
			kern = writeCharFont((unsigned char)str[i]);
			if (kern > 0 && isKern)
				cursorX += kern;
			else
				cursorX += CHARWIDTH;
		}
		if((cursorX+CHARWIDTH) > ROWSIZE){
			cursorY += CHARHEIGHT;
			cursorX = 0;
		}
		++i;
	}
	return i;
}

/* int vprints(const char* fmt, uint8_t isKern, va_list args)
 *     Prints a string with a similar format as
 *     vprintf(). Supports Kerning based on isKern.
 *
 *     escape variables %:
 *         '%':	Display char '%'
 *         'd':	Display arg as a Decimal Int
 *         'x':	Display arg as a lowercase Hex int
 *         'X': Display arg as a uppercase Hex int
 *         'c': Display arg as a char
 *         's': Display arg as a string
 *
 *	   summary:
 *	       Accepts a list of variable args for display.
 *	       supports various escape characters using
 *	       '%' as its escape character. Uses
 *	       writeCharFont() and writeStringFont()
 *	       to display to the screen. If isKern is TRUE
 *	       spacing will be kerned for the string.
 *	       It is recommended to use printg() or
 *	       printk() for follow through use of this
 *	       function.
 *	   returns:
 *	       Returns total ammount of chars/bytes writen to
 *	       the screen.
 */
int vprints(const char* fmt, uint8_t isKern, va_list args)
{
	char buffer[30] = {'\0'};
	uint16_t length = 0;
	uint16_t kern;
	int re=0;
	int	d;

	while (*fmt){

		if ('%' == *fmt){
			if ('.' == *fmt){
				++fmt;
				while (*fmt >= 0x30 && *fmt <= 0x39){
					length *= 10;
					length += (*fmt)-0x30;
					++fmt;
				}
			}
			else
				++fmt;

			switch (*fmt){
				case '%':
					kern = writeCharFont((unsigned char)*fmt);
					++re;
					if (kern > 0 && isKern)
						cursorX += kern;
					else
						cursorX += CHARWIDTH;
					break;
				case 'd':
					d = va_arg( args, int);
					re += writeStringFont( itoa(d,buffer,10), isKern);
					break;
				case 'x':
					d = va_arg( args, int);
					re += writeStringFont( itoa(d,buffer,16), isKern);
					break;
				case 'X':
					d= va_arg(args, int);
					re += writeStringFont( itoa(d,buffer,116), isKern);
					break;
				case 'c':
					buffer[0] = va_arg( args, int);
					kern = writeCharFont((unsigned char)*buffer);
					++re;
					if (kern > 0 && isKern)
						cursorX += kern;
					else
						cursorX += CHARWIDTH;
					break;
				case 's':
					re += writeStringFont( va_arg( args, char*), isKern);
					break;
			 }
		}
		else{
			if (*fmt == '\n'){
				cursorX = 0;
				cursorY += CHARHEIGHT;
			}
			else if (*fmt == '\t'){
				cursorX += CHARWIDTH*4;
			}
			else{
				kern = writeCharFont((unsigned char)*fmt);
				if (kern > 0 && isKern)
					cursorX += kern;
				else
					cursorX += CHARWIDTH;
			}
			++re;
		}
		if((cursorX+CHARWIDTH) > ROWSIZE){
			cursorY += CHARHEIGHT;
			cursorX = 0;
		}
		++fmt;

	}
	return re;
}
/* printg(const char* fmt, ... )
 *	   Prints a string with a similar format as
 *     printf(). Variable args is used. Displays
 *     using grid spacing.
 *
 *     escape variables %:
 *         '%':	Display char '%'
 *         'd':	Display arg as a Decimal Int
 *         'x':	Display arg as a lowercase Hex int
 *         'X': Display arg as a uppercase Hex int
 *         'c': Display arg as a char
 *         's': Display arg as a string
 *
 *	   summary:
 *	       Accepts a list of variable args for display.
 *	       supports various escape characters using
 *	       '%' as its escape character. Uses vprints()
 *	       and sets kerning FALSE for grid spacing.
 *
 *	   returns:
 *	       Returns total ammount of chars/bytes writen to
 *	       the screen.
 */
int printg(const char* fmt, ... )
{
	va_list args;
	int re;
	va_start( args, fmt );
	re = vprints(fmt,0, args);
	va_end( args );
	return re;
}
/* printk(const char* fmt, ... )
 *	   Prints a string with a similar format as
 *     printf(). Variable args is used. Displays
 *     using grid spacing.
 *
 *     escape variables %:
 *         '%':	Display char '%'
 *         'd':	Display arg as a Decimal Int
 *         'x':	Display arg as a lowercase Hex int
 *         'X': Display arg as a uppercase Hex int
 *         'c': Display arg as a char
 *         's': Display arg as a string
 *
 *	   summary:
 *	       Accepts a list of variable args for display.
 *	       supports various escape characters using
 *	       '%' as its escape character. Uses vprints()
 *	       and sets kerning FALSE for kerned spacing.
 *
 *	   returns:
 *	       Returns total ammount of chars/bytes writen to
 *	       the screen.
 */
int printk(const char* fmt, ... )
{
	va_list args;
	int re;
	va_start( args, fmt );
	re = vprints(fmt,1, args);
	va_end( args );
	return re;
}


