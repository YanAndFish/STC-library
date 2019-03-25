#include "HAL_PWM.h"
#include "bitctrl.h"
#include "interrupt.h"
#include "HAL_GPIO.h"

xdata PwmConfig PWM_CON={ENABLE,sysTimer(1),DISABLE,LOW,0};
#ifdef STC8
xdata PwmPortConfig PWM_0={0,0},PWM_1={0,0},PWM_2={0,0},PWM_3={0,0},PWM_4={0,0},PWM_5={0,0},PWM_6={0,0},PWM_7={0,0};
#endif
	
#ifdef STC15
xdata PwmPortConfig PWM_2={0,0},PWM_3={0,0},PWM_4={0,0},PWM_5={0,0},PWM_6={0,0},PWM_7={0,0};
#endif


#ifdef STC8
int PWM_begin(unsigned char port,unsigned int duty,unsigned char pwmint)
{
	int c=0;
	P_SW2 sh setHigh[7];
	if(duty==0)duty=1;//防止出现相位逆转，初始化值不要为0
	switch(port)
	{
		case 0:pinMode(20,OUTPUT);PWM_set(port,duty);PWM0CR sh setHigh[7];if(pwmint==DISABLE)PWM0CR sl ~0x07;else if(pwmint==INT_HIGH)PWM0CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM0CR sh 0x05; break;
		case 1:pinMode(21,OUTPUT);PWM_set(port,duty);PWM1CR sh setHigh[7];if(pwmint==DISABLE)PWM1CR sl ~0x07;else if(pwmint==INT_HIGH)PWM1CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM1CR sh 0x05; break;
		case 2:pinMode(22,OUTPUT);PWM_set(port,duty);PWM2CR sh setHigh[7];if(pwmint==DISABLE)PWM2CR sl ~0x07;else if(pwmint==INT_HIGH)PWM2CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM2CR sh 0x05; break;
		case 3:pinMode(23,OUTPUT);PWM_set(port,duty);PWM3CR sh setHigh[7];if(pwmint==DISABLE)PWM3CR sl ~0x07;else if(pwmint==INT_HIGH)PWM3CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM3CR sh 0x05; break;
		case 4:pinMode(24,OUTPUT);PWM_set(port,duty);PWM4CR sh setHigh[7];if(pwmint==DISABLE)PWM4CR sl ~0x07;else if(pwmint==INT_HIGH)PWM4CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM4CR sh 0x05; break;
		case 5:pinMode(25,OUTPUT);PWM_set(port,duty);PWM5CR sh setHigh[7];if(pwmint==DISABLE)PWM5CR sl ~0x07;else if(pwmint==INT_HIGH)PWM5CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM5CR sh 0x05; break;
		case 6:pinMode(26,OUTPUT);PWM_set(port,duty);PWM6CR sh setHigh[7];if(pwmint==DISABLE)PWM6CR sl ~0x07;else if(pwmint==INT_HIGH)PWM6CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM6CR sh 0x05; break;
		case 7:pinMode(27,OUTPUT);PWM_set(port,duty);PWM7CR sh setHigh[7];if(pwmint==DISABLE)PWM7CR sl ~0x07;else if(pwmint==INT_HIGH)PWM7CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM7CR sh 0x05; break;
		default:P_SW2 sl setLow[7];return -1;
	}
	c|=PWM_setup();
	P_SW2 sl setLow[7];
	EA=1;
	return c;
}
int PWM_end(unsigned char port)
{
	P_SW2 sh setHigh[7];
	switch(port)
	{
		case 0:PWM0CR sl ~0x87;PWM_CON.PwmPort sl setLow[0];pinMode(20,INPUT);break;
		case 1:PWM1CR sl ~0x87;PWM_CON.PwmPort sl setLow[1];pinMode(21,INPUT);break;
		case 2:PWM2CR sl ~0x87;PWM_CON.PwmPort sl setLow[2];pinMode(22,INPUT);break;
		case 3:PWM3CR sl ~0x87;PWM_CON.PwmPort sl setLow[3];pinMode(23,INPUT);break;
		case 4:PWM4CR sl ~0x87;PWM_CON.PwmPort sl setLow[4];pinMode(24,INPUT);break;
		case 5:PWM5CR sl ~0x87;PWM_CON.PwmPort sl setLow[5];pinMode(25,INPUT);break;
		case 6:PWM6CR sl ~0x87;PWM_CON.PwmPort sl setLow[6];pinMode(26,INPUT);break;
		case 7:PWM7CR sl ~0x87;PWM_CON.PwmPort sl setLow[7];pinMode(27,INPUT);break;
		default:P_SW2 sl setLow[7];return -1;
	}
	if(PWM_CON.PwmPort==0)
	{
		PWM_CON.PwmRun=0;
		PWMCR sl setLow[7];
	}
	P_SW2 sl setLow[7];
	return 0;
}
int PWM_set(unsigned char port,unsigned int duty)
{
	P_SW2 sh setHigh[7];
	if(duty>PWM_VALUE)
		return -1;
	switch(port)
	{
		case 0:PWM_0.PwmHighPort=duty;if(duty==0)PWM0HLD=0x01;else if (duty==PWM_VALUE)PWM0HLD=0x02;else PWM0HLD=0x00;PWM0T1=0x00;PWM0T2=duty;break;
		case 1:PWM_1.PwmHighPort=duty;if(duty==0)PWM1HLD=0x01;else if (duty==PWM_VALUE)PWM1HLD=0x02;else PWM1HLD=0x00;PWM1T1=0x00;PWM1T2=duty;break;
		case 2:PWM_2.PwmHighPort=duty;if(duty==0)PWM2HLD=0x01;else if (duty==PWM_VALUE)PWM2HLD=0x02;else PWM2HLD=0x00;PWM2T1=0x00;PWM2T2=duty;break;
		case 3:PWM_3.PwmHighPort=duty;if(duty==0)PWM3HLD=0x01;else if (duty==PWM_VALUE)PWM3HLD=0x02;else PWM3HLD=0x00;PWM3T1=0x00;PWM3T2=duty;break;
		case 4:PWM_4.PwmHighPort=duty;if(duty==0)PWM4HLD=0x01;else if (duty==PWM_VALUE)PWM4HLD=0x02;else PWM4HLD=0x00;PWM4T1=0x00;PWM4T2=duty;break;
		case 5:PWM_5.PwmHighPort=duty;if(duty==0)PWM5HLD=0x01;else if (duty==PWM_VALUE)PWM5HLD=0x02;else PWM5HLD=0x00;PWM5T1=0x00;PWM5T2=duty;break;
		case 6:PWM_6.PwmHighPort=duty;if(duty==0)PWM6HLD=0x01;else if (duty==PWM_VALUE)PWM6HLD=0x02;else PWM6HLD=0x00;PWM6T1=0x00;PWM6T2=duty;break;
		case 7:PWM_7.PwmHighPort=duty;if(duty==0)PWM7HLD=0x01;else if (duty==PWM_VALUE)PWM7HLD=0x02;else PWM7HLD=0x00;PWM7T1=0x00;PWM7T2=duty;break;
		default:P_SW2 sl setLow[7];return -1;
	}
	P_SW2 sl setLow[7];
	return 0;
}

