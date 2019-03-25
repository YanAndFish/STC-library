#ifndef __HAL_EXTI_H__
#define __HAL_EXTI_H__

#include "hand.h"

#define	Int0			0	//初始化外中断0
#define	Int1			1	//初始化外中断1
#define	Int2			2	//初始化外中断2
#define	Int3			3	//初始化外中断3
#define	Int4			4	//初始化外中断4

#define	EXTI_RISE_FALL	0	//上升沿/下降沿中断
#define	EXTI_FALL			1	//下降沿中断

#define ENABLE 1
#define DISABLE 0

#define HIGH	1
#define LOW		0

typedef struct
{
	unsigned char	ExtiMode;			//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	unsigned char	ExtiPolity;		//优先级设置	PolityHigh,PolityLow
	unsigned char	ExtiInterrupt;		//中断允许		ENABLE,DISABLE
} Exti_config;

extern xdata Exti_config INT0_CON,INT1_CON,INT2_CON,INT3_CON,INT4_CON;

extern int EXTI_set(unsigned char exti,unsigned char mode,void (*hander)());
extern int EXTI_addTask(unsigned char exti,void (*hander)());
extern void EXTI_config(Exti_config *exti,	unsigned char	ExtiMode,unsigned char	ExtiPolity,unsigned char	ExtiInterrupt);
extern int EXTI_setup(unsigned char exti);
extern int EXTI_ON(unsigned char exti);
extern int EXTI_OFF(unsigned char exti);

#endif