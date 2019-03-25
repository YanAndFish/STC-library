#ifndef __DHT11_H__
#define __DHT11_H__

#include "hand.h"

typedef struct
{
	unsigned char pin;
	unsigned int temp;
	unsigned int humidity;
} DHT11;

extern void DHT11_get(DHT11 *dht);
extern void DHT11_begin(DHT11 *dht,unsigned char pin);
extern unsigned char DHT11_com(DHT11 *dht);

#endif