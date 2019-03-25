#ifndef __DS3232_H__
#define __DS3232_H__

#include "hand.h"

typedef struct
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned char AMPM; //0:AM 1:PM 24:24hour
} DS3231_config;

extern xdata DS3231_config DS3231;

extern void DS3231_readDateTime();
extern void DS3231_writeDateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
extern void DS3231_printDateTime(unsigned char uart);

#endif