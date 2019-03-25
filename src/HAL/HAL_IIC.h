#ifndef __HAL_IIC_H__
#define __HAL_IIC_H__

#include "hand.h"

sbit SDA=P1^4;
sbit SCL=P1^5;

#ifdef STC8

#define ENABLE	1
#define DISABLE	0

#define IIC_MASTER	0
#define IIC_SALVE		1

typedef struct
{
	unsigned char IIC_interrupt;//中断使能
	unsigned char IIC_mode;//模式
	unsigned char IIC_busy;
} IicConfig;

extern xdata IicConfig IIC_CON;

extern int IIC_begin(unsigned char mode,unsigned char inte,unsigned char addr);
extern void IIC_wait();
extern void IIC_Start();
extern void IIC_write(char dat);
extern void IIC_readAsk();
extern char IIC_read();
extern void IIC_writeAsk();
extern void IIC_writeNoAsk();
extern void IIC_stop();
extern int IIC_addTask(void(*hander)());


#endif

#ifdef STC15

#define ENABLE	1
#define DISABLE	0

extern void IIC_Start();
extern void IIC_Stop();
extern int IIC_Write(unsigned char dat);
extern unsigned char IIC_read();
extern void IIC_writeNoAsk();
extern void IIC_writeAsk();
extern void IIC_wait();

#endif

#endif