#ifndef __TSL2561_H__
#define __TSL2561_H__

#include "hand.h"

#define TSL2561_ADDR_0 0x52 //address with '0' shorted on board (0x29)
#define TSL2561_ADDR 0x72   //default address (0x39)
#define TSL2561_ADDR_1 0x92 //address with '1' shorted on board (0x49)

typedef struct
{
    unsigned char address;
    unsigned char gain;
    unsigned char time;
    unsigned int data0;
    unsigned int data1;
    double lux;
} TSL2561_config;

extern xdata TSL2561_config TSL2561;

extern void TSL2561_begin(unsigned char address, unsigned char gain, unsigned char time);
extern void TSL2561_opwerOn();
extern void TSL2561_opwerOff();
extern void TSL2561_setTiming();
extern int TSL2561_readData();
extern void TSL2561_printData(unsigned char uart);

#endif