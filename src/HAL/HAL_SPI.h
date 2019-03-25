#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include "hand.h"

#define SPI_CLK_4		0
#define SPI_CLK_8		1
#define SPI_CLK_16	2
#define SPI_CLK_32	3

#define SPI_RISE	0
#define SPI_FALL	1

#define SPI_MSBF	0
#define SPI_LSBF	1

#define HIGH	1
#define LOW		0

typedef struct
{
	unsigned char pin;
	unsigned char result;
} SPIslave;

typedef struct
{
	unsigned char	SpiPolity;		//优先级设置	PolityHigh,PolityLow
	unsigned char	SpiInterrupt;		//中断允许		ENABLE,DISABLE
	unsigned char SpiClock;
	unsigned char SpiClockMode;
	unsigned char SpiSendMode;
	SPIslave *Spiresult;
} Spi_config;

extern xdata Spi_config SPI_CON;

extern void SPI_setup();
extern void SPI_cb();
extern void SPI_begin();
extern void SPI_end();
extern void SPI_send(SPIslave *sla,unsigned char dat);
extern void SPI_slaveBegin(SPIslave *sla,unsigned char pin);
extern void SPI_config(unsigned char	SpiPolity,		//优先级设置	PolityHigh,PolityLow
								unsigned char	SpiInterrupt,		//中断允许		ENABLE,DISABLE
								unsigned char SpiClock,
								unsigned char SpiClockMode,
								unsigned char SpiSendMode);

#endif