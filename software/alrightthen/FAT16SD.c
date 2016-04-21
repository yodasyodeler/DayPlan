#include "FAT16SD.h"

uint8_t sdBuffer[512];
fat16Boot bData;
fat16File file;

/* sdMount
 *   initializes sd card through spi
 *   Populates Fat16Boot data
 */
int sdMount() {
	uint32_t bootRecordAddr = 0;

	bData._sdhc = sd_init();
	if (bData._sdhc == -1){
		printf("SD card initialization failed \n");
		return -1;
	}

	if (sd_readSector(bData._sdhc, 0 ,sdBuffer) != 0){
		printf("Sector 0 read failed\n");
		return 2;
	}

	if ( (sdBuffer[0] != 0xEB) && (sdBuffer[0] != 0xE9) ){
		if ((sdBuffer[510] != 0x55) || (sdBuffer[511] != 0xAA)){
			printf("MBR signature not matched\n");
			return 3;
		}

		if ( (sdBuffer[0x1C2]!=0x04) && (sdBuffer[0x1C2]!=0x06) && (sdBuffer[0x1C2]!=0x0e) ){
			printf("FAT16 file type not match\n");
			return 4;
		}

		bootRecordAddr = grabWord(sdBuffer, 0x1C6);
		if ( sd_readSector(bData._sdhc,bootRecordAddr,sdBuffer) != 0 ){
			printf("Boot record read failed\n");
			return 5;
		}
	}
	if ( (sdBuffer[510]!= 0x55) || (sdBuffer[511]!=0xaa)){
		printf ("Boot record signature not match\n");
		return 6;
	}

	bData._sectorSize	= grabHalf(sdBuffer, 0x0B);
	bData._clusterSize 	= sdBuffer[0x0D];
	bData._rdirSize	   	= grabHalf(sdBuffer, 0x11);
	bData._fatSize		= grabHalf(sdBuffer, 0x16);
	bData._fatAddr     	= bootRecordAddr  +  grabHalf(sdBuffer, 0x0E);
	bData._rdirAddr    	= bData._fatAddr  + (bData._fatSize * (sdBuffer[0x10]) );
	bData._dataAddr    	= bData._rdirAddr + ((bData._rdirSize*32)/512);

	return 0;
}

/* sdOpen
 *   Attempts to find a file and then
 *
 */
int sdOpen(const char fileName[8], const char fileExt[3]){
	uint8_t cont = 1;
	file._isOpen = 0;
	int i;
	int j;


	for (i=0; i<(bData._rdirSize/16) && cont; ++i){
		if (sd_readSector(bData._sdhc, bData._rdirAddr+i, sdBuffer) != 0){
			printf("SD directory read failed \n");
			return -1;
		}
		for (j=0; j<16 && cont; ++j){
			if ( !strncmp((char*)&(sdBuffer[(j*32)])  , fileName, 8) &&
				 !strncmp((char*)&(sdBuffer[(j*32)+8]), fileExt , 3) )
				cont = 0;
		}
	}
	j-=1;
	i-=1;

	if (cont != 0) {
		printf("File not found!");
		return 1;
	}

	strncpy(file._fileName, fileName, 9);
	strncpy(file._fileExt, fileExt, 4);
	file._clusterAddr = grabHalf(sdBuffer,0x1A+(j*32));
	file._fatCurrentIndex = file._clusterAddr;
	file._fileSize = grabWord(sdBuffer,0x1C+(j*32));

	if (sd_readSector(bData._sdhc, bData._fatAddr+(file._clusterAddr/256), sdBuffer) != 0){
		printf("SD Fat read failed \n");
		return -1;
	}

	file._fatNextIndex = grabHalf(sdBuffer, file._clusterAddr*2);
	file._rdirIndex = j + i*16;
	file._seek = 0;
	file._isOpen = 1;
	file._SectorIndex = 0;

	if (sd_readSector(bData._sdhc, bData._dataAddr + ((file._clusterAddr-2)*bData._clusterSize), sdBuffer) != 0){
		printf("SD Fat read failed \n");
		return -1;
	}

	return 0;
}
int sdReadNext(){
	uint32_t current;
	file._SectorIndex +=1;

	file._seek += 1;

	if (!file._isOpen)
	{
		printf("File is not open\n");
		return -1;
	}

	if (file._seek >= bData._clusterSize || (file._SectorIndex*bData._sectorSize) >= file._fileSize )
	{
		if (file._fatNextIndex == 0xffff)
		{
			file._isOpen = 0;
			return 1;
		}

		file._seek = 0;
		if (sd_readSector(bData._sdhc, bData._fatAddr+(file._fatNextIndex/256), sdBuffer) != 0){
			printf("SD Fat read failed \n");
			return -1;
		}
		current = file._fatNextIndex;
		file._fatNextIndex = grabHalf(sdBuffer, current*2);
		file._fatCurrentIndex = current;
	}

	if (sd_readSector(bData._sdhc, bData._dataAddr + file._seek + ((file._fatCurrentIndex-2)*bData._clusterSize), sdBuffer) != 0){
		printf("SD Fat read failed \n");
		return -1;
	}


	return 0;
}






