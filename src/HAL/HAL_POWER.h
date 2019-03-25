#ifndef __HAL_POWER_H__
#define __HAL_POWER_H__

#include "hand.h"

#define DIV_1		0
#define DIV_2		1
#define DIV_4		2
#define DIV_8		3
#define DIV_16	4
#define DIV_32	5
#define DIV_64	6
#define DIV_128	7

#define ENABLE 1
#define DISABLE 0

extern int POWER_div(unsigned char div);
extern void POWER_idle();
extern void POWER_down(unsigned char clock,unsigned int ms);
extern void POWER_reset();
extern int POWER_autoReboot();
extern void POWER_Reboot();
	
#endif