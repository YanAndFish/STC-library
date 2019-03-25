#include "HAL_delay.h"
#include "HAL_GPIO.h"
#include "DS18b20.h"


void DS18B20_begin(DS18B20 *ds,unsigned char pin)
{
	ds->pin=pin;
	pinMode(pin,OUTPUT);
}
int DS18B20_init(DS18B20 *ds)
{
	xdata unsigned char i;

	writePin(ds->pin,0);			 //����������480us~960us
	delay_10us(64);
	writePin(ds->pin,1);			//Ȼ���������ߣ����DS18B20������Ӧ�Ὣ��15us~60us����������
	i = 0;
	while(readPin(ds->pin))	//�ȴ�DS18B20��������
	{
		delay(1);
		i++;
		if(i>5)//�ȴ�>5MS
		{
			return -1;//��ʼ��ʧ��
		}
	
	}
	return 0;//��ʼ���ɹ�
}

unsigned char DS18B20_read(DS18B20 *ds)
{
	unsigned char byte, bi;
	unsigned int i, j;	
	for(j=8; j>0; j--)
	{
		writePin(ds->pin,0);//�Ƚ���������1us
		_nop_();
		_nop_();
		i = 3;
		while (--i);
		writePin(ds->pin,1);//Ȼ���ͷ�����
		_nop_();
		_nop_();
		i = 33;
		while (--i);
		bi = readPin(ds->pin);	 //��ȡ���ݣ������λ��ʼ��ȡ
		/*��byte����һλ��Ȼ����������7λ���bi��ע���ƶ�֮���Ƶ���λ��0��*/
		byte = (byte >> 1) | (bi << 7);						  
		delay_10us(5);
	}				
	return byte;
}

void DS18B20_write(DS18B20 *ds,unsigned char dat)
{
	unsigned int i,j;

	for(j=0; j<8; j++)
	{
		writePin(ds->pin,0);	     	  //ÿд��һλ����֮ǰ�Ȱ���������1us
		_nop_();
		_nop_();
		i = 3;
		while (--i);
		writePin(ds->pin,(dat & 0x01));  //Ȼ��д��һ�����ݣ������λ��ʼ
		i=6;
		delay_10us(7); //��ʱ68us������ʱ������60us
		writePin(ds->pin,1);	//Ȼ���ͷ����ߣ�����1us�����߻ָ�ʱ����ܽ���д��ڶ�����ֵ
		dat >>= 1;
	}
}

void  DS18B20_ReadTempCom(DS18B20 *ds)
{	
	DS18B20_init(ds);
	delay(1);
	DS18B20_write(ds,0xcc);	 //����ROM��������
	DS18B20_write(ds,0xbe);	 //���Ͷ�ȡ�¶�����
}
void  DS18B20_ChangTemp(DS18B20 *ds)
{
	DS18B20_init(ds);
	delay(1);
	DS18B20_write(ds,0xcc);		//����ROM��������		 
	DS18B20_write(ds,0x44);	    //�¶�ת������
	//Delay1ms(100);	//�ȴ�ת���ɹ������������һֱˢ�ŵĻ����Ͳ��������ʱ��
   
}

void DS18B20_getTemp(DS18B20 *ds)
{
	xdata float tp;
	xdata int temp = 0;
	xdata unsigned char tmh, tml;
	DS18B20_ChangTemp(ds);			 	//��д��ת������
	DS18B20_ReadTempCom(ds);			//Ȼ��ȴ�ת������Ͷ�ȡ�¶�����
	tml = DS18B20_read(ds);		//��ȡ�¶�ֵ��16λ���ȶ����ֽ�
	tmh = DS18B20_read(ds);		//�ٶ����ֽ�
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
	temp=tp*0.0625*100+0.5;
	ds->temp=temp;
}
