#include "DHT11.h"
#include "HAL_GPIO.h"
#include "HAL_delay.h"

xdata unsigned char U8FLAG,U8temp;

void DHT11_begin(DHT11 *dht,unsigned char pin)
{
	dht->pin=pin;
	pinMode(pin,INOUT);
}

unsigned char DHT11_com(DHT11 *dht)
{
  unsigned char i;
	xdata unsigned char U8DHT11_comdata;

    for(i=0; i<8; i++)
    {

        U8FLAG=2;
        while((readPin(dht->pin)==0)&&U8FLAG++);
        delay_10us(3);
        U8temp=0;
        if(readPin(dht->pin)==1)U8temp=1;
        U8FLAG=2;
        while((readPin(dht->pin)==1)&&U8FLAG++);
        //��ʱ������forѭ��
        if(U8FLAG==1)break;
        //�ж�����λ��0����1

        // ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1

        U8DHT11_comdata<<=1;
        U8DHT11_comdata|=U8temp;        //0
    }
	return U8DHT11_comdata;
}

void DHT11_get(DHT11 *dht)
{
    xdata unsigned char temp_H,temp_L,hum_H,hum_L,check,check_add;
	
    writePin(dht->pin,0);
    delay(18);
    writePin(dht->pin,1);
    //������������������ ������ʱ20us
    delay_10us(4);
    //������Ϊ���� �жϴӻ���Ӧ�ź�
    writePin(dht->pin,1);
    //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
    if(readPin(dht->pin)==0)		 //T !
    {
        U8FLAG=2;
        //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
        while((readPin(dht->pin)==0)&&U8FLAG++);
        U8FLAG=2;
        //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
        while((readPin(dht->pin)==1)&&U8FLAG++);
        //���ݽ���״̬
        hum_H=DHT11_com(dht);
        hum_L=DHT11_com(dht);
        temp_H=DHT11_com(dht);
        temp_L=DHT11_com(dht);
        check=DHT11_com(dht);
        writePin(dht->pin,1);
        //����У��

        check_add=(temp_H+temp_L+hum_H+hum_L);
        if(check==check_add)
        {
					dht->humidity=hum_H;
					dht->humidity<<=8;
					dht->humidity|=hum_L;
					dht->temp=temp_H;
					dht->temp<<=8;
					dht->temp|=temp_L;
        }
    }

}