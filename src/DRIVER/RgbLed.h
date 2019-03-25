#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include "hand.h"

//伽马值校正
//#define GAMMA_10
//#define GAMMA_15
//#define GAMMA_20
#define GAMMA_26
//#define GAMMA_30
//#define GAMMA_NOT


#define ENABLE	1
#define DISABLE	0

typedef struct
{
	unsigned char redPort;
	unsigned char greenPort;
	unsigned char bluePort;
} RgbLed;

extern int RGBLED_begin(RgbLed *led,unsigned char rport,unsigned char gport,unsigned char bport);
extern void RGBLED_set(RgbLed *led,unsigned int red,unsigned int green,unsigned int blue);
extern void RGBLED_end(RgbLed *led);

#ifdef GAMMA_30
extern code unsigned int gamma[1000];
#endif

#ifdef GAMMA_26
extern code unsigned int gamma[1000];
#endif

#ifdef GAMMA_20
extern code unsigned int gamma[1000];
#endif

#ifdef GAMMA_15
extern code unsigned int gamma[1000];
#endif

#ifdef GAMMA_10
extern code unsigned int gamma[1000];
#endif

#endif