#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "hand.h"

#define rxBUFFER_SIZE 110
#define txBUFFER_SIZE	70

#define Uart1 1
#define Uart2 2
#define Uart3 3
#define Uart4 4

#define	UartShiftRight	0		//同步移位输出
#define	Uart8bitBRT	(1<<6)	//8位数据,可变波特率
#define	Uart9bit		(2<<6)	//9位数据,固定波特率
#define	Uart9bitBRT	(3<<6)	//9位数据,可变波特率

#define ENABLE	1
#define DISABLE 0

#define HIGH	1
#define LOW		0

typedef struct{
	unsigned char rx_buffer[rxBUFFER_SIZE];
	unsigned char tx_buffer[txBUFFER_SIZE];
	unsigned char read_flag;
	unsigned char write_flag;
	unsigned char rx_flag;
	unsigned char tx_flag;
	unsigned char busy;
} UartBufferConfig;

typedef struct{
	unsigned char	UartMode;			//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	unsigned char	UartBrt;		//使用波特率发生器,   BRT_Timer1,BRT_Timer2
	unsigned long	UartBaudRate;		//波特率,       ENABLE,DISABLE
	unsigned char	UartMoreCommunicate;	//多机通讯允许, ENABLE,DISABLE
	unsigned char	UartRxEnable;		//允许接收,   ENABLE,DISABLE
	unsigned char	BaudRateDouble;		//波特率加倍, ENABLE,DISABLE
	unsigned char	UartInterrupt;		//中断控制,   ENABLE,DISABLE
	unsigned char	UartPolity;		//优先级,     PolityLow,PolityHigh
	unsigned char	UartShort;	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
} UartConfig;

extern xdata UartBufferConfig UART1_BUFF_CON,UART2_BUFF_CON,UART3_BUFF_CON,UART4_BUFF_CON;
extern xdata UartConfig UART1_CON,UART2_CON,UART3_CON,UART4_CON;

extern int UART_begin(unsigned char uart,unsigned long baud);
extern int UART_end(unsigned char uart);
extern int UART_addTask(unsigned char uart,void(*hander)());
extern void UART_config(UartConfig *uart,unsigned char	UartMode,unsigned char	UartBrt,unsigned long	UartBaudRate,
unsigned char	UartMoreCommunicate,unsigned char	UartRxEnable,unsigned char	BaudRateDouble,unsigned char	UartInterrupt,
unsigned char	UartPolity,unsigned char	UartShort);
extern int UART_setup(unsigned char uart);
extern unsigned char UART_read(unsigned char uart);
extern unsigned int UART_available(unsigned char uart);
extern int UART_print(unsigned char uart,unsigned char *string);
extern int UART_println(unsigned char uart,unsigned char *string);
extern int UART_write(unsigned char uart,unsigned char dat);
extern int UART_flush(unsigned char uart);
extern void UART_CB1();
extern void UART_CB2();
extern void UART_CB3();
extern void UART_CB4();
#endif