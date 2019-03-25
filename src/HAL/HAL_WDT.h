#ifndef __HAL_WDT_H__
#define __HAL_WDT_H__

#include "hand.h"

#define WDT_2			0 //32.7ms
#define WDT_4			1 //65.5ms
#define WDT_8			2 //131ms
#define WDT_16		3 //262.1ms
#define WDT_32		4 //624.3ms
#define WDT_64		5 //1048.6ms
#define WDT_128		6 //2097.2ms
#define WDT_256		7 //4194.3ms

#define ENABLE		1
#define DISABLE		0

extern int WDT_begin(unsigned char prescale,unsigned char idle);
extern void WDT_end();
extern void WDT_clean();

#endif