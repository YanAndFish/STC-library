#include "HAL_PCA.h"
#include "interrupt.h"
#include "bitctrl.h"

xdata PcaConfig PCA_CON={PCA_1,0,0,1};

int PCA_begin(unsigned char port,unsigned char mode,unsigned int value,unsigned char inte)//配置系统，配置端口
{
	switch(port)
	{
		case 0:
			switch(mode)
			{
				case 0:CCAPM0 sl ~0x7e;CCAPM0 sh 0x20;break;
				case 1:CCAPM0 sl ~0x7e;CCAPM0 sh 0x10;break;
				case 2:CCAPM0 sl ~0x7e;CCAPM0 sh 0x30;break;
				case 3:CCAPM0 sl ~0x7e;CCAPM0 sh 0x48;break;
				case 4:CCAPM0 sl ~0x7e;CCAPM0 sh 0x4c;break;
				default:return -1;
			}
			if(inte==1)
				CCAPM0 sh setHigh[0];
			else
				CCAPM0 sl setLow[0];
			CCAP0L=(unsigned char)value;
			CCAP0H=(unsigned char)(value >>8);
			break;
		case 1:
			switch(mode)
			{
				case 0:CCAPM1 sl ~0x7e;CCAPM1 sh 0x20;break;
				case 1:CCAPM1 sl ~0x7e;CCAPM1 sh 0x10;break;
				case 2:CCAPM1 sl ~0x7e;CCAPM1 sh 0x30;break;
				case 3:CCAPM1 sl ~0x7e;CCAPM1 sh 0x48;break;
				case 4:CCAPM1 sl ~0x7e;CCAPM1 sh 0x4c;break;
				default:return -1;
			}
			if(inte==1)
				CCAPM1 sh setHigh[0];
			else
				CCAPM1 sl setLow[0];
			CCAP1L=(unsigned char)value;
			CCAP1H=(unsigned char)(value >>8);
			break;
		case 2:
			switch(mode)
			{
				case 0:CCAPM2 sl ~0x7e;CCAPM2 sh 0x20;break;
				case 1:CCAPM2 sl ~0x7e;CCAPM2 sh 0x10;break;
				case 2:CCAPM2 sl ~0x7e;CCAPM2 sh 0x30;break;
				case 3:CCAPM2 sl ~0x7e;CCAPM2 sh 0x48;break;
				case 4:CCAPM2 sl ~0x7e;CCAPM2 sh 0x4c;break;
				default:return -1;
			}
			if(inte==1)
				CCAPM2 sh setHigh[0];
			else
				CCAPM2 sl setLow[0];
			CCAP2L=(unsigned char)value;
			CCAP2H=(unsigned char)(value >>8);
			break;
	#ifdef STC8
		case 3:
			switch(mode)
			{
				case 0:CCAPM3 sl ~0x7e;CCAPM3 sh 0x20;break;
				case 1:CCAPM3 sl ~0x7e;CCAPM3 sh 0x10;break;
				case 2:CCAPM3 sl ~0x7e;CCAPM3 sh 0x30;break;
				case 3:CCAPM3 sl ~0x7e;CCAPM3 sh 0x48;break;
				case 4:CCAPM3 sl ~0x7e;CCAPM3 sh 0x4c;break;
				default:return -1;
			}
			if(inte==1)
				CCAPM3 sh setHigh[0];
			else
				CCAPM3 sl setLow[0];
			CCAP3L=(unsigned char)value;
			CCAP3H=(unsigned char)(value >>8);
			break;
	#endif
		default:return -1;
	}
	PCA_setup();
	EA=1;
	return 0;
}

void PCA_end()//关闭中断和定时器
{
	CCON sl setLow[6];
	CMOD sl setLow[0];
	CCAPM0 sl setLow[0];
	CCAPM1 sl setLow[0];
	CCAPM2 sl setLow[0];
#ifdef STC8
	CCAPM3 sl setLow[0];
#endif
}
void PCA_config(unsigned char	PcaClock,
								unsigned char	PcaInterrupt,
								unsigned char	PcaPolity,
								unsigned char PcaRun)
{
	PCA_CON.PcaClock=PcaClock;
	PCA_CON.PcaInterrupt=PcaInterrupt;
	PCA_CON.PcaPolity=PcaPolity;
	PCA_CON.PcaRun=PcaRun;
}
void PCA_setup()//上传系统配置
{
	CCON sl setLow[6];//关闭
	CMOD sl ~0x0e;
	CMOD sh (PCA_CON.PcaClock<<1);//配置时钟
	if(PCA_CON.PcaInterrupt==1)
		CMOD sh setHigh[0];
	else
		CMOD sl setLow[0];
	if(PCA_CON.PcaPolity==1)
		IP sh setHigh[7];
	else
		IP sl setLow[7];
	if(PCA_CON.PcaRun==1)
		CCON sh setHigh[6];
}
int PCA_addTask(void (*hander)())//添加中断任务
{
	return setInterruptTask(&PCA_INT,++PCA_INT.taskID,hander);
}

int PCA_tog(unsigned char port,unsigned char onoff)
{
	switch(port)
	{
		case 0:if(onoff==1)CCAPM0 sh setHigh[2];else CCAPM0 sl setLow[2];break;
		case 1:if(onoff==1)CCAPM1 sh setHigh[2];else CCAPM1 sl setLow[2];break;
		case 2:if(onoff==1)CCAPM2 sh setHigh[2];else CCAPM2 sl setLow[2];break;
		#ifdef STC8
		case 3:if(onoff==1)CCAPM3 sh setHigh[2];else CCAPM3 sl setLow[2];break;
		#endif
		default:return -1;
	}
	return 0;
}