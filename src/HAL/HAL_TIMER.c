#include "HAL_TIMER.h"
#include "interrupt.h"
#include "bitctrl.h"

xdata Time_config TIME0_CON={0,0,1,0,0,41536,0},TIME1_CON={0,0,1,0,0,41536,0},TIME2_CON={0,0,1,0,0,41536,0},TIME3_CON={0,0,1,0,0,41536,0},TIME4_CON={0,0,1,0,0,41536,0};

xdata unsigned int TIME_CLOCK[5];
xdata void(*TIMEx_CB[5])();

int TIMER_set(unsigned char timer,unsigned int ms,void(*callback)())
{
	switch(timer)
	{
		case Timer0:
			setInterruptTask(&TIME0_INT,++TIME0_INT.taskID,TIME0_CB);
			TIME_CLOCK[0]=ms;
			TIMEx_CB[0]=callback;
			TIMER_setup(Timer0);
			break;
		case Timer1:
			setInterruptTask(&TIME1_INT,++TIME1_INT.taskID,TIME1_CB);
			TIME_CLOCK[1]=ms;
			TIMEx_CB[1]=callback;
			TIMER_setup(Timer1);
			break;
		case Timer2:
			setInterruptTask(&TIME2_INT,++TIME2_INT.taskID,TIME2_CB);
			TIME_CLOCK[2]=ms;
			TIMEx_CB[2]=callback;
			TIMER_setup(Timer2);
			break;
		case Timer3:
			setInterruptTask(&TIME3_INT,++TIME3_INT.taskID,TIME3_CB);
			TIME_CLOCK[3]=ms;
			TIMEx_CB[3]=callback;
			TIMER_setup(Timer3);
			break;
		case Timer4:
			setInterruptTask(&TIME4_INT,++TIME4_INT.taskID,TIME4_CB);
			TIME_CLOCK[4]=ms;
			TIMEx_CB[4]=callback;
			TIMER_setup(Timer4);
			break;
		default:
			return -1;
	}
	return 0;
}

int TIMER_start(unsigned char timer)
{
	switch(timer)
	{
		case 0:
			TR0=1;
			TIME0_CON.TimeRun=1;
			break;
		case 1:
			TR1=1;
			TIME1_CON.TimeRun=1;
			break;
		case 2:
			AUXR sh setHigh[4];
			TIME2_CON.TimeRun=1;
			break;
		case 3:
			T4T3M sh setHigh[3];
			TIME3_CON.TimeRun=1;
			break;
		case 4:
			T4T3M sh setHigh[7];
			TIME4_CON.TimeRun=1;
			break;
		default:
			return -1;
	}
	EA=1;
	return 0;
}

int TIMER_end(unsigned char timer)
{
		switch(timer)
	{
		case 0:
			TR0=0;
			TIME0_CON.TimeRun=0;
			break;
		case 1:
			TR1=0;
			TIME1_CON.TimeRun=0;
			break;
		case 2:
			AUXR sl setLow[4];
			TIME2_CON.TimeRun=0;
			break;
		case 3:
			T4T3M sl setLow[3];
			TIME3_CON.TimeRun=0;
			break;
		case 4:
			T4T3M sl setLow[7];
			TIME4_CON.TimeRun=0;
			break;
		default: return -1;
	}
	return 0;
}

int TIMER_addTask(unsigned char timer,void(*hander)())
{
	xdata int c=0;
	switch(timer)
	{
		case 0:
			c |= setInterruptTask(&TIME0_INT,++TIME0_INT.taskID,hander);
			break;
		case 1:
			c |= setInterruptTask(&TIME1_INT,++TIME1_INT.taskID,hander);
			break;
		case 2:
			c |= setInterruptTask(&TIME2_INT,++TIME2_INT.taskID,hander);
			break;
		case 3:
			c |= setInterruptTask(&TIME3_INT,++TIME3_INT.taskID,hander);
			break;
		case 4:
			c |= setInterruptTask(&TIME4_INT,++TIME4_INT.taskID,hander);
			break;
		default:return -1;
	}
	return c;
}
void TIMER_config(Time_config *time,
	unsigned char	TimeMode,				
	unsigned char	TimePolity,
	unsigned char	TimeInterrupt,	
	unsigned char	TimeClkSource, 
	unsigned char	TimeClkOut,		
	unsigned int	TimeValue,
	unsigned char	TimeRun)
{
	time->TimeMode=TimeMode;
	time->TimePolity=TimePolity;
	time->TimeInterrupt=TimeInterrupt;
	time->TimeClkSource=TimeClkSource;
	time->TimeClkOut=TimeClkOut;
	time->TimeValue=TimeValue;
	time->TimeRun=TimeRun;
}