int PWM_setup()
{
	P_SW2 sh setHigh[7];
	PWMCR sl setLow[7];
	if(PWM_CON.PwmClkSource >=0 && PWM_CON.PwmClkSource <=16)
	{
		PWMCKS sl ~0x1f;
		PWMCKS |= PWM_CON.PwmClkSource;
	}
	else 
	{
		P_SW2 sl setLow[7];
		return -1;
	}
	if(PWM_CON.PwmInterrupt == ENABLE)
		PWMCR sh setHigh[6];
	else
		PWMCR sl setLow[6];
	if(PWM_CON.PwmPolity== HIGH)
		IP2 sh setHigh[2];
	else
		IP2 sl setLow[2];
	PWMC=PWM_VALUE;
	if(PWM_CON.PwmRun==ENABLE)
		PWMCR sh setHigh[7];
	P_SW2 sl setLow[7];
	return 0;
}

#endif

#ifdef STC15

int PWM_begin(unsigned char port,unsigned int duty,unsigned char pwmint)
{
	int c=0;
	P_SW2 sh setHigh[7];
	if(duty==0)duty=1;//防止出现相位逆转，初始化值不要为0
	switch(port)
	{
		case 2:pinMode(37,OUTPUT);PWM_set(port,duty);PWMCR sh setHigh[0];if(pwmint==DISABLE)PWM2CR sl ~0x07;else if(pwmint==INT_HIGH)PWM2CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM2CR sh 0x05; break;
		case 3:pinMode(21,OUTPUT);PWM_set(port,duty);PWMCR sh setHigh[1];if(pwmint==DISABLE)PWM3CR sl ~0x07;else if(pwmint==INT_HIGH)PWM3CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM3CR sh 0x05; break;
		case 4:pinMode(22,OUTPUT);PWM_set(port,duty);PWMCR sh setHigh[2];if(pwmint==DISABLE)PWM4CR sl ~0x07;else if(pwmint==INT_HIGH)PWM4CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM4CR sh 0x05; break;
		case 5:pinMode(23,OUTPUT);PWM_set(port,duty);PWMCR sh setHigh[3];if(pwmint==DISABLE)PWM5CR sl ~0x07;else if(pwmint==INT_HIGH)PWM5CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM5CR sh 0x05; break;
		case 6:pinMode(16,OUTPUT);PWM_set(port,duty);PWMCR sh setHigh[4];if(pwmint==DISABLE)PWM6CR sl ~0x07;else if(pwmint==INT_HIGH)PWM6CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM6CR sh 0x05; break;
		case 7:pinMode(17,OUTPUT);PWM_set(port,duty);PWMCR sh setHigh[5];if(pwmint==DISABLE)PWM7CR sl ~0x07;else if(pwmint==INT_HIGH)PWM7CR sh 0x06;else if(pwmint==INT_TIMER)PWM_CON.PwmInterrupt=ENABLE;else PWM7CR sh 0x05; break;
		default:P_SW2 sl setLow[7];return -1;
	}
	c|=PWM_setup();
	P_SW2 sl setLow[7];
	EA=1;
	return c;
}

