#ifndef __HAL_PWM_H__
#define __HAL_PWM_H__

#include "hand.h"

#define PWM_VALUE 1000//����

#define pwmTimer2 	16
#define sysTimer(x)	x-1

#define HIGH	1
#define LOW		0

#define ENABLE	1
#define DISABLE	0

#define INT_HIGH	2
#define INT_LOW		1
#define	INT_ALL		3
#define INT_TIMER	4


typedef struct
{
	unsigned char PwmRun;//�Ƿ�����PWM��ʱ��
	//unsigned int PwmValue;//��ʱ����ֵ
	unsigned char PwmClkSource;//ʱ��Դ Timer2 or system clock
	unsigned char PwmInterrupt;//�Ƿ�ʹ�ܶ�ʱ���ж�
	unsigned char PwmPolity;//�ж����ȼ�
	unsigned char PwmPort;
} PwmConfig;

typedef struct
{
	unsigned int PwmHighPort;//��תΪ�ߵĵ�(2)
	unsigned int PwmLowPort;//��תΪ�͵ĵ�(1)
} PwmPortConfig;

extern xdata PwmConfig PWM_CON;

#ifdef STC8
extern xdata PwmPortConfig PWM_0,PWM_1,PWM_2,PWM_3,PWM_4,PWM_5,PWM_6,PWM_7;
#endif

#ifdef STC15
extern xdata PwmPortConfig PWM_2,PWM_3,PWM_4,PWM_5,PWM_6,PWM_7;
#endif

extern int PWM_begin(unsigned char port,unsigned int duty,unsigned char pwmint);
extern int PWM_end(unsigned char port);
extern int PWM_set(unsigned char port,unsigned int duty);
extern void PWM_config(unsigned char PwmRun,unsigned char PwmClkSource,unsigned char PwmInterrupt,unsigned char PwmPolity);
extern int PWM_setup();
extern int PWM_addTask(void(*hander)());
extern unsigned int PWM_persent(unsigned long persent);

#endif