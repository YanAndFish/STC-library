#include "interrupt.h"
#include "bitctrl.h"
#include <intrins.h>

xdata interruptTask INT0_INT={0,RUN_ALL_TASK},INT1_INT={0,RUN_ALL_TASK},INT2_INT={0,RUN_ALL_TASK},INT3_INT={0,RUN_ALL_TASK},INT4_INT={0,RUN_ALL_TASK},
TIME0_INT={0,RUN_ALL_TASK},TIME1_INT={0,RUN_ALL_TASK},TIME2_INT={0,RUN_ALL_TASK},TIME3_INT={0,RUN_ALL_TASK},TIME4_INT={0,RUN_ALL_TASK},
UART1_INT={0,RUN_ALL_TASK},UART2_INT={0,RUN_ALL_TASK},UART3_INT={0,RUN_ALL_TASK},UART4_INT={0,RUN_ALL_TASK},
ADC_INT={0,RUN_ALL_TASK},LVD_INT={0,RUN_ALL_TASK},PCA_INT={0,RUN_ALL_TASK},SPI_INT={0,RUN_ALL_TASK},CMP_INT={0,RUN_ALL_TASK},PWM_INT={0,RUN_ALL_TASK},PWMFD_INT={0,RUN_ALL_TASK},IIC_INT={0,RUN_ALL_TASK};

int setInterruptTask(interruptTask *intTask,unsigned char taskID,void(*hander)())
{
	if(taskID>=1 && taskID<=MAX_TASK_NUM)
	{
		intTask->taskID=taskID;
		intTask->hander[taskID-1]=hander;
		return 0;
	}
	return -1;
}

int setInterruptRunTask(interruptTask *intTask,unsigned char taskID)
{
	if((taskID<0 && taskID>intTask->taskID-1)||(taskID==MAX_TASK_NUM)){
		intTask->runTaskID=taskID;
		return 0;
	}
	return -1;
}

#ifdef STC8

void INT0_callBack() interrupt 0 //外部中断0
{
	xdata unsigned char i;
	if(INT0_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT0_INT.taskID;i++)
			INT0_INT.hander[i]();
	else
		INT0_INT.hander[INT0_INT.runTaskID]();
}

void TIME0_callBack() interrupt 1 //定时器0
{
	xdata unsigned char i;
	if(TIME0_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME0_INT.taskID;i++)
			TIME0_INT.hander[i]();
	else
		TIME0_INT.hander[TIME0_INT.runTaskID]();
}

void INT1_callBack() interrupt 2 //外部中断1
{
	xdata unsigned char i;
	if(INT1_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT1_INT.taskID;i++)
			INT1_INT.hander[i]();
	else
		INT1_INT.hander[INT1_INT.runTaskID]();
}

void TIME1_callBack() interrupt 3 //定时器1
{
	xdata unsigned char i;
	if(TIME1_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME1_INT.taskID;i++)
			TIME1_INT.hander[i]();
	else
		TIME1_INT.hander[TIME1_INT.runTaskID]();
}

void UART1_callBack() interrupt 4 //串口1
{
	xdata unsigned char i;
	if(UART1_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART1_INT.taskID;i++)
			UART1_INT.hander[i]();
	else
		UART1_INT.hander[UART1_INT.runTaskID]();
	TI=0;RI=0;
}

void ADC_callBack() interrupt 5 //ADC
{
	xdata unsigned char i;
	if(ADC_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<ADC_INT.taskID;i++)
			ADC_INT.hander[i]();
	else
		ADC_INT.hander[ADC_INT.runTaskID]();
	ADC_CONTR sl setLow[5];
}

void LVD_callBack() interrupt 6 //低压检测
{
	xdata unsigned char i;
	if(LVD_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<LVD_INT.taskID;i++)
			LVD_INT.hander[i]();
	else
		LVD_INT.hander[LVD_INT.runTaskID]();
}

void PCA_callBack() interrupt 7 //捕获
{
	xdata unsigned char i;
	if(PCA_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<PCA_INT.taskID;i++)
			PCA_INT.hander[i]();
	else
		PCA_INT.hander[PCA_INT.runTaskID]();
	CCON sl ~0x8f;
}

void UART2_callBack() interrupt 8 //串口2
{
	xdata unsigned char i;
	if(UART2_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART2_INT.taskID;i++)
			UART2_INT.hander[i]();
	else
		UART2_INT.hander[UART2_INT.runTaskID]();
	S2CON &= 0xfc;
}

void SPI_callBack() interrupt 9 //SPI
{
	xdata unsigned char i;
	if(SPI_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<SPI_INT.taskID;i++)
			SPI_INT.hander[i]();
	else
		SPI_INT.hander[SPI_INT.runTaskID]();
	SPSTAT sh setHigh[6];
	SPSTAT sh setHigh[7];
}

void INT2_callBack() interrupt 10 //外部中断2
{
	xdata unsigned char i;
	if(INT2_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT2_INT.taskID;i++)
			INT2_INT.hander[i]();
	else
		INT2_INT.hander[INT2_INT.runTaskID]();
}

