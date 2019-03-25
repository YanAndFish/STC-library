#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include "hand.h"

#define	Timer0						0
#define	Timer1						1
#define	Timer2						2
#define	Timer3						3
#define	Timer4						4

#define	Time_16BitAuto				0
#define	Time_16Bit						1
#define	Time_8BitAuto					2
#define	Time_16BitAutoNoMask	3

#define	CLOCK_1T				0
#define	CLOCK_12T				1
#define	CLOCK_Ext				2

#define HIGH	1
#define LOW		0

#define ENABLE 1
#define DISABLE 0

typedef struct
{
	unsigned char	TimeMode;				//工作模式,  	TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	unsigned char	TimePolity;			//优先级设置	High,Low
	unsigned char	TimeInterrupt;	//中断允许		ENABLE,DISABLE
	unsigned char	TimeClkSource;	//时钟源		TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	unsigned char	TimeClkOut;			//可编程时钟输出,	ENABLE,DISABLE
	unsigned int	TimeValue;			//装载初值
	unsigned char	TimeRun;				//是否运行		ENABLE,DISABLE
} Time_config;

extern xdata Time_config TIME0_CON,TIME1_CON,TIME2_CON,TIME3_CON,TIME4_CON;

extern void TIME0_CB();
extern void TIME1_CB();
extern void TIME2_CB();
extern void TIME3_CB();
extern void TIME4_CB();
extern int TIMER_set(unsigned char timer,unsigned int ms,void(*callback)());
extern int TIMER_start(unsigned char timer);
extern int TIMER_end(unsigned char timer);
extern int TIMER_addTask(unsigned char timer,void(*hander)());
extern int TIMER_setup(unsigned char timer);
extern void TIMER_config(Time_config *time,unsigned char	TimeMode,unsigned char	TimePolity,unsigned char	TimeInterrupt,	
unsigned char	TimeClkSource,unsigned char	TimeClkOut,unsigned int	TimeValue,unsigned char	TimeRun);
	
#endif