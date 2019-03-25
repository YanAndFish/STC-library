#include "HAL_DELAY.h"


void delay(unsigned int ms)
{	
		unsigned long clo;
    do{  
        clo=CLOCK/63000;  
        while(--clo); 
    }while(--ms); 
	
}

#ifdef STC15 

void delay_10us(unsigned int us)
{
	unsigned char i;
	do
	{
		i = 56;
		while (--i);
	}while(--us);
}

#endif

#ifdef STC8

void delay_10us(unsigned int us)
{
	unsigned char i;
	do
	{
		i = 78;
		while (--i);
	}while(--us);
}

#endif