
#include "bitmapSD.h"

static bitmapHeader bInfo;


int loadBitmap( const char fileName[8], const char fileExt[3], const uint32_t address, bitmapHeader* info)
{

	int ramIndex = 0;
	int bitmapRowLen = 0;
	int i = 0;

	if (sdOpen(fileName, fileExt) != 0)
		return -1;

	if (sdBuffer[0] != 0x42 || sdBuffer[1] != 0x4D)
		return -2;

	bInfo.fileSize = grabWord(sdBuffer, 2);
	bInfo.startPos  = grabWord(sdBuffer, 10);
	bInfo.size = bInfo.fileSize - bInfo.startPos;
	bInfo.height = grabWord(sdBuffer, 22);
	bInfo.width  = grabWord(sdBuffer, 18);
	bInfo.pixelResolution = grabHalf(sdBuffer, 28);

	/* Account for padding, half words per line*/
	bitmapRowLen = (((bInfo.pixelResolution*bInfo.width)+31)/32)*2;

	if (bInfo.pixelResolution == 1){


		for(i=bInfo.startPos; i<bData._sectorSize && ramIndex < (bInfo.size>>1); i+=2){
			IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1))) + (ramIndex%bitmapRowLen), ((sdBuffer[i])<<8)|(sdBuffer[i+1]) );
			ramIndex += 1;
		}
		while (sdReadNext() == 0){
			for(i=0;  i <bData._sectorSize && ramIndex < (bInfo.size>>1); i+=2){
				IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1))) + (ramIndex%bitmapRowLen), ((sdBuffer[i])<<8)|(sdBuffer[i+1]) );
				ramIndex += 1;
			}
		}
	}
	else if (bInfo.pixelResolution == 16){
		for(i=bInfo.startPos; i<bData._sectorSize && ramIndex < bInfo.size; i+=2){
					IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*(bInfo.height - ((ramIndex/bitmapRowLen)+1))) + (ramIndex%bitmapRowLen), grabHalf(sdBuffer,i) );
					ramIndex += 1;
				}
				while (sdReadNext() == 0){
					for(i=0;  i <bData._sectorSize && ramIndex < bInfo.size ; i+=2){
						IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1))) + (ramIndex%bitmapRowLen), grabHalf(sdBuffer,i) );
						ramIndex += 1;
					}
				}
	}
//	else if (bInfo.pixelResolution == 32){
//		for(i=bInfo.startPos; i<bData._sectorSize && ramIndex < bInfo.size; i+=4){
//			IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1))) + (ramIndex%bitmapRowLen), (grabWord(sdBuffer, i)>>16) );
//			IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1))+2) + (ramIndex%bitmapRowLen), (grabWord(sdBuffer, i)&0xFF) );
//			ramIndex += 4;
//		}
//		while (sdReadNext() == 0){
//			for(i=0;  i <bData._sectorSize && ramIndex < bInfo.size ; i+=4){
//				IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1))) + (ramIndex%bitmapRowLen), (grabWord(sdBuffer, i)>>16) );
//				IOWR(NEW_SDRAM_CONTROLLER_0_BASE, address + (bitmapRowLen*((bInfo.height) - ((ramIndex/bitmapRowLen)+1)+2)) + (ramIndex%bitmapRowLen), (grabWord(sdBuffer, i)&0xFF) );
//				ramIndex += 4;
//			}
//		}
//	}
	else{
		return - 3;
	}

	if (info != NULL)
		*info = bInfo;

	return 0;
}