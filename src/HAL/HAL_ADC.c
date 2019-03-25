#include "HAL_ADC.h"
#include "interrupt.h"
#include "bitctrl.h"
#include "HAL_DELAY.h"

xdata AdcConfig ADC_CON;
xdata unsigned char ADC_BEGIN_FLAG=0;

#ifdef STC8

void ADC_begin(unsigned char inte)
{
	ADC_config(1,inte,LOW,6);
	if(inte==1 && ADC_BEGIN_FLAG ==0)
	{
		ADC_BEGIN_FLAG=1;
		ADC_addTask(ADC_read);
	}
	ADC_setup();
	EA=1;
}
int ADC_start(unsigned char port)
{
	if(port<=15 && port>=0)
	{
		ADC_CONTR sl ~0x0f;
		ADC_CONTR += port;
	}
	else
		return -1;
	ADC_CONTR sh setHigh[6];
	if(ADC_CON.AdcInterrupt==DISABLE)
	{
		_nop_();
		_nop_();
		_nop_();
		while((ADC_CONTR & BIT_5)==0);
		ADC_CONTR sl setLow[5];
		ADC_read();
	}
	return 0;
}

void ADC_read()
{
	ADC_CON.AdcResult=ADC_RES;
	ADC_CON.AdcResult<<=8;
	ADC_CON.AdcResult+=ADC_RESL;
}

void ADC_setup()
{
	ADC_CONTR sl setLow[7];
	_nop_();
	_nop_();
	_nop_();
	ADCCFG sh setHigh[5];
	if(ADC_CON.AdcInterrupt==ENABLE)
		EADC=1;
	else
		EADC=0;
	if(ADC_CON.AdcPolity==HIGH)
		PADC=1;
	else
		PADC=0;
	ADCCFG sl ~0x0f;
	ADC_CONTR |= ADC_CON.AdcSpeed;
	if(ADC_CON.AdcRun)
	{
		ADC_CONTR sh setHigh[7];
		_nop_();
		_nop_();
		_nop_();
	}
	ADC_CONTR sl setLow[5];
}

void ADC_end()
{
	ADC_CONTR sl setLow[7];
}

#endif

#ifdef STC15

void ADC_begin(unsigned char inte)
{
	ADC_config(1,inte,LOW,2);
	if(inte==1 && ADC_BEGIN_FLAG ==0)
	{
		ADC_BEGIN_FLAG=1;
		ADC_addTask(ADC_read);
	}
	ADC_setup();
	EA=1;
}
int ADC_start(unsigned char port)
{
	if(port<=7 && port>=0)
	{
		ADC_CONTR sl ~0x07;
		ADC_CONTR |= port;
		P1ASF sh setHigh[port];
	}
	else
		return -1;
	ADC_CONTR sh setHigh[3];
	if(ADC_CON.AdcInterrupt==DISABLE)
	{
		_nop_();
		_nop_();
		_nop_();
		while((ADC_CONTR & BIT_4)==0);
		ADC_CONTR sl setLow[4];
		ADC_read();
		P1ASF sl ~0xff;
	}
	return 0;
}

void ADC_read(){
	ADC_CON.AdcResult=ADC_RES;
	ADC_CON.AdcResult<<=2;
	ADC_CON.AdcResult|=ADC_RESL;
}

void ADC_setup()
{
	ADC_CONTR sl setLow[7];
	_nop_();
	_nop_();
	_nop_();
	if(ADC_CON.AdcInterrupt==ENABLE)
		EADC=1;
	else
		EADC=0;
	if(ADC_CON.AdcPolity==HIGH)
		PADC=1;
	else
		PADC=0;
	ADC_CONTR sl ~0x60;
	ADC_CONTR |= (ADC_CON.AdcSpeed <<5);
	if(ADC_CON.AdcRun==1)
	{
		ADC_CONTR sh setHigh[7];
		delay(2);
	}
	ADC_CONTR sl setLow[4];
}

void ADC_end()
{
	ADC_CONTR sl setLow[7];
	P1ASF sl ~0xff;
}
#endif

void ADC_config(unsigned char AdcRun,unsigned char AdcInterrupt,unsigned char AdcPolity,unsigned char AdcSpeed)
{
	ADC_CON.AdcRun=AdcRun;
	ADC_CON.AdcInterrupt=AdcInterrupt;
	ADC_CON.AdcPolity=AdcPolity;
	ADC_CON.AdcSpeed=AdcSpeed;
}

int ADC_addTask(void (*hander)())
{
	return setInterruptTask(&ADC_INT,++ADC_INT.taskID,hander);
}