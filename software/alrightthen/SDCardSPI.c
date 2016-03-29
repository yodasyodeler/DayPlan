#include "SDCardSPI.h"

int sd_isReady()
{
	uint32_t data;
	data = IORD(SPI_AVALON_SD_0_BASE, SD_RD_DATA);

	return ((data & 0x100) >> 8);
}
void sd_wrCS(uint8_t cs)
{
	IOWR(SPI_AVALON_SD_0_BASE, SD_CS, cs);
}
void sd_wrDvsr(uint8_t dvsr)
{
	IOWR(SPI_AVALON_SD_0_BASE, SD_DVSR, dvsr);
}
void sd_wrByte(uint8_t data)
{
	while (!(sd_isReady()));

	IOWR(SPI_AVALON_SD_0_BASE, SD_WR_DATA, data);
}
uint8_t sd_rdByte()
{
	uint8_t data;

	sd_wrByte(0xff);
	data = IORD(SPI_AVALON_SD_0_BASE, SD_RD_DATA);

	return (data & 0xff);
}

uint8_t sd_wrCmd(uint8_t cmd, uint32_t arg, uint32_t *r3)
{
	int i;
	uint8_t crc, rcode;
	uint32_t ocr = 0;

	if (cmd == SD_CMD0_RESET)
		crc = 0x95;		//crc for CMD0(0)
	else
		crc = 0x87;		//crc for CMD8(0x1aa)

	/* send command */
	sd_wrByte(cmd|0x40); //MSB is 01

	/* send argument */
	sd_wrByte((uint8_t)(arg>>24));
	sd_wrByte((uint8_t)(arg>>16));
	sd_wrByte((uint8_t)(arg>>8));
	sd_wrByte((uint8_t)(arg));

	/* send crc, only valid for CMD0, CMD8 */
	sd_wrByte(crc);

	/* read response */
	rcode = 0xff;
	for (i=0; i<8 && (rcode == 0xff); ++i){
		rcode = sd_rdByte();
	}

	/* read 4 additional bytes for CMD8 and CMD58 */
	if ((cmd==SD_CMD8_SEND_IF || cmd==SD_CMD58_RD_OCR)){
		for (i=0; i<4; ++i){
			ocr = (ocr<<8| sd_rdByte());
		}
		*r3 = ocr;
	}
	return rcode;
}

int sd_init()
{
	int i;
	uint8_t rcode;
	uint32_t ocr;
	int hcs;

	sd_wrDvsr(250);			//set sclk to 200kHz
	sd_wrCS(1);
	for(i=0; i<250; ++i){
		sd_wrByte(0xff);
	}

	sd_wrCS(0);
	rcode = sd_wrCmd(SD_CMD0_RESET, 0, &ocr);
	if (rcode!=0x01){
		printf("CMD0 command fails: R1=0x%02x \n:", rcode);
		return -1;
	}

	rcode = sd_wrCmd(SD_CMD8_SEND_IF, 0x000001aa, &ocr);
	if (rcode!=0x01 || ocr!=0x000001aa){
		printf("CMD8 command fails: R1=0x%02x \n:", rcode);
		return -1;
	}

	for(i=0; i<SD_INIT_TIME_OUT; ++i){
		sd_wrCmd(SD_CMD55_APP, 0, &ocr);
		rcode = sd_wrCmd(SD_ACMD41_INIT_SD, 0x40000000, &ocr);
		if (rcode==0x00)
			break;
	}
	if (rcode != 0x00){
		printf("ACMD41 command fails: R1=0x%02x\n", rcode);
		return -1;
	}

	rcode = sd_wrCmd(SD_CMD58_RD_OCR, 0x000001aa, &ocr);
	if (rcode != 0x00){
		printf("CMD58 command fails: R1=0x%02x \n", rcode);
		return -1;
	}

	hcs = (ocr & 0x40000000) >>30;

	sd_wrDvsr(75);

	return hcs;
}
int sd_readSector(int sdhc, uint32_t sect, uint8_t *buf)
{
	int i;
	uint8_t  rcode, token = 0;
	uint32_t addr, ocr;

	if (sdhc==0)
		addr = (sect<<9);
	else
		addr = sect;

	rcode = sd_wrCmd(SD_CMD17_READ, addr, &ocr);
	if (rcode != 0x00){
		printf("CMD17 command fails: R1=0x%02x \n", rcode);
		return -1;
	}

	for (i=0; (i<SD_READ_TIME_OUT) && (token!=SD_TOKEN_DATA_START); ++i){
		token = sd_rdByte();
	}
	if (token!=SD_TOKEN_DATA_START){
		printf("No data start token: last token=0x%02x\n", token);
		return -1;
	}

	for (i=0; i<512; ++i)
		buf[i] = sd_rdByte();

	/* discard two crc bytes */
	sd_rdByte();
	sd_rdByte();

	return 0;
}
int sd_writeSector(int sdhc, uint32_t sect, uint8_t *buf)
{
	int i;
	uint8_t  rcode, token;
	uint32_t addr, dummy;

	if (sdhc==0)
		addr = (sect<<9);
	else
		addr = sect;

	rcode = sd_wrCmd(SD_CMD24_WRITE, addr, &dummy);
	if (rcode != 0x00){
		printf("CMD24 command fails: R1=0x%02x \n", rcode);
		return -1;
	}

	sd_wrByte(SD_TOKEN_DATA_START);

	for (i=0; i<512; ++i)
		sd_wrByte(buf[i]);

	sd_wrByte(0xff);
	sd_wrByte(0xff);

	for (i=0; i<SD_WRITE_TIME_OUT; ++i){
		token = ((sd_rdByte())&0x1f);
		if (token==SD_TOKEN_DATA_ACCEPT)
			break;
	}
	if (token != SD_TOKEN_DATA_ACCEPT){
		printf("No data accept token: last token=0x%02x \n", token);
		return -1;
	}

	for (i=0; i<SD_WRITE_TIME_OUT; ++i){
		token = sd_rdByte();
		if (token==0xff)
			break;
	}
	if (token != 0xff){
		printf("Write completion timeout: last token=0x%02x \n", token);
		return -1;
	}

	return 0;
}
