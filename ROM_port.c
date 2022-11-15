#include <cms.h>

unsigned char eep_read(unsigned char read_address)
{
	EEADR = read_address;
	EEPGD = 0;
	RD = 1;
	asm("nop");
	asm("nop");
	return EEDAT;
}

void eep_write(unsigned char write_address, unsigned char write_data)
{
	EEADR = write_address;
	EEDAT = write_data;
	EEPGD = 0;
	WREN = 1;
	GIE = 0;
	EECON2 = 0x55;
	EECON2 = 0xaa;
	WR = 1;
	GIE = 1;
	while (WR == 1);
	WREN = 0;
}

char MTF_ROM_read(uint8_t *pBuffer, size_t ReadAddr, size_t NumByteToRead) //∂¡»°flash
{
	size_t i = 0;

	for (i < NumByteToRead; i++)
	{
		pBuffer[i] = eep_read(ReadAddr+i);
	}
	return 0;
}

char MTF_ROM_write(uint8_t *pBuffer, size_t WriteAddr, size_t NumByteToWrite) //–¥»Îflash
{
	size_t i = 0;

	for (i < NumByteToWrite; i++)
	{
		eep_write(WriteAddr, pBuffer[i]);
	}
	return 0;
}
