#include "core.h"

xdata char time[7];
unsigned char i=0;

void setup()
{
	POWER_autoReboot();
	UART_begin(Uart1,115200);
	pinMode(14,INOUT);
	pinMode(15,INOUT);
	SDA=1;
	SCL=1;
}

void loop()
{
	EA=0;
	IIC_Start();
	IIC_Write(0xd0);
	IIC_Write(0x00);
	IIC_Start();
	IIC_Write(0xd1);
	for(i=0;i<6;i++)
	{
		time[i]=IIC_read();
		IIC_writeAsk();
	}
	time[6]=IIC_read();
	IIC_writeNoAsk();
	IIC_Stop();
	EA=1;
	
	UART_print(Uart1,"20");
	UART_write(Uart1,0x30+((time[6]&0xf0)>>4));
	UART_write(Uart1,0x30+(time[6]&0x0f));
	UART_write(Uart1,'.');
	UART_write(Uart1,0x30+((time[5]&0x70)>>4));
	UART_write(Uart1,0x30+(time[5]&0x0f));	
	UART_write(Uart1,'.');
	UART_write(Uart1,0x30+((time[4]&0xf0)>>4));
	UART_write(Uart1,0x30+(time[4]&0x0f));	
	UART_print(Uart1," ");
	if(time[2] & 0x40)//12
	{
		if(time[2] & 0x10)
			UART_print(Uart1,"PM");
		else
			UART_print(Uart1,"AM");
		UART_write(Uart1,0x30+((time[2]&0x10)>>4));
		UART_write(Uart1,0x30+(time[2]&0x0f));
	}
	else//24
	{
		UART_write(Uart1,0x30+((time[2]&0x30)>>4));
		UART_write(Uart1,0x30+(time[2]&0x0f));
	}
	UART_write(Uart1,':');
	UART_write(Uart1,0x30+((time[1]&0xf0)>>4));
	UART_write(Uart1,0x30+(time[1]&0x0f));
	UART_write(Uart1,':');
	UART_write(Uart1,0x30+((time[0]&0xf0)>>4));
	UART_write(Uart1,0x30+(time[0]&0x0f));
	UART_println(Uart1,".");
	
	delay(1000);
}