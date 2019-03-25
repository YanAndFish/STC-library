#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__

#include "hand.h"

#define HIGH	1
#define LOW		0

#define ENABLE	1
#define DISABLE	0

#define ADC_return ADC_CON.AdcResult

typedef struct
{
	unsigned char AdcRun;//ADC电源
	unsigned char AdcInterrupt;//ADC中断
	unsigned char AdcPolity;//中断优先级
	unsigned int AdcResult;//ADC返回结果
	unsigned char AdcSpeed;//转换速度 0-15 / 0-3
} AdcConfig; 

extern xdata AdcConfig ADC_CON;

extern void ADC_read();
extern void ADC_begin(unsigned char inte);
extern int ADC_start(unsigned char port);
extern void ADC_setup();
extern void ADC_end();
extern void ADC_config(unsigned char AdcRun,unsigned char AdcInterrupt,unsigned char AdcPolity,unsigned char AdcSpeed);
extern int ADC_addTask(void (*hander)());

#endif