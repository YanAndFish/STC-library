#ifndef __HAL_PCA_H__
#define __HAL_PCA_H__

#include "hand.h"

#define PCA_1			4
#define PCA_2			1
#define PCA_4			5
#define PCA_6			6
#define PCA_8			7
#define PCA_12		0
#define PCA_Time0	2
#define PCA_Eci		3

#define PCA_RISE			0
#define PCA_FALL			1
#define PCA_RISE_FALL	2
#define PCA_TIME			3
#define PCA_TOG				4

#define ENABLE		1
#define DISABLE		0

#define HIGH	1
#define LOW		0

#define ON	1
#define OFF	0

typedef struct
{
	unsigned char	PcaClock;	//时钟
	unsigned char	PcaInterrupt;	//中断开关
	unsigned char	PcaPolity;	//优先级设置
	unsigned char PcaRun;//运行
} PcaConfig;



extern xdata PcaConfig PCA_CON;

extern int PCA_begin(unsigned char port,unsigned char mode,unsigned int value,unsigned char inte);
extern void PCA_end();
extern void PCA_config(unsigned char	PcaClock,unsigned char	PcaInterrupt,unsigned char	PcaPolity,unsigned char PcaRun);
extern void PCA_setup();
extern int PCA_addTask(void (*hander)());
extern int PCA_tog(unsigned char port,unsigned char onoff);
			
#endif