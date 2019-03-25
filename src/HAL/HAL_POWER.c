#include "HAL_POWER.h"
#include "HAL_UART.h"
#include "bitctrl.h"
#include "interrupt.h"
#include <intrins.h>

int POWER_div(unsigned char div)
{
	if(div<0 || div >7)
		return -1;
	CLK_DIV &= 0xf8;
	CLK_DIV |= div;
	_nop_();_nop_();
	_nop_();_nop_();
	_nop_();_nop_();
	_nop_();_nop_();
	_nop_();_nop_();
	return 0;
}

void POWER_idle()
{
	PCON |=0x01;
	_nop_();_nop_();
	_nop_();_nop_();
	_nop_();_nop_();
}

void POWER_down(unsigned char clock,unsigned int ms)//此功能有故障
{
	ms *= 2;
	if(clock==1)
	{
		WKTCL=(unsigned char)ms;
		WKTCH=(unsigned char)(ms>>8);
		_nop_();_nop_();
		_nop_();_nop_();
		WKTCH sh setHigh[7];
	}
	else
	{
		WKTCL=0x00;
		WKTCH=0x00;
	}
	_nop_();_nop_();
	_nop_();_nop_();
	PCON |=0x02;
	_nop_();_nop_();
	_nop_();_nop_();
	_nop_();_nop_();
}

void reboot_CB()
{
	xdata unsigned char cheat;
	static unsigned char i,j;
	static unsigned long changeBaud;
	if(RI)
	{
		cheat=SBUF;
		if(cheat==0x7f || cheat==0x80 || cheat==0xe0 || cheat==0xf8 || cheat==0xfe || (cheat==0x00 && UART1_CON.UartBaudRate>19200))
			i++;
		else
			j++;
		if(j>=4)
		{
			j=0;i=0;
			if(changeBaud!=0 && UART1_CON.UartBaudRate==2400)
			{
				UART1_CON.UartBaudRate=changeBaud;
				UART_setup(Uart1);
				changeBaud=0;
			}
		}
		if(i>=20 && cheat==0x7f)//波特率2400
			IAP_CONTR |= 0x60;
		else if(i>=30 && cheat==0xfe && UART1_CON.UartBaudRate==4800)//波特率4800
			IAP_CONTR |= 0x60;
		else if(i>=30 && cheat==0xf8 && UART1_CON.UartBaudRate==9600)//波特率9600
			IAP_CONTR |= 0x60;
		else if(i>=30 && cheat==0xe0 && UART1_CON.UartBaudRate==14400)//波特率14400
			IAP_CONTR |= 0x60;
		else if(i>=30 && cheat==0x80 && UART1_CON.UartBaudRate==19200)//波特率19200
			IAP_CONTR |= 0x60;
		else if(i>=30 && cheat==0x00 && UART1_CON.UartBaudRate>19200)//高速波特率
		{
			changeBaud=UART1_CON.UartBaudRate;
			UART1_CON.UartBaudRate=2400;
			UART_setup(Uart1);
			i=0;j=0;
		}
	}
}

int POWER_autoReboot()
{
	return setInterruptTask(&UART1_INT,++UART1_INT.taskID,reboot_CB);
}

void POWER_reset()
{
	IAP_CONTR |= 0x20;
}

void POWER_Reboot()
{
	IAP_CONTR |= 0x60;
}