void INT3_callBack() interrupt 11 //外部中断3
{
	xdata unsigned char i;
	if(INT3_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT3_INT.taskID;i++)
			INT3_INT.hander[i]();
	else
		INT3_INT.hander[INT3_INT.runTaskID]();
}

void TIME2_callBack() interrupt 12 //定时器2
{
	xdata unsigned char i;
	if(TIME2_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME2_INT.taskID;i++)
			TIME2_INT.hander[i]();
	else
		TIME2_INT.hander[TIME2_INT.runTaskID]();
}

void INT4_callBack() interrupt 13 //外部中断4
{
	xdata unsigned char i;
	if(INT4_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT4_INT.taskID;i++)
			INT4_INT.hander[i]();
	else
		INT4_INT.hander[INT4_INT.runTaskID]();
}

void UART3_callBack() interrupt 14 //串口3
{
	xdata unsigned char i;
	if(UART3_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART3_INT.taskID;i++)
			UART3_INT.hander[i]();
	else
		UART3_INT.hander[UART3_INT.runTaskID]();
	S3CON &= 0xfc;
}

void UART4_callBack() interrupt 15//串口4
{
	xdata unsigned char i;
	if(UART4_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART4_INT.taskID;i++)
			UART4_INT.hander[i]();
	else
		UART4_INT.hander[UART4_INT.runTaskID]();
	S4CON &= 0xfc;
}

void TIME3_callBack() interrupt 16//定时器3
{
	xdata unsigned char i;
	if(TIME3_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME3_INT.taskID;i++)
			TIME3_INT.hander[i]();
	else
		TIME3_INT.hander[TIME3_INT.runTaskID]();
}

void TIME4_callBack() interrupt 17//定时器4
{
	xdata unsigned char i;
	if(TIME4_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME4_INT.taskID;i++)
			TIME4_INT.hander[i]();
	else
		TIME4_INT.hander[TIME4_INT.runTaskID]();
}

void CMP_callBack() interrupt 18//比较器
{
	xdata unsigned char i;
	if(CMP_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<CMP_INT.taskID;i++)
			CMP_INT.hander[i]();
	else
		CMP_INT.hander[CMP_INT.runTaskID]();
}

void PWM_callBack() interrupt 19//PWM
{
	xdata unsigned char i;
	if(PWM_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<PWM_INT.taskID;i++)
			PWM_INT.hander[i]();
	else
		PWM_INT.hander[PWM_INT.runTaskID]();
	
	P_SW2 sh setHigh[7];
	PWMCFG sl setLow[6];
	PWMIF = 0x00;
	P_SW2 sl setLow[7];
}

void PWMFD_callBack() interrupt 20//PWM错误
{
	xdata unsigned char i;
	if(PWMFD_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<PWMFD_INT.taskID;i++)
			PWMFD_INT.hander[i]();
	else
		PWMFD_INT.hander[PWMFD_INT.runTaskID]();
}

void IIC_callBack() interrupt 21//IIC
{
	xdata unsigned char i;
	if(IIC_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<IIC_INT.taskID;i++)
			IIC_INT.hander[i]();
	else
		IIC_INT.hander[IIC_INT.runTaskID]();
	P_SW2 sh setHigh[7];
	if(I2CMSST & 0x40)I2CMSST &= ~0x40;
	else if(I2CSLST & 0x40)I2CSLST &= ~0x40;
	else if(I2CSLST & 0x20)I2CSLST &= ~0x20;
	else if(I2CSLST & 0x10)I2CSLST &= ~0x10;
	else if(I2CSLST & 0x08)I2CSLST &= ~0x08;
	P_SW2 sl setLow[7];
}

#endif

#ifdef STC15

void INT0_callBack() interrupt 0 //外部中断0
{
	xdata unsigned char i;
	if(INT0_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT0_INT.taskID;i++)
			INT0_INT.hander[i]();
	else
		INT0_INT.hander[INT0_INT.runTaskID]();
}

void TIME0_callBack() interrupt 1 //定时器0
{
	xdata unsigned char i;
	if(TIME0_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME0_INT.taskID;i++)
			TIME0_INT.hander[i]();
	else
		TIME0_INT.hander[TIME0_INT.runTaskID]();
}

void INT1_callBack() interrupt 2 //外部中断1
{
	xdata unsigned char i;
	if(INT1_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT1_INT.taskID;i++)
			INT1_INT.hander[i]();
	else
		INT1_INT.hander[INT1_INT.runTaskID]();
}

void TIME1_callBack() interrupt 3 //定时器1
{
	xdata unsigned char i;
	if(TIME1_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME1_INT.taskID;i++)
			TIME1_INT.hander[i]();
	else
		TIME1_INT.hander[TIME1_INT.runTaskID]();
}