int TIMER_setup(unsigned char timer)
{
	switch(timer)
	{
		case 0:
			TR0 = 0;		//停止计数
			if(TIME0_CON.TimeInterrupt == ENABLE)		ET0 = 1;	//允许中断
			else									ET0 = 0;	//禁止中断
			if(TIME0_CON.TimePolity == HIGH)		PT0 = 1;	//高优先级中断
			else									PT0 = 0;	//低优先级中断
			if(TIME0_CON.TimeMode >  Time_16BitAutoNoMask)	return -1;	//错误
			TMOD = (TMOD & ~0x03) | TIME0_CON.TimeMode;	//工作模式,0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
			if(TIME0_CON.TimeClkSource == CLOCK_12T)	AUXR &= ~0x80;	//12T
			if(TIME0_CON.TimeClkSource == CLOCK_1T)		AUXR |=  0x80;	//1T
			if(TIME0_CON.TimeClkSource == CLOCK_Ext)	TMOD |=  0x04;	//对外计数或分频
			else																			TMOD &= ~0x04;	//定时
			if(TIME0_CON.TimeClkOut == ENABLE)				INT_CLKO |=  0x01;	//输出时钟
			else																			INT_CLKO &= ~0x01;	//不输出时钟
			TH0 = (unsigned char)(TIME0_CON.TimeValue >> 8);
			TL0 = (unsigned char)TIME0_CON.TimeValue;
			if(TIME0_CON.TimeRun == ENABLE)	TR0 = 1;	//开始运行
			break;
		case 1:
			TR1 = 0;		//停止计数
			if(TIME1_CON.TimeInterrupt == ENABLE)		ET1 = 1;	//允许中断
			else									ET1 = 0;	//禁止中断
			if(TIME1_CON.TimePolity == HIGH)		PT1 = 1;	//高优先级中断
			else									PT1 = 0;	//低优先级中断
			if(TIME1_CON.TimeMode >  Time_16BitAutoNoMask)	return -1;	//错误
			TMOD = (TMOD & ~0x30) | TIME1_CON.TimeMode;	//工作模式,0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
			if(TIME1_CON.TimeClkSource == CLOCK_12T)	AUXR &= ~0x40;	//12T
			if(TIME1_CON.TimeClkSource == CLOCK_1T)		AUXR |=  0x40;	//1T
			if(TIME1_CON.TimeClkSource == CLOCK_Ext)	TMOD |=  0x40;	//对外计数或分频
			else																			TMOD &= ~0x40;	//定时
			if(TIME1_CON.TimeClkOut == ENABLE)				INT_CLKO |=  0x02;	//输出时钟
			else																			INT_CLKO &= ~0x02;	//不输出时钟
			TH1 = (unsigned char)(TIME1_CON.TimeValue >> 8);
			TL1 = (unsigned char)TIME1_CON.TimeValue;
			if(TIME1_CON.TimeRun == ENABLE)	TR1 = 1;	//开始运行
			break;
		case 2:
			AUXR &= ~(1<<4);	//停止计数
			if(TIME2_CON.TimeInterrupt == ENABLE)					IE2  |=  (1<<2);	//允许中断
			else																			IE2  &= ~(1<<2);	//禁止中断
			if(TIME2_CON.TimeClkSource >  CLOCK_Ext)	return -1;
			if(TIME2_CON.TimeClkSource == CLOCK_12T)	AUXR &= ~(1<<2);	//12T
			if(TIME2_CON.TimeClkSource == CLOCK_1T)		AUXR |=  (1<<2);	//1T
			if(TIME2_CON.TimeClkSource == CLOCK_Ext)	AUXR |=  (1<<3);	//对外计数或分频
			else																			AUXR &= ~(1<<3);	//定时
			if(TIME2_CON.TimeClkOut == ENABLE)						INT_CLKO |=  0x04;	//输出时钟
			else																			INT_CLKO &= ~0x04;	//不输出时钟
			T2H =(unsigned char)(TIME2_CON.TimeValue >> 8);
			T2L =(unsigned char)TIME2_CON.TimeValue;
			if(TIME2_CON.TimeRun == ENABLE)	AUXR |=  (1<<4);	//开始运行
			break;
		case 3:
			T4T3M sl setLow[3];	//停止计数
			if(TIME3_CON.TimeInterrupt == ENABLE)					IE2  |=  (1<<5);	//允许中断
			else																			IE2  &= ~(1<<5);	//禁止中断
			if(TIME3_CON.TimeClkSource >  CLOCK_Ext)	return -1;
			if(TIME3_CON.TimeClkSource == CLOCK_12T)	T4T3M sl setLow[1];	//12T
			if(TIME3_CON.TimeClkSource == CLOCK_1T)		T4T3M sh setHigh[1];	//1T
			if(TIME3_CON.TimeClkSource == CLOCK_Ext)	T4T3M sh setHigh[2];	//对外计数或分频
			else																			T4T3M sl setLow[2];	//定时
			if(TIME3_CON.TimeClkOut == ENABLE)				T4T3M sh setHigh[0];	//输出时钟
			else																			T4T3M sl setLow[0];	//不输出时钟
			T3H =(unsigned char)(TIME3_CON.TimeValue >> 8);
			T3L =(unsigned char)TIME3_CON.TimeValue;
			if(TIME3_CON.TimeRun == ENABLE)	T4T3M sh setHigh[3];	//开始运行
			break;
		case 4:
			T4T3M sl setLow[7];	//停止计数
			if(TIME4_CON.TimeInterrupt == ENABLE)					IE2  |=  (1<<6);	//允许中断
			else																			IE2  &= ~(1<<6);	//禁止中断
			if(TIME4_CON.TimeClkSource >  CLOCK_Ext)	return -1;
			if(TIME4_CON.TimeClkSource == CLOCK_12T)	T4T3M sl setLow[5];	//12T
			if(TIME4_CON.TimeClkSource == CLOCK_1T)		T4T3M sh setHigh[5];	//1T
			if(TIME4_CON.TimeClkSource == CLOCK_Ext)	T4T3M sh setHigh[6];	//对外计数或分频
			else																			T4T3M sl setLow[6];	//定时
			if(TIME4_CON.TimeClkOut == ENABLE)				T4T3M sh setHigh[4];	//输出时钟
			else																			T4T3M sl setLow[4];	//不输出时钟
			T4H =(unsigned char)(TIME4_CON.TimeValue >> 8);
			T4L =(unsigned char)TIME4_CON.TimeValue;
			if(TIME4_CON.TimeRun == ENABLE)	T4T3M sh setHigh[7];	//开始运行
			break;
		default:return -1;
	}
	return 0;
}

void TIME0_CB()
{
	static int i;
	if(++i>=TIME_CLOCK[0])
	{
		i=0;
		TIMEx_CB[0]();
	}
}

void TIME1_CB()
{
	static int i;
	if(++i>=TIME_CLOCK[1])
	{
		i=0;
		TIMEx_CB[1]();
	}
}

void TIME2_CB()
{
	static int i;
	if(++i>=TIME_CLOCK[2])
	{
		i=0;
		TIMEx_CB[2]();
	}
}

void TIME3_CB()
{
	static int i;
	if(++i>=TIME_CLOCK[3])
	{
		i=0;
		TIMEx_CB[3]();
	}
}

void TIME4_CB()
{
	static int i;
	if(++i>=TIME_CLOCK[4])
	{
		i=0;
		TIMEx_CB[4]();
	}
}