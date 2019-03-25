#include "HAL_WDT.h"
#include "bitctrl.h"

int WDT_begin(unsigned char prescale,unsigned char idle)
{
	if(prescale < WDT_2 || prescale > WDT_256)
		return -1;
	WDT_CONTR=0x00;
	WDT_CONTR |= prescale;
	if(idle==1)
		WDT_CONTR sh setHigh[3];
	else if(idle==0)
		WDT_CONTR sl setLow[3];
	else
		return -1;
	WDT_CONTR sh setHigh[5];
	return 0;
}
void WDT_end()
{
	WDT_CONTR sl setLow[5];
}
void WDT_clean()
{
	WDT_CONTR sh setHigh[4];
}