#include "HAL_GPIO.h"
#include "bitctrl.h"

int pinMode(unsigned char pin,unsigned char mode)
{
	switch(pin/10)
	{
		case 0:
			switch(mode){
				case INOUT:	
					P0M1 sl setLow[pin%10];
					P0M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P0M1 sl setLow[pin%10];
					P0M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P0M1 sh setHigh[pin%10];
					P0M0 sl setHigh[pin%10];
				break;
				case INPUT:
					P0M1 sh setHigh[pin%10];
					P0M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 1:
			switch(mode){
				case INOUT:	
					P1M1 sl setLow[pin%10];
					P1M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P1M1 sl setLow[pin%10];
					P1M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P1M1 sh setHigh[pin%10];
					P1M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P1M1 sh setHigh[pin%10];
					P1M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 2:
			switch(mode){
				case INOUT:	
					P2M1 sl setLow[pin%10];
					P2M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P2M1 sl setLow[pin%10];
					P2M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P2M1 sh setHigh[pin%10];
					P2M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P2M1 sh setHigh[pin%10];
					P2M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 3:
			switch(mode){
				case INOUT:	
					P3M1 sl setLow[pin%10];
					P3M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P3M1 sl setLow[pin%10];
					P3M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P3M1 sh setHigh[pin%10];
					P3M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P3M1 sh setHigh[pin%10];
					P3M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 4:
			switch(mode){
				case INOUT:	
					P4M1 sl setLow[pin%10];
					P4M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P4M1 sl setLow[pin%10];
					P4M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P4M1 sh setHigh[pin%10];
					P4M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P4M1 sh setHigh[pin%10];
					P4M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 5:
			switch(mode){
				case INOUT:	
					P5M1 sl setLow[pin%10];
					P5M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P5M1 sl setLow[pin%10];
					P5M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P5M1 sh setHigh[pin%10];
					P5M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P5M1 sh setHigh[pin%10];
					P5M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 6:
			switch(mode){
				case INOUT:	
					P6M1 sl setLow[pin%10];
					P6M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P6M1 sl setLow[pin%10];
					P6M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P6M1 sh setHigh[pin%10];
					P6M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P6M1 sh setHigh[pin%10];
					P6M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		case 7:
			switch(mode){
				case INOUT:	
					P7M1 sl setLow[pin%10];
					P7M0 sl setLow[pin%10];
				break;
				case OUTPUT:
					P7M1 sl setLow[pin%10];
					P7M0 sh setHigh[pin%10];
				break;
				case OUTPUT_OD:
					P7M1 sh setHigh[pin%10];
					P7M0 sh setHigh[pin%10];
				break;
				case INPUT:
					P7M1 sh setHigh[pin%10];
					P7M0 sl setLow[pin%10];
				break;
				default:return -1;
				}		
		break;
		default:return -1;
	}
	return 0;
}


int portMode(unsigned char port,unsigned mode)
{
	switch(port)
	{
		case 0:
			switch(mode){
				case INOUT:	
					P0M1 =0x00;
					P0M0 =0x00;
				break;
				case OUTPUT:
					P0M1 =0x00;
					P0M0 =0xff;
				break;
				case OUTPUT_OD:
					P0M1 =0xff;
					P0M0 =0xff;
				break;
				case INPUT:
					P0M1 =0xff;
					P0M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 1:
			switch(mode){
				case INOUT:	
					P1M1 =0x00;
					P1M0 =0x00;
				break;
				case OUTPUT:
					P1M1 =0x00;
					P1M0 =0xff;
				break;
				case OUTPUT_OD:
					P1M1 =0xff;
					P1M0 =0xff;
				break;
				case INPUT:
					P1M1 =0xff;
					P1M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 2:
			switch(mode){
				case INOUT:	
					P2M1 =0x00;
					P2M0 =0x00;
				break;
				case OUTPUT:
					P2M1 =0x00;
					P2M0 =0xff;
				break;
				case OUTPUT_OD:
					P2M1 =0xff;
					P2M0 =0xff;
				break;
				case INPUT:
					P2M1 =0xff;
					P2M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 3:
			switch(mode){
				case INOUT:	
					P3M1 =0x00;
					P3M0 =0x00;
				break;
				case OUTPUT:
					P3M1 =0x00;
					P3M0 =0xff;
				break;
				case OUTPUT_OD:
					P3M1 =0xff;
					P3M0 =0xff;
				break;
				case INPUT:
					P3M1 =0xff;
					P3M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 4:
			switch(mode){
				case INOUT:	
					P4M1 =0x00;
					P4M0 =0x00;
				break;
				case OUTPUT:
					P4M1 =0x00;
					P4M0 =0xff;
				break;
				case OUTPUT_OD:
					P4M1 =0xff;
					P4M0 =0xff;
				break;
				case INPUT:
					P4M1 =0xff;
					P4M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 5:
			switch(mode){
				case INOUT:	
					P5M1 =0x00;
					P5M0 =0x00;
				break;
				case OUTPUT:
					P5M1 =0x00;
					P5M0 =0xff;
				break;
				case OUTPUT_OD:
					P5M1 =0xff;
					P5M0 =0xff;
				break;
				case INPUT:
					P5M1 =0xff;
					P5M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 6:
			switch(mode){
				case INOUT:	
					P6M1 =0x00;
					P6M0 =0x00;
				break;
				case OUTPUT:
					P6M1 =0x00;
					P6M0 =0xff;
				break;
				case OUTPUT_OD:
					P6M1 =0xff;
					P6M0 =0xff;
				break;
				case INPUT:
					P6M1 =0xff;
					P6M0 =0x00;
				break;
				default:return -1;
				}
			break;
		case 7:	
			switch(mode){
				case INOUT:	
					P7M1 =0x00;
					P7M0 =0x00;
				break;
				case OUTPUT:
					P7M1 =0x00;
					P7M0 =0xff;
				break;
				case OUTPUT_OD:
					P7M1 =0xff;
					P7M0 =0xff;
				break;
				case INPUT:
					P7M1 =0xff;
					P7M0 =0x00;
				break;
				default:return -1;
				}
			break;
		default:return -1;
	}
	return 0;
}

void writePort(unsigned char port,unsigned char dat)
{
		switch(port)
	{
		case 0:P0=dat;break;
		case 1:P1=dat;break;
		case 2:P2=dat;break;
		case 3:P3=dat;break;
		case 4:P4=dat;break;
		case 5:P5=dat;break;
		case 6:P6=dat;break;
		case 7:P7=dat;break;
		default:break;
	}
}

unsigned char readPort(unsigned char port)
{
	xdata unsigned char c;
		switch(port)
	{
		case 0:c=P0;break;
		case 1:c=P1;break;
		case 2:c=P2;break;
		case 3:c=P3;break;
		case 4:c=P4;break;
		case 5:c=P5;break;
		case 6:c=P6;break;
		case 7:c=P7;break;
		default:break;
	}
	return c;
}

void writePin(unsigned char pin,unsigned char dat)
{
	switch(pin/10)
	{
		case 0:if(dat==0)P0 sl setLow[pin%10];else P0 sh setHigh[pin%10];break;
		case 1:if(dat==0)P1 sl setLow[pin%10];else P1 sh setHigh[pin%10];break;
		case 2:if(dat==0)P2 sl setLow[pin%10];else P2 sh setHigh[pin%10];break;
		case 3:if(dat==0)P3 sl setLow[pin%10];else P3 sh setHigh[pin%10];break;
		case 4:if(dat==0)P4 sl setLow[pin%10];else P4 sh setHigh[pin%10];break;
		case 5:if(dat==0)P5 sl setLow[pin%10];else P5 sh setHigh[pin%10];break;
		case 6:if(dat==0)P6 sl setLow[pin%10];else P6 sh setHigh[pin%10];break;
		case 7:if(dat==0)P7 sl setLow[pin%10];else P7 sh setHigh[pin%10];break;
		default:break;
	}
}

bit readPin(unsigned char pin)
{
	xdata unsigned char c;
	switch(pin/10)
	{
		case 0:c=P0 & setHigh[pin%10];break;
		case 1:c=P1 & setHigh[pin%10];break;
		case 2:c=P2 & setHigh[pin%10];break;
		case 3:c=P3 & setHigh[pin%10];break;
		case 4:c=P4 & setHigh[pin%10];break;
		case 5:c=P5 & setHigh[pin%10];break;
		case 6:c=P6 & setHigh[pin%10];break;
		case 7:c=P7 & setHigh[pin%10];break;
		default:break;
	}
	if(c==0)
		return 0;
	else
		return 1;
}