int PWM_end(unsigned char port)
{
	P_SW2 sh setHigh[7];
	switch(port)
	{
		case 2:PWMCR sl setLow[0];PWM2CR sl ~0x07;PWM_CON.PwmPort sl setLow[2];pinMode(37,INPUT);break;
		case 3:PWMCR sl setLow[1];PWM3CR sl ~0x07;PWM_CON.PwmPort sl setLow[3];pinMode(21,INPUT);break;
		case 4:PWMCR sl setLow[2];PWM4CR sl ~0x07;PWM_CON.PwmPort sl setLow[4];pinMode(22,INPUT);break;
		case 5:PWMCR sl setLow[3];PWM5CR sl ~0x07;PWM_CON.PwmPort sl setLow[5];pinMode(23,INPUT);break;
		case 6:PWMCR sl setLow[4];PWM6CR sl ~0x07;PWM_CON.PwmPort sl setLow[6];pinMode(16,INPUT);break;
		case 7:PWMCR sl setLow[5];PWM7CR sl ~0x07;PWM_CON.PwmPort sl setLow[7];pinMode(17,INPUT);break;
		default:P_SW2 sl setLow[7];return -1;
	}
	if(PWM_CON.PwmPort==0)
	{
		PWM_CON.PwmRun=0;
		PWMCR sl setLow[7];
	}
	P_SW2 sl setLow[7];
	return 0;
}

