#ifndef FAT16SD_H_
#define FAT16SD_H_
#include <stdio.h>
#include <system.h>
#include <io.h>
#include <stdint.h>
#include "SDCardSPI.h"
#include <string.h>

typedef struct fat16_boot {
	uint8_t		_sdhc;			//type of SD card
	uint16_t	_fatAddr;		//starting sector
	uint16_t	_rdirAddr;		//starting sector
	uint16_t	_dataAddr;		//starting sector
	uint8_t		_clusterSize;	//num of sectors per Cluster
	uint16_t	_rdirSize;		//num of files   For Directory
	uint16_t	_sectorSize;	//size of sectors
	uint16_t 	_fatSize;
} fat16Boot;

typedef struct fat16_file {
	char 		_fileName[9];
	char 		_fileExt[4];

	uint32_t 	_clusterAddr;	//Cluster starting address
	uint32_t	_fileSize;		//File size in Bytes

	uint32_t	_seek;			//Sector index position within cluster
	uint32_t	_SectorIndex;	//Sector index of the file
	uint32_t	_rdirIndex;		//File index position within rdir
	uint32_t	_fatNextIndex;	//Sector index position for next fat
	uint32_t	_fatCurrentIndex;

	uint8_t		_isOpen;
} fat16File;

#define grabWord(x,y) ((x[y])|(x[y+1]<<8)|(x[y+2]<<16)|(x[y+3]<<24))
#define grabHalf(x,y) ((x[y])|(x[y+1]<<8))


int sdMount();
int sdOpen(char fileName[8], char fileExt[3]);
int sdReadNext();


extern uint8_t sdBuffer[512];
extern fat16Boot bData;
extern fat16File file;


#endif
