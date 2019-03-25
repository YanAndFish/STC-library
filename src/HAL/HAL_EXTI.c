#include "HAL_EXTI.h"
#include "interrupt.h"
#include "bitctrl.h"
#include "HAL_GPIO.h"

xdata Exti_config INT0_CON={1,0,1},INT1_CON={1,0,1},INT2_CON={1,0,1},INT3_CON={1,0,1},INT4_CON={1,0,1};

int EXTI_set(unsigned char exti,unsigned char mode,void (*hander)())
{
	switch(exti)
	{
		case 0:
			INT0_CON.ExtiMode=mode;
			EXTI_setup(Int0);
			EXTI_addTask(0,hander);
		break;
		case 1:
			INT1_CON.ExtiMode=mode;
			EXTI_setup(Int1);
			EXTI_addTask(1,hander);
		break;
		case 2:
			EXTI_setup(Int2);
			EXTI_addTask(2,hander);
		break;
		case 3:
			EXTI_setup(Int3);
			EXTI_addTask(3,hander);
		break;
		case 4:
			EXTI_setup(Int4);
			EXTI_addTask(4,hander);
			pinMode(37,OUTPUT);
		break;
		default:return -1;
	}
	EA=1;
	return 0;
}
int EXTI_addTask(unsigned char exti,void (*hander)())
{
	xdata int c=0;
	switch(exti)
	{
		case 0:
			c |= setInterruptTask(&INT0_INT,++INT0_INT.taskID,hander);
			break;
		case 1:
			c |= setInterruptTask(&INT1_INT,++INT1_INT.taskID,hander);
			break;
		case 2:
			c |= setInterruptTask(&INT2_INT,++INT2_INT.taskID,hander);
			break;
		case 3:
			c |= setInterruptTask(&INT3_INT,++INT3_INT.taskID,hander);
			break;
		case 4:
			c |= setInterruptTask(&INT4_INT,++INT4_INT.taskID,hander);
			break;
		default:return -1;
	}
	return c;
}
void EXTI_config(Exti_config *exti,	unsigned char	ExtiMode,unsigned char	ExtiPolity,unsigned char	ExtiInterrupt)
{
	exti->ExtiMode=ExtiMode;
	exti->ExtiPolity=ExtiPolity;
	exti->ExtiInterrupt=ExtiInterrupt;
}
int EXTI_setup(unsigned char exti)
{
	switch(exti)
	{
		case 0:
		if(INT0_CON.ExtiInterrupt == ENABLE)		EX0 = 1;	//允许中断
		else									EX0 = 0;	//禁止中断
		if(INT0_CON.ExtiPolity == HIGH)		PX0 = 1;	//高优先级中断
		else									PX0 = 0;	//低优先级中断
		if(INT0_CON.ExtiMode == EXTI_FALL)	IT0 = 1;	//下降沿中断
		else									IT0 = 0;	//上升,下降沿中断
		break;
		case 1:
		if(INT1_CON.ExtiInterrupt == ENABLE)		EX1 = 1;	//允许中断
		else									EX1 = 0;	//禁止中断
		if(INT1_CON.ExtiPolity == HIGH)		PX1 = 1;	//高优先级中断
		else									PX1 = 0;	//低优先级中断
		if(INT1_CON.ExtiMode == EXTI_FALL)	IT1 = 1;	//下降沿中断
		else									IT1 = 0;	//上升,下降沿中断
		break;
		case 2:
		if(INT2_CON.ExtiInterrupt == ENABLE)	INT_CLKO |=  (1 << 4);	//允许中断	
		else								INT_CLKO &= ~(1 << 4);	//禁止中断
		break;
		case 3:
		if(INT3_CON.ExtiInterrupt == ENABLE)	INT_CLKO |=  (1 << 5);	//允许中断	
		else								INT_CLKO &= ~(1 << 5);	//禁止中断
		break;
		case 4:
		if(INT4_CON.ExtiInterrupt == ENABLE)	INT_CLKO |=  (1 << 6);	//允许中断	
		else								INT_CLKO &= ~(1 << 6);	//禁止中断
		break;
		default:return -1;
	}
	return 0;
}
int EXTI_ON(unsigned char exti)
{
	switch(exti)
	{
		case 0:
			EX0=1;
			INT0_CON.ExtiInterrupt=1;
		break;
		case 1:
			EX1=1;
			INT1_CON.ExtiInterrupt=1;
		break;
		case 2:
			INT_CLKO |=  (1 << 4);
			INT2_CON.ExtiInterrupt=1;
		break;
		case 3:
			INT_CLKO |=  (1 << 5);
			INT3_CON.ExtiInterrupt=1;
		break;
		case 4:
			INT_CLKO |=  (1 << 6);
			INT4_CON.ExtiInterrupt=1;
		break;
		default:return -1;
	}
	EA=1;
	return 0;
}
int EXTI_OFF(unsigned char exti)
{
		switch(exti)
	{
		case 0:
			EX0=0;
			INT0_CON.ExtiInterrupt=0;
		break;
		case 1:
			EX1=0;
			INT1_CON.ExtiInterrupt=0;
		break;
		case 2:
			INT_CLKO &= ~(1 << 4);
			INT2_CON.ExtiInterrupt=0;
		break;
		case 3:
			INT_CLKO &= ~(1 << 5);
			INT3_CON.ExtiInterrupt=0;
		break;
		case 4:
			INT_CLKO &= ~(1 << 6);
			INT4_CON.ExtiInterrupt=0;
		break;
		default:return -1;
	}
	return 0;
}