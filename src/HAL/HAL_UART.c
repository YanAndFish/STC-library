#include "HAL_UART.h"
#include "HAL_TIMER.h"
#include "interrupt.h"
#include "bitctrl.h"

xdata UartBufferConfig UART1_BUFF_CON={{0},{0},0,0,0,0,0},UART2_BUFF_CON={{0},{0},0,0,0,0,0},UART3_BUFF_CON={{0},{0},0,0,0,0,0},UART4_BUFF_CON={{0},{0},0,0,0,0,0};
xdata UartConfig UART1_CON,UART2_CON,UART3_CON,UART4_CON;

int UART_begin(unsigned char uart,unsigned long baud)
{
	switch(uart)
	{
		case 1:
			UART_config(&UART1_CON,Uart8bitBRT,Timer1,baud,DISABLE,ENABLE,DISABLE,ENABLE,LOW,DISABLE);
			UART_setup(Uart1);
			UART_addTask(Uart1,UART_CB1);
			break;
		case 2:
			UART_config(&UART2_CON,Uart8bitBRT,Timer2,baud,DISABLE,ENABLE,DISABLE,ENABLE,LOW,DISABLE);
			UART_setup(Uart2);
			UART_addTask(Uart2,UART_CB2);
			break;
		case 3:
			UART_config(&UART3_CON,Uart8bitBRT,Timer3,baud,DISABLE,ENABLE,DISABLE,ENABLE,LOW,DISABLE);
			UART_setup(Uart3);
			UART_addTask(Uart3,UART_CB3);
			break;
		case 4:
			UART_config(&UART4_CON,Uart8bitBRT,Timer4,baud,DISABLE,ENABLE,DISABLE,ENABLE,LOW,DISABLE);
			UART_setup(Uart4);
			UART_addTask(Uart4,UART_CB4);
			break;
		default:return -1;
	}
	EA=1;
	return 0;
}

int UART_end(unsigned char uart)
{
	switch(uart)
	{
		case 1:
			ES=0;
			if(UART1_CON.UartBrt == Timer1)//T1 or T2
				TR1=0;
			else
				AUXR &= ~(1<<4);
			break;
		case 2:
			IE2 sl setLow[0];
			AUXR &= ~(1<<4);
			break;
		case 3:
			IE2 sl setLow[3];
			if(UART1_CON.UartBrt == Timer2)//T2 or T3
				AUXR &= ~(1<<4);
			else
				T4T3M sl setLow[3];
			break;
		case 4:
			IE2 sl setLow[4];
			if(UART1_CON.UartBrt == Timer2)//T2 or T4
				AUXR &= ~(1<<4);
			else
				T4T3M sl setLow[7];
			break;
		default:return -1;
	}
	return 0;
}

int UART_addTask(unsigned char uart,void(*hander)())
{
	xdata int c=0;
	switch(uart)
	{
		case 1:
			c |= setInterruptTask(&UART1_INT,++UART1_INT.taskID,hander);
			break;
		case 2:
			c |= setInterruptTask(&UART2_INT,++UART2_INT.taskID,hander);
			break;
		case 3:
			c |= setInterruptTask(&UART3_INT,++UART3_INT.taskID,hander);
			break;
		case 4:
			c |= setInterruptTask(&UART4_INT,++UART4_INT.taskID,hander);
			break;
		default:return -1;
	}
	return c;
}

void UART_config(UartConfig *uart,
	unsigned char	UartMode,			//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	unsigned char	UartBrt,		//使用波特率,   BRT_Timer1,BRT_Timer2
	unsigned long	UartBaudRate,		//波特率,       ENABLE,DISABLE
	unsigned char	UartMoreCommunicate,	//多机通讯允许, ENABLE,DISABLE
	unsigned char	UartRxEnable,		//允许接收,   ENABLE,DISABLE
	unsigned char	BaudRateDouble,		//波特率加倍, ENABLE,DISABLE
	unsigned char	UartInterrupt,		//中断控制,   ENABLE,DISABLE
	unsigned char	UartPolity,		//优先级,     PolityLow,PolityHigh
	unsigned char	UartShort)	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
{
	uart->UartMode=UartMode;
	uart->UartBrt=UartBrt;
	uart->UartBaudRate=UartBaudRate;
	uart->UartMoreCommunicate=UartMoreCommunicate;
	uart->UartRxEnable=UartRxEnable;
	uart->BaudRateDouble=BaudRateDouble;
	uart->UartInterrupt=UartInterrupt;
	uart->UartPolity=UartPolity;
	uart->UartShort=UartShort;
}

