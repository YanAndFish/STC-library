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

	writePin(ds->pin,0);			 //将总线拉低480us~960us
	delay_10us(64);
	writePin(ds->pin,1);			//然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
	i = 0;
	while(readPin(ds->pin))	//等待DS18B20拉低总线
	{
		delay(1);
		i++;
		if(i>5)//等待>5MS
		{
			return -1;//初始化失败
		}
	
	}
	return 0;//初始化成功
}

unsigned char DS18B20_read(DS18B20 *ds)
{
	unsigned char byte, bi;
	unsigned int i, j;	
	for(j=8; j>0; j--)
	{
		writePin(ds->pin,0);//先将总线拉低1us
		_nop_();
		_nop_();
		i = 3;
		while (--i);
		writePin(ds->pin,1);//然后释放总线
		_nop_();
		_nop_();
		i = 33;
		while (--i);
		bi = readPin(ds->pin);	 //读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
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
		writePin(ds->pin,0);	     	  //每写入一位数据之前先把总线拉低1us
		_nop_();
		_nop_();
		i = 3;
		while (--i);
		writePin(ds->pin,(dat & 0x01));  //然后写入一个数据，从最低位开始
		i=6;
		delay_10us(7); //延时68us，持续时间最少60us
		writePin(ds->pin,1);	//然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		dat >>= 1;
	}
}

void  DS18B20_ReadTempCom(DS18B20 *ds)
{	
	DS18B20_init(ds);
	delay(1);
	DS18B20_write(ds,0xcc);	 //跳过ROM操作命令
	DS18B20_write(ds,0xbe);	 //发送读取温度命令
}
void  DS18B20_ChangTemp(DS18B20 *ds)
{
	DS18B20_init(ds);
	delay(1);
	DS18B20_write(ds,0xcc);		//跳过ROM操作命令		 
	DS18B20_write(ds,0x44);	    //温度转换命令
	//Delay1ms(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了
   
}

void DS18B20_getTemp(DS18B20 *ds)
{
	xdata float tp;
	xdata int temp = 0;
	xdata unsigned char tmh, tml;
	DS18B20_ChangTemp(ds);			 	//先写入转换命令
	DS18B20_ReadTempCom(ds);			//然后等待转换完后发送读取温度命令
	tml = DS18B20_read(ds);		//读取温度值共16位，先读低字节
	tmh = DS18B20_read(ds);		//再读高字节
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
	temp=tp*0.0625*100+0.5;
	ds->temp=temp;
}
