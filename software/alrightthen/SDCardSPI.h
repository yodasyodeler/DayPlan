#ifndef SDCARDSPI_H_
#define SDCARDSPI_H_
#include <stdio.h>
#include <stdint.h>
#include <system.h>
#include <io.h>

/*	 SD SPI data regs 	*/
#define SD_RD_DATA	(0)
#define SD_CS		(1)
#define SD_DVSR		(2)
#define SD_WR_DATA	(3)

/*	 SD SPI commands	*/
#define SD_CMD0_RESET			(0)
#define SD_CMD8_SEND_IF			(8)
#define SD_CMD17_READ			(17)
#define SD_CMD24_WRITE			(24)
#define SD_CMD55_APP			(55)
#define SD_CMD58_RD_OCR			(58)
#define SD_ACMD41_INIT_SD		(41)

/*	 SD SPI card token	*/
#define SD_TOKEN_DATA_START		(0xfe)
#define SD_TOKEN_DATA_ACCEPT	(0x05)

/* SD SPI timeout cycles */
#define SD_INIT_TIME_OUT 		(900)		// 0.50s w/ 200kHz sclk
#define SD_READ_TIME_OUT		(312500)	// 0.10s w/  25MHz sclk
#define SD_WRITE_TIME_OUT		(781250)	// 0.25s w/  25MHz sclk

/*	   SPI Commands 	*/
int sd_isReady();
void sd_wrCS(uint8_t cs);
void sd_wrDvsr(uint8_t dvsr);
void sd_wrByte(uint8_t data);
uint8_t sd_rdByte();

/*	SD Driver Functions */
uint8_t sd_wrCmd(uint8_t cmd, uint32_t arg, uint32_t *r3);

int sd_init();
int sd_readSector(int sdhc, uint32_t sect, uint8_t *buf);
int sd_writeSector(int sdhc, uint32_t sect, uint8_t *buf);

#endif /* SDCARDSPI_H_ */
