#include "core.h"

SPIslave moha;//make SPI object

void setup()
{
	SPI_slaveBegin(&moha,14);//setup the SPI object SS pin
	SPI_begin();//begin SPI
}

void loop()
{
	unsigned char SPIdata;//SPI object return data
	
	SPI_send(&moha,0x12);//sned SPI data
	SPIdata=moha.result;//get data
	
	delay(5000);
	
	//SPI_end();//close SPI
}