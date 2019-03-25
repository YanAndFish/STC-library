#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "hand.h"

#define MAX_TASK_NUM 5
#define RUN_ALL_TASK MAX_TASK_NUM

#define enableInterrupt()		EA=1;
#define disableInterrupt()	EA=0;

typedef struct
{
	unsigned char taskID;
	unsigned char runTaskID;
	void(*hander[MAX_TASK_NUM])();
} interruptTask;

extern xdata interruptTask INT0_INT,INT1_INT,INT2_INT,INT3_INT,INT4_INT,
TIME0_INT,TIME1_INT,TIME2_INT,TIME3_INT,TIME4_INT,
UART1_INT,UART2_INT,UART3_INT,UART4_INT,
ADC_INT,LVD_INT,PCA_INT,SPI_INT,CMP_INT,PWM_INT,PWMFD_INT,IIC_INT;


extern int setInterruptTask(interruptTask *intTask,unsigned char taskID,void(*hander)());
extern int setInterruptRunTask(interruptTask *intTask,unsigned char taskID);

#endif