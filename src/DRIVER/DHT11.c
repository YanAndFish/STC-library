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
        //超时则跳出for循环
        if(U8FLAG==1)break;
        //判断数据位是0还是1

        // 如果高电平高过预定0高电平值则数据位为 1

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
    //总线由上拉电阻拉高 主机延时20us
    delay_10us(4);
    //主机设为输入 判断从机响应信号
    writePin(dht->pin,1);
    //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
    if(readPin(dht->pin)==0)		 //T !
    {
        U8FLAG=2;
        //判断从机是否发出 80us 的低电平响应信号是否结束
        while((readPin(dht->pin)==0)&&U8FLAG++);
        U8FLAG=2;
        //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
        while((readPin(dht->pin)==1)&&U8FLAG++);
        //数据接收状态
        hum_H=DHT11_com(dht);
        hum_L=DHT11_com(dht);
        temp_H=DHT11_com(dht);
        temp_L=DHT11_com(dht);
        check=DHT11_com(dht);
        writePin(dht->pin,1);
        //数据校验

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