int PWM_set(unsigned char port,unsigned int duty)
{
	P_SW2 sh setHigh[7];
	if(duty>PWM_VALUE)
		return -1;
	//PWMCR sl setLow[7];
	switch(port)
	{
		case 2:if(duty==0)pinMode(37,INPUT);else if(PWM_2.PwmHighPort==0){pinMode(37,OUTPUT);PWM2T1=PWM_2.PwmLowPort;PWM2T2=duty;}else{PWM2T1=PWM_2.PwmLowPort;PWM2T2=duty;}PWM_2.PwmHighPort=duty;break;
		case 3:if(duty==0)pinMode(21,INPUT);else if(PWM_3.PwmHighPort==0){pinMode(21,OUTPUT);PWM3T1=PWM_3.PwmLowPort;PWM3T2=duty;}else{PWM3T1=PWM_3.PwmLowPort;PWM3T2=duty;}PWM_3.PwmHighPort=duty;break;
		case 4:if(duty==0)pinMode(22,INPUT);else if(PWM_4.PwmHighPort==0){pinMode(22,OUTPUT);PWM4T1=PWM_4.PwmLowPort;PWM4T2=duty;}else{PWM4T1=PWM_4.PwmLowPort;PWM4T2=duty;}PWM_4.PwmHighPort=duty;break;
		case 5:if(duty==0)pinMode(23,INPUT);else if(PWM_5.PwmHighPort==0){pinMode(23,OUTPUT);PWM5T1=PWM_5.PwmLowPort;PWM5T2=duty;}else{PWM5T1=PWM_5.PwmLowPort;PWM5T2=duty;}PWM_5.PwmHighPort=duty;break;
		case 6:if(duty==0)pinMode(16,INPUT);else if(PWM_6.PwmHighPort==0){pinMode(16,OUTPUT);PWM6T1=PWM_6.PwmLowPort;PWM6T2=duty;}else{PWM6T1=PWM_6.PwmLowPort;PWM6T2=duty;}PWM_6.PwmHighPort=duty;break;
		case 7:if(duty==0)pinMode(17,INPUT);else if(PWM_7.PwmHighPort==0){pinMode(17,OUTPUT);PWM7T1=PWM_7.PwmLowPort;PWM7T2=duty;}else{PWM7T1=PWM_7.PwmLowPort;PWM7T2=duty;}PWM_7.PwmHighPort=duty;break;
		default:P_SW2 sl setLow[7];return -1;
	}
	//PWMCR sh setHigh[7];
	P_SW2 sl setLow[7];
	return 0;
}

int PWM_setup()
{
	P_SW2 sh setHigh[7];
	PWMCR sl setLow[7];
	PWMCFG =0x00;
	if(PWM_CON.PwmClkSource >=0 && PWM_CON.PwmClkSource <=16)
	{
		PWMCKS sl ~0x1f;
		PWMCKS |= PWM_CON.PwmClkSource;
	}
	else 
	{
		P_SW2 sl setLow[7];
		return -1;
	}
	if(PWM_CON.PwmInterrupt == ENABLE)
		PWMCR sh setHigh[6];
	else
		PWMCR sl setLow[6];
	if(PWM_CON.PwmPolity== HIGH)
		IP2 sh setHigh[2];
	else
		IP2 sl setLow[2];
	PWMC=PWM_VALUE;
	if(PWM_CON.PwmRun==ENABLE)
		PWMCR sh setHigh[7];
	P_SW2 sl setLow[7];
	return 0;
}

#endif

void PWM_config(unsigned char PwmRun,
								unsigned char PwmClkSource,
								unsigned char PwmInterrupt,
								unsigned char PwmPolity)
{
	PWM_CON.PwmRun=PwmRun;
	PWM_CON.PwmClkSource=PwmClkSource;
	PWM_CON.PwmInterrupt=PwmInterrupt;
	PWM_CON.PwmPolity=PwmPolity;
}

int PWM_addTask(void(*hander)())
{
	return setInterruptTask(&PWM_INT,++PWM_INT.taskID,hander);
}

unsigned int PWM_persent(unsigned long persent)
{
	xdata unsigned long c;
	c=((unsigned long)PWM_VALUE*persent)/100;
	return (unsigned int)c;
}