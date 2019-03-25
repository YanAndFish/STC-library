#include "HAL_IIC.h"
#include "HAL_DELAY.h"
#include "interrupt.h"
#include "bitctrl.h"

#ifdef STC15

void IIC_Start()
{
	SDA=1;
	IIC_wait();
	SCL=1;
	IIC_wait();//����ʱ����SDA����ʱ��>4.7us
	SDA=0;
	IIC_wait();//����ʱ����>4us	
	SCL=0;
	IIC_wait();//����ʱ����>4us
}

void IIC_Stop()
{
	IIC_wait();	
	SDA=0;
	IIC_wait();
	SCL=1;
	IIC_wait();//����ʱ�����4.7us
	SDA=1;
	IIC_wait();		
}

int IIC_Write(unsigned char dat)
{
	unsigned char a=0,b=0;//���255��һ����������Ϊ1us�������ʱ255us��		
	for(a=0;a<8;a++)//Ҫ����8λ�������λ��ʼ
	{
		SDA=dat>>7;	 //��ʼ�ź�֮��SCL=0�����Կ���ֱ�Ӹı�SDA�ź�
		dat=dat<<1;
		IIC_wait();
		SCL=1;
		IIC_wait();//����ʱ��>4.7us
		SCL=0;
		IIC_wait();//ʱ�����4us		
	}
	SDA=1;
	IIC_wait();
	SCL=1;
	while(SDA)//�ȴ�Ӧ��Ҳ���ǵȴ����豸��SDA����
	{
		b++;
		IIC_wait();
		if(b>200)	 //�������2000usû��Ӧ����ʧ��
		{
			SCL=0;
			IIC_wait();
			return -1;
		}
	}
	SCL=0;
	IIC_wait();
 	return 0;		
}

unsigned char IIC_read()
{
	unsigned char a=0,dat=0;
	SDA=1;			//��ʼ�ͷ���һ���ֽ�֮��SCL����0
	IIC_wait();
	for(a=0;a<8;a++)//����8���ֽ�
	{
		SCL=1;
		IIC_wait();
		dat<<=1;
		dat|=SDA;
		IIC_wait();
		SCL=0;
		IIC_wait();
	}
	return dat;		
}

void IIC_writeNoAsk(void)
{
    SDA = 1;  
    IIC_wait();  
    SCL = 1;  
    IIC_wait();  
    SCL = 0;  
    IIC_wait();  
}

void IIC_writeAsk(void)
{  
    SDA = 0;  
    IIC_wait();
    SCL = 1;  
    IIC_wait();  
    SCL = 0;  
    IIC_wait();  
}

void IIC_wait()
{
	unsigned char i;
	_nop_();
	_nop_();
	i = 33;
	while (--i);
}
#endif

#ifdef STC8

xdata IicConfig IIC_CON;

int IIC_begin(unsigned char mode,unsigned char inte,unsigned char addr)
{
	P_SW2 sh setHigh[7];
	I2CCFG sl setLow[7];
	I2CCFG |= 0x3f;
	if(mode==IIC_MASTER)
	{
		I2CCFG sh setHigh[6];
		if(inte==1)
			I2CMSCR sh setHigh[7];
		else
			I2CMSCR sl setLow[7];
	}
	else if(mode==IIC_SALVE)
	{
		I2CCFG sl setLow[6];
		I2CSLADR = addr;
		if(inte==1)
			I2CSLCR |= 0x78;
		else
			I2CSLCR &= ~0x78;
	}
	else
	{
		P_SW2 sl setLow[7];
		return -1;
	}
	IIC_CON.IIC_mode=mode;
	IIC_CON.IIC_interrupt=inte;
	
	I2CCFG sh setHigh[7];
	P_SW2 sl setLow[7];
	EA=1;
	return 0;
}

void IIC_wait()
{
	while (!(I2CMSST & 0x40));
  I2CMSST &= ~0x40;
	IIC_CON.IIC_busy=0;
}

void IIC_Start()
{
	P_SW2 sh setHigh[7];
  IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x81;                             //����START����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
}

void IIC_write(char dat)
{
	P_SW2 sh setHigh[7];
  I2CTXD = dat;                               //д���ݵ����ݻ�����
	IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x82;                             //����SEND����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
}

void IIC_readAsk()
{
	P_SW2 sh setHigh[7];
	IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x83;                             //���Ͷ�ACK����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
}

char IIC_read()
{
	P_SW2 sh setHigh[7];
	IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x84;                             //����RECV����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
  return I2CRXD;
}

void IIC_writeAsk()
{
	P_SW2 sh setHigh[7];
  I2CMSST = 0x00;                             //����ACK�ź�
	IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x85;                             //����ACK����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
}

void IIC_writeNoAsk()
{
	P_SW2 sh setHigh[7];
  I2CMSST = 0x01;                             //����NAK�ź�
	IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x85;                             //����ACK����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
}

void IIC_stop()
{
	P_SW2 sh setHigh[7];
	IIC_CON.IIC_busy = 1;
  I2CMSCR = 0x86;                             //����STOP����
	if(IIC_CON.IIC_interrupt==ENABLE)
    while (IIC_CON.IIC_busy==1);
	else
		IIC_wait();
	P_SW2 sl setLow[7];
}

int IIC_addTask(void(*hander)())
{
	return setInterruptTask(&IIC_INT,++IIC_INT.taskID,hander);
}

#endif