void UART1_callBack() interrupt 4 //串口1
{
	xdata unsigned char i;
	if(UART1_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART1_INT.taskID;i++)
			UART1_INT.hander[i]();
	else
		UART1_INT.hander[UART1_INT.runTaskID]();
	TI=0;RI=0;
}

void ADC_callBack() interrupt 5 //ADC
{
	xdata unsigned char i;
	if(ADC_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<ADC_INT.taskID;i++)
			ADC_INT.hander[i]();
	else
		ADC_INT.hander[ADC_INT.runTaskID]();
	ADC_CONTR sl setLow[4];
	P1ASF sl ~0xff;
}

void LVD_callBack() interrupt 6 //低压检测
{
	xdata unsigned char i;
	if(LVD_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<LVD_INT.taskID;i++)
			LVD_INT.hander[i]();
	else
		LVD_INT.hander[LVD_INT.runTaskID]();
}

void PCA_callBack() interrupt 7 //捕获
{
	xdata unsigned char i;
	if(PCA_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<PCA_INT.taskID;i++)
			PCA_INT.hander[i]();
	else
		PCA_INT.hander[PCA_INT.runTaskID]();
	CCON sl ~0x87;
}

void UART2_callBack() interrupt 8 //串口2
{
	xdata unsigned char i;
	if(UART2_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART2_INT.taskID;i++)
			UART2_INT.hander[i]();
	else
		UART2_INT.hander[UART2_INT.runTaskID]();
	S2CON &= 0xfc;
}

void SPI_callBack() interrupt 9 //SPI
{
	xdata unsigned char i;
	if(SPI_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<SPI_INT.taskID;i++)
			SPI_INT.hander[i]();
	else
		SPI_INT.hander[SPI_INT.runTaskID]();
	SPSTAT sh setHigh[6];
	SPSTAT sh setHigh[7];
}

void INT2_callBack() interrupt 10 //外部中断2
{
	xdata unsigned char i;
	if(INT2_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT2_INT.taskID;i++)
			INT2_INT.hander[i]();
	else
		INT2_INT.hander[INT2_INT.runTaskID]();
}

void INT3_callBack() interrupt 11 //外部中断3
{
	xdata unsigned char i;
	if(INT3_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT3_INT.taskID;i++)
			INT3_INT.hander[i]();
	else
		INT3_INT.hander[INT3_INT.runTaskID]();
}

void TIME2_callBack() interrupt 12 //定时器2
{
	xdata unsigned char i;
	if(TIME2_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME2_INT.taskID;i++)
			TIME2_INT.hander[i]();
	else
		TIME2_INT.hander[TIME2_INT.runTaskID]();
}

void INT4_callBack() interrupt 16 //外部中断4
{
	xdata unsigned char i;
	if(INT4_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<INT4_INT.taskID;i++)
			INT4_INT.hander[i]();
	else
		INT4_INT.hander[INT4_INT.runTaskID]();
}

void UART3_callBack() interrupt 17 //串口3
{
	xdata unsigned char i;
	if(UART3_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART3_INT.taskID;i++)
			UART3_INT.hander[i]();
	else
		UART3_INT.hander[UART3_INT.runTaskID]();
	S3CON &= 0xfc;
}

void UART4_callBack() interrupt 18//串口4
{
	xdata unsigned char i;
	if(UART4_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<UART4_INT.taskID;i++)
			UART4_INT.hander[i]();
	else
		UART4_INT.hander[UART4_INT.runTaskID]();
	S4CON &= 0xfc;
}

void TIME3_callBack() interrupt 19//定时器3
{
	xdata unsigned char i;
	if(TIME3_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME3_INT.taskID;i++)
			TIME3_INT.hander[i]();
	else
		TIME3_INT.hander[TIME3_INT.runTaskID]();
}

void TIME4_callBack() interrupt 20//定时器4
{
	xdata unsigned char i;
	if(TIME4_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<TIME4_INT.taskID;i++)
			TIME4_INT.hander[i]();
	else
		TIME4_INT.hander[TIME4_INT.runTaskID]();
}

void CMP_callBack() interrupt 21//比较器
{
	xdata unsigned char i;
	if(CMP_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<CMP_INT.taskID;i++)
			CMP_INT.hander[i]();
	else
		CMP_INT.hander[CMP_INT.runTaskID]();
}

void PWM_callBack() interrupt 22//PWM
{
	xdata unsigned char i;
	if(PWM_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<PWM_INT.taskID;i++)
			PWM_INT.hander[i]();
	else
		PWM_INT.hander[PWM_INT.runTaskID]();
	
	P_SW2 sh setHigh[7];
	_nop_();
	PWMIF sl ~0x7f;
	P_SW2 sl setLow[7];
	_nop_();
}

void PWMFD_callBack() interrupt 23//PWM错误
{
	xdata unsigned char i;
	if(PWMFD_INT.runTaskID==RUN_ALL_TASK)
		for(i=0;i<PWMFD_INT.taskID;i++)
			PWMFD_INT.hander[i]();
	else
		PWMFD_INT.hander[PWMFD_INT.runTaskID]();
}
 
#endif