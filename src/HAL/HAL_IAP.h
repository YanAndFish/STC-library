#ifndef __HAL_IAP_H__
#define __HAL_IAP_H__

#include "hand.h"
#include "intrins.h"

#define ADDRESS 0xE000U;

#define		IapStandby()	IAP_CMD = 0		//ISP¿ÕÏĞÃüÁî£¨½ûÖ¹£©
#define		IapRead()			IAP_CMD = 1		//ISP¶Á³öÃüÁî
#define		IapWrite()		IAP_CMD = 2		//ISPĞ´ÈëÃüÁî
#define		IapErase()		IAP_CMD = 3		//ISP²Á³ıÃüÁî
#define 	IapTrig()	IAP_TRIG = 0x5A,	IAP_TRIG = 0xA5		//ISP´¥·¢ÃüÁî

#define IAP_EN							(1<<7)
#define IAP_SWBS						(1<<6)
#define IAP_SWRST						(1<<5)
#define IAP_CMD_FAIL				(1<<4)
#define	IAP_WAIT_FREQUENCY	1//24Mhz


typedef struct
{
	unsigned int IapSectorNum;
	unsigned char IapSectorBuffer[512];
} IapConfig;

extern xdata IapConfig IAP_CON;

extern void IAP_reads(unsigned int EE_address,unsigned char *DataAddress,unsigned int number);
extern void IAP_disable();
extern unsigned char IAP_read(unsigned int EE_address);
extern void IAP_erase(unsigned int EE_address);
extern void IAP_write(unsigned int EE_address,unsigned char Data);
extern void IAP_update();


#endif