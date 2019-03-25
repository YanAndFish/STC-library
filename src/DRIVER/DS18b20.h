#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "hand.h"

typedef struct
{
	unsigned char pin;//Òý½Å
	unsigned int temp;//½á¹û
} DS18B20;

extern void DS18B20_begin(DS18B20 *ds,unsigned char pin);
extern int DS18B20_init(DS18B20 *ds);
extern unsigned char DS18B20_read(DS18B20 *ds);
extern void DS18B20_write(DS18B20 *ds,unsigned char dat);
extern void DS18B20_ReadTempCom(DS18B20 *ds);
extern void DS18B20_ChangTemp(DS18B20 *ds);
extern void DS18B20_getTemp(DS18B20 *ds);

#endif
