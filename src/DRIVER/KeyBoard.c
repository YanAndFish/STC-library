#include "KeyBoard.h"
#include "HAL_GPIO.h"
#include "HAL_delay.h"

int KEYBOARD_begin(KeyBoard *kb,unsigned char port,unsigned char mode,unsigned int time)
{
	if(port<0 || port>7)
		return -1;
	kb->port=port;
	portMode(port,INOUT);
	if(mode<0 || mode>1)
		return -1;
	kb->mode=mode;
	if(time==0)
		time=1;
	kb->time=time;
	return 0;
}

void KEYBOARD_getkey(KeyBoard *kb)
{
	unsigned int a=0;
	xdata unsigned char KeyValue=0xff;
	writePort(kb->port,0x0f);
	if(readPort(kb->port)!=0x0f)//��ȡ�����Ƿ���
	{
		delay(15);//��ʱ15ms��������
		if(readPort(kb->port)!=0x0f)//�ٴμ������Ƿ���
		{	
			//������
			writePort(kb->port,0x0f);
			switch(readPort(kb->port))
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//������
			writePort(kb->port,0xf0);
			switch(readPort(kb->port))
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<kb->time)&&(readPort(kb->port)!=0xf0))	 //��ⰴ�����ּ��
			{
				delay(1);
				if(kb->mode==KEYBOARD_REPEAT)a++;
			}
		}
	}
	kb->keynum=KeyValue;
}