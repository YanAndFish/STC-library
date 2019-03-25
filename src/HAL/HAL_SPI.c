#include "HAL_SPI.h"
#include "interrupt.h"
#include "bitctrl.h"
#include "HAL_GPIO.h"

xdata Spi_config SPI_CON={0,0,2,0,0};

void SPI_begin()
{
	SPDAT=0;
	SPI_setup();
	if(SPI_CON.SpiInterrupt==1)
		setInterruptTask(&SPI_INT,++SPI_INT.taskID,SPI_cb);
	SPCTL sl setHigh[6];
}

void SPI_end()
{
	SPCTL sl setLow[6];
}

void SPI_send(SPIslave *sla,unsigned char dat)
{
	SPI_CON.Spiresult = sla;
	writePin(sla->pin,LOW);
	SPDAT=dat;
	if(SPI_CON.SpiInterrupt==1)
		return;
	while(!(SPSTAT & 0x80));
	SPSTAT sh setHigh[6];
	SPSTAT sh setHigh[7];
	SPI_CON.Spiresult->result=SPDAT;
	writePin(SPI_CON.Spiresult->pin,HIGH);
}

void SPI_setup()
{
	if(SPI_CON.SpiPolity==1)
		IP2 sh setHigh[1];
	else
		IP2 sl setLow[1];
	if(SPI_CON.SpiInterrupt==1)
		IE2 sh setHigh[1];
	else
		IE2 sl setLow[1];
	if(SPI_CON.SpiClockMode==1)
		SPCTL sh setHigh[3];
	else
		SPCTL sl setLow[3];
	if(SPI_CON.SpiSendMode==1)
		SPCTL sh setHigh[5];
	else
		SPCTL sl setLow[5];
	SPCTL sl setLow[0];
	SPCTL sl setLow[1];
	SPCTL |= SPI_CON.SpiClock;
	
	SPCTL sh setHigh[7];
	SPCTL sh setHigh[4];
}

void SPI_config(unsigned char	SpiPolity,		//优先级设置	PolityHigh,PolityLow
								unsigned char	SpiInterrupt,		//中断允许		ENABLE,DISABLE
								unsigned char SpiClock,
								unsigned char SpiClockMode,
								unsigned char SpiSendMode)
{
	SPI_CON.SpiPolity=SpiPolity;
	SPI_CON.SpiInterrupt=SpiInterrupt;
	SPI_CON.SpiClock=SpiClock;
	SPI_CON.SpiClockMode=SpiClockMode;
	SPI_CON.SpiSendMode=SpiSendMode;
}

void SPI_slaveBegin(SPIslave *sla,unsigned char pin)
{
	sla->pin=pin;
	pinMode(sla->pin,OUTPUT);
	writePin(sla->pin,HIGH);
}

void SPI_cb()
{
	//SPSTAT sh setHigh[6];
	//SPSTAT sh setHigh[7];
	SPI_CON.Spiresult->result=SPDAT;
	writePin(SPI_CON.Spiresult->pin,HIGH);
}