int UART_setup(unsigned char uart)
{
	xdata unsigned int clk;
	switch(uart)
	{
		case 1:
			if(UART1_CON.UartMode > Uart9bitBRT)	return -1;	//模式错误
			if(UART1_CON.UartPolity == HIGH)		PS = 1;	//高优先级中断
			else																PS = 0;	//低优先级中断
			SCON = (SCON & 0x3f) | UART1_CON.UartMode;
			if((UART1_CON.UartMode == Uart9bitBRT) ||(UART1_CON.UartMode == Uart8bitBRT))	//可变波特率
			{
				clk = (CLOCK / 4) / UART1_CON.UartBaudRate;	//按1T计算
				if(clk >= 65536UL)	return -1;	//错误
				clk = 65536UL - clk;
				if(UART1_CON.UartBrt == Timer1)
				{
					AUXR &= ~0x01;		//S1 BRT Use Timer1;
					TIMER_config(&TIME1_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
					TIMER_setup(Timer1);
				}
				else if(UART1_CON.UartBrt == Timer2)
				{
					AUXR |= 0x01;		//S1 BRT Use Timer2;
					TIMER_config(&TIME2_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
					TIMER_setup(Timer2);
				}
				else return -1;	//错误
			}
			else if(UART1_CON.UartMode == UartShiftRight)
			{
				if(UART1_CON.BaudRateDouble == ENABLE)AUXR |=  (1<<5);	//固定波特率SysClk/2
				else																	AUXR &= ~(1<<5);	//固定波特率SysClk/12
			}
			else if(UART1_CON.UartMode == Uart9bit)	//固定波特率SysClk*2^SMOD/64
			{
				if(UART1_CON.BaudRateDouble == ENABLE)PCON |=  (1<<7);	//固定波特率SysClk/32
				else																	PCON &= ~(1<<7);	//固定波特率SysClk/64
			}
			if(UART1_CON.UartInterrupt == ENABLE)		ES = 1;	//允许中断
			else																		ES = 0;	//禁止中断
			if(UART1_CON.UartRxEnable == ENABLE)		REN = 1;	//允许接收
			else																		REN = 0;	//禁止接收
			if(UART1_CON.UartShort == ENABLE)				CLK_DIV |=  (1<<4);	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
			else																		CLK_DIV &= ~(1<<4);
			break;
		case 2:
			if((UART2_CON.UartMode == Uart9bitBRT) ||(UART2_CON.UartMode == Uart8bitBRT))	//可变波特率
			{
				if(UART2_CON.UartPolity == HIGH)		IP2 |=  1;	//高优先级中断
				else																			IP2 &= ~1;	//低优先级中断
				if(UART2_CON.UartMode == Uart9bitBRT)			S2CON |=  (1<<7);	//9bit
				else																			S2CON &= ~(1<<7);	//8bit
				clk = (CLOCK / 4) / UART2_CON.UartBaudRate;	//按1T计算
				if(clk >= 65536UL)	return -1;	//错误
				clk = 65536UL - clk;
				TIMER_config(&TIME2_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
				TIMER_setup(Timer2);
			}
			else	return -1;	//模式错误
			if(UART2_CON.UartInterrupt == ENABLE)	IE2   |=  1;		//允许中断
			else																		IE2   &= ~1;		//禁止中断
			if(UART2_CON.UartRxEnable == ENABLE)		S2CON |=  (1<<4);	//允许接收
			else																		S2CON &= ~(1<<4);	//禁止接收
			break;
		case 3:
			if((UART3_CON.UartMode == Uart9bitBRT) ||(UART3_CON.UartMode == Uart8bitBRT))	//可变波特率
			{
				if(UART3_CON.UartMode == Uart9bitBRT)			S3CON |=  (1<<7);	//9bit
				else																			S3CON &= ~(1<<7);	//8bit
				clk = (CLOCK / 4) / UART3_CON.UartBaudRate;	//按1T计算
				if(clk >= 65536UL)	return -1;	//错误
				clk = 65536UL - clk;
				if(UART3_CON.UartBrt == Timer2)
				{
					S3CON sl setLow[6];		//S3 BRT Use Timer2;
					TIMER_config(&TIME2_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
					TIMER_setup(Timer2);
				}
				else if(UART3_CON.UartBrt == Timer3)
				{
					S3CON sh setHigh[6];		//S3 BRT Use Timer3;
					TIMER_config(&TIME3_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
					TIMER_setup(Timer3);
				}
				else
					return -1;
			}
			else	return -1;	//模式错误
			if(UART3_CON.UartInterrupt == ENABLE)		IE2 sh setHigh[3];		//允许中断
			else																		IE2 sl setLow[3];		//禁止中断
			if(UART3_CON.UartRxEnable == ENABLE)		S3CON |=  (1<<4);	//允许接收
			else																		S3CON &= ~(1<<4);	//禁止接收
			break;
		case 4:
			if((UART4_CON.UartMode == Uart9bitBRT) ||(UART4_CON.UartMode == Uart8bitBRT))	//可变波特率
			{
				if(UART4_CON.UartMode == Uart9bitBRT)			S4CON |=  (1<<7);	//9bit
				else																			S4CON &= ~(1<<7);	//8bit
				clk = (CLOCK / 4) / UART4_CON.UartBaudRate;	//按1T计算
				if(clk >= 65536UL)	return -1;	//错误
				clk = 65536UL - clk;
				if(UART4_CON.UartBrt == Timer2)
				{
					S4CON sl setLow[6];		//S4 BRT Use Timer2;
					TIMER_config(&TIME2_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
					TIMER_setup(Timer2);
				}
				else if(UART4_CON.UartBrt == Timer4)
				{
					S4CON sh setHigh[6];		//S4 BRT Use Timer4;
					TIMER_config(&TIME4_CON,Time_16BitAuto,LOW,DISABLE,CLOCK_1T,DISABLE,clk,ENABLE);
					TIMER_setup(Timer4);
				}
				else
					return -1;
			}
			else	return -1;	//模式错误
			if(UART4_CON.UartInterrupt == ENABLE)		IE2 sh setHigh[4];		//允许中断
			else																		IE2 sl setLow[4];		//禁止中断
			if(UART4_CON.UartRxEnable == ENABLE)		S4CON |=  (1<<4);	//允许接收
			else																		S4CON &= ~(1<<4);	//禁止接收
			break;
		default:return -1;
	}
	return 0;
}

unsigned char UART_read(unsigned char uart)
{
	switch(uart)
	{
		unsigned char c;
		case 1:
			if (UART1_BUFF_CON.rx_flag == UART1_BUFF_CON.read_flag) 
				return -1;
			else 
				c = UART1_BUFF_CON.rx_buffer[UART1_BUFF_CON.read_flag];
			UART1_BUFF_CON.read_flag = (UART1_BUFF_CON.read_flag + 1) % rxBUFFER_SIZE;
			return c;
		case 2:
			if (UART2_BUFF_CON.rx_flag == UART2_BUFF_CON.read_flag) 
				return -1;
			else 
				c = UART2_BUFF_CON.rx_buffer[UART2_BUFF_CON.read_flag];
			UART2_BUFF_CON.read_flag = (UART2_BUFF_CON.read_flag + 1) % rxBUFFER_SIZE;
			return c;
		case 3:
			if (UART3_BUFF_CON.rx_flag == UART3_BUFF_CON.read_flag) 
				return -1;
			else 
				c = UART3_BUFF_CON.rx_buffer[UART3_BUFF_CON.read_flag];
			UART3_BUFF_CON.read_flag = (UART3_BUFF_CON.read_flag + 1) % rxBUFFER_SIZE;
			return c;
		case 4:
			if (UART4_BUFF_CON.rx_flag == UART4_BUFF_CON.read_flag) 
				return -1;
			else 
				c = UART4_BUFF_CON.rx_buffer[UART4_BUFF_CON.read_flag];
			UART4_BUFF_CON.read_flag = (UART4_BUFF_CON.read_flag + 1) % rxBUFFER_SIZE;
			return c;
		default:return -1;
	}
}

unsigned int UART_available(unsigned char uart)
{
	switch(uart)
	{
		case 1:return ((unsigned int)(rxBUFFER_SIZE + UART1_BUFF_CON.rx_flag - UART1_BUFF_CON.read_flag)) % rxBUFFER_SIZE;
		case 2:return ((unsigned int)(rxBUFFER_SIZE + UART2_BUFF_CON.rx_flag - UART2_BUFF_CON.read_flag)) % rxBUFFER_SIZE;
		case 3:return ((unsigned int)(rxBUFFER_SIZE + UART3_BUFF_CON.rx_flag - UART3_BUFF_CON.read_flag)) % rxBUFFER_SIZE;
		case 4:return ((unsigned int)(rxBUFFER_SIZE + UART4_BUFF_CON.rx_flag - UART4_BUFF_CON.read_flag)) % rxBUFFER_SIZE;
		default:return 0;//error
	}
}

int UART_println(unsigned char uart,unsigned char *string)
{
	xdata int c;
	c = UART_print(uart,string);
	c |= UART_print(uart,"\r\n");
	return c;
}

int UART_print(unsigned char uart,unsigned char *string)
{
	switch(uart)
	{
		case 1:
			for (; *string != 0;	string++)
			{
				UART1_BUFF_CON.tx_buffer[UART1_BUFF_CON.tx_flag]= *string; 
				UART1_BUFF_CON.tx_flag = (UART1_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
				if(UART1_BUFF_CON.busy != 1)
				{
					UART1_BUFF_CON.busy = 1;
					TI=1;
				}
			}
			break;
		case 2:
			for (; *string != 0;	string++)
			{
				UART2_BUFF_CON.tx_buffer[UART2_BUFF_CON.tx_flag]= *string; 
				UART2_BUFF_CON.tx_flag = (UART2_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
				if(UART2_BUFF_CON.busy != 1)
				{
					UART2_BUFF_CON.busy = 1;
					S2CON sh setHigh[1];
				}
			}
			break;
		case 3:
			for (; *string != 0;	string++)
			{
				UART3_BUFF_CON.tx_buffer[UART3_BUFF_CON.tx_flag]= *string; 
				UART3_BUFF_CON.tx_flag = (UART3_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
				if(UART3_BUFF_CON.busy != 1)
				{
					UART3_BUFF_CON.busy = 1;
					S3CON sh setHigh[1];
				}
			}
			break;
		case 4:
			for (; *string != 0;	string++)
			{
				UART4_BUFF_CON.tx_buffer[UART4_BUFF_CON.tx_flag]= *string; 
				UART4_BUFF_CON.tx_flag = (UART4_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
				if(UART4_BUFF_CON.busy != 1)
				{
					UART4_BUFF_CON.busy = 1;
					S4CON sh setHigh[1];
				}
			}
			break;
		default:return -1;
	}
	return 0;
}

int UART_write(unsigned char uart,unsigned char dat)
{
	switch(uart)
	{
		case 1:
			UART1_BUFF_CON.tx_buffer[UART1_BUFF_CON.tx_flag]= dat; 
			UART1_BUFF_CON.tx_flag = (UART1_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
			if(UART1_BUFF_CON.busy != 1)
			{
				UART1_BUFF_CON.busy = 1;
				TI=1;
			}
			break;
		case 2:
			UART2_BUFF_CON.tx_buffer[UART2_BUFF_CON.tx_flag]= dat; 
			UART2_BUFF_CON.tx_flag = (UART2_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
			if(UART2_BUFF_CON.busy != 1)
			{
				UART2_BUFF_CON.busy = 1;
				S2CON sh setHigh[1];
			}
			break;
		case 3:
			UART3_BUFF_CON.tx_buffer[UART3_BUFF_CON.tx_flag]= dat; 
			UART3_BUFF_CON.tx_flag = (UART3_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
			if(UART3_BUFF_CON.busy != 1)
			{
				UART3_BUFF_CON.busy = 1;
				S3CON sh setHigh[1];
			}
			break;
		case 4:
			UART4_BUFF_CON.tx_buffer[UART4_BUFF_CON.tx_flag]= dat; 
			UART4_BUFF_CON.tx_flag = (UART4_BUFF_CON.tx_flag + 1) % txBUFFER_SIZE;
			if(UART4_BUFF_CON.busy != 1)
			{
				UART4_BUFF_CON.busy = 1;
				S4CON sh setHigh[1];
			}
			break;
		default:return -1;
	}
	return 0;
}


int UART_flush(unsigned char uart)
{
	unsigned char i;
		switch(uart)
	{
		case 1:
			for(i=0;i<rxBUFFER_SIZE;i++)
				UART1_BUFF_CON.rx_buffer[i]=0;
			for(i=0;i<txBUFFER_SIZE;i++)
				UART1_BUFF_CON.tx_buffer[i]=0;
			UART1_BUFF_CON.read_flag=0;
			UART1_BUFF_CON.rx_flag=0;
			break;
		case 2:
			for(i=0;i<rxBUFFER_SIZE;i++)
				UART2_BUFF_CON.rx_buffer[i]=0;
			for(i=0;i<txBUFFER_SIZE;i++)
				UART2_BUFF_CON.tx_buffer[i]=0;
			UART1_BUFF_CON.read_flag=0;
			UART1_BUFF_CON.rx_flag=0;
			break;
		case 3:
			for(i=0;i<rxBUFFER_SIZE;i++)
				UART3_BUFF_CON.rx_buffer[i]=0;
			for(i=0;i<txBUFFER_SIZE;i++)
				UART3_BUFF_CON.tx_buffer[i]=0;
			UART1_BUFF_CON.read_flag=0;
			UART1_BUFF_CON.rx_flag=0;
			break;
		case 4:
			for(i=0;i<rxBUFFER_SIZE;i++)
				UART4_BUFF_CON.rx_buffer[i]=0;
			for(i=0;i<txBUFFER_SIZE;i++)
				UART4_BUFF_CON.tx_buffer[i]=0;
			UART1_BUFF_CON.read_flag=0;
			UART1_BUFF_CON.rx_flag=0;
			break;
		default:return -1;
	}
	return 0;
}

void UART_CB1()
{
	if(RI==1)
	{
		UART1_BUFF_CON.rx_buffer[UART1_BUFF_CON.rx_flag] = SBUF; 
		UART1_BUFF_CON.rx_flag = (UART1_BUFF_CON.rx_flag + 1) % rxBUFFER_SIZE;
	}
	if(TI==1)
	{
		if(UART1_BUFF_CON.write_flag == UART1_BUFF_CON.tx_flag)
			UART1_BUFF_CON.busy=0;
		else
		{
			SBUF = UART1_BUFF_CON.tx_buffer[UART1_BUFF_CON.write_flag]; 
			UART1_BUFF_CON.write_flag = (UART1_BUFF_CON.write_flag + 1) % txBUFFER_SIZE;
		}
	}
}

void UART_CB2()
{
	if(S2CON & 0x01)
	{
		UART2_BUFF_CON.rx_buffer[UART2_BUFF_CON.rx_flag] = S2BUF; 
		UART2_BUFF_CON.rx_flag = (UART2_BUFF_CON.rx_flag + 1) % rxBUFFER_SIZE;
	}
	if(S2CON & 0x02)
	{
		if(UART2_BUFF_CON.write_flag == UART2_BUFF_CON.tx_flag)
			UART2_BUFF_CON.busy=0;
		else
		{
			S2BUF = UART2_BUFF_CON.tx_buffer[UART2_BUFF_CON.write_flag]; 
			UART2_BUFF_CON.write_flag = (UART2_BUFF_CON.write_flag + 1) % txBUFFER_SIZE;
		}
	}
}
void UART_CB3()
{
	if(S3CON & 0x01)
	{
		UART3_BUFF_CON.rx_buffer[UART3_BUFF_CON.rx_flag] = S3BUF; 
		UART3_BUFF_CON.rx_flag = (UART3_BUFF_CON.rx_flag + 1) % rxBUFFER_SIZE;
	}
	if(S3CON & 0x02)
	{
		if(UART3_BUFF_CON.write_flag == UART3_BUFF_CON.tx_flag)
			UART3_BUFF_CON.busy=0;
		else
		{
			S3BUF = UART3_BUFF_CON.tx_buffer[UART3_BUFF_CON.write_flag]; 
			UART3_BUFF_CON.write_flag = (UART3_BUFF_CON.write_flag + 1) % txBUFFER_SIZE;
		}
	}
}

void UART_CB4()
{
	if(S4CON & 0x01)
	{
		UART4_BUFF_CON.rx_buffer[UART4_BUFF_CON.rx_flag] = S4BUF; 
		UART4_BUFF_CON.rx_flag = (UART4_BUFF_CON.rx_flag + 1) % rxBUFFER_SIZE;
	}
	if(S4CON & 0x02)
	{
		if(UART4_BUFF_CON.write_flag == UART4_BUFF_CON.tx_flag)
			UART4_BUFF_CON.busy=0;
		else
		{
			S4BUF = UART4_BUFF_CON.tx_buffer[UART4_BUFF_CON.write_flag]; 
			UART4_BUFF_CON.write_flag = (UART4_BUFF_CON.write_flag + 1) % txBUFFER_SIZE;
		}
	}
}