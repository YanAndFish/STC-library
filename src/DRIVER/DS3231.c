#include "DS3231.h"
#include "interrupt.h"
#include "HAL_UART.h"
#include "HAL_GPIO.h"
#include "HAL_IIC.h"

xdata DS3231_config DS3231;

void DS3231_readDateTime()
{
    unsigned char time[6];
    int i = 0;

    disableInterrupt();
    IIC_Start();
    IIC_Write(0xd0);
    IIC_Write(0x00);
    IIC_Start();
    IIC_Write(0xd1);
    for (i = 0; i < 6; i++)
    {
        time[i] = IIC_read();
        IIC_writeAsk();
    }
    time[6] = IIC_read();
    IIC_writeNoAsk();
    IIC_Stop();
    enableInterrupt();

    DS3231.year = 2000 + (((time[6] & 0xf0) >> 4) * 10) + (time[6] & 0x0f);
    DS3231.month = (((time[5] & 0x70) >> 4) * 10) + (time[5] & 0x0f);
    DS3231.day = (((time[4] & 0xf0) >> 4) * 10) + (time[4] & 0x0f);
    if (time[2] & 0x40) //12
    {
        if (time[2] & 0x10)
            DS3231.AMPM = 1;
        else
            DS3231.AMPM = 0;
        DS3231.hour = (((time[2] & 0x10) >> 4) * 10) + (time[2] & 0x0f);
    }
    else //24
    {
        DS3231.AMPM = 24;
        DS3231.hour = (((time[2] & 0x30) >> 4) * 10) + (time[2] & 0x0f);
    }
    DS3231.minute = (((time[1] & 0xf0) >> 4) * 10) + (time[1] & 0x0f);
    DS3231.second = (((time[0] & 0xf0) >> 4) * 10) + (time[0] & 0x0f);
}

void DS3231_printDateTime(unsigned char uart)
{
    UART_write(uart, 0x30 + (DS3231.year / 1000));
    UART_write(uart, 0x30 + (DS3231.year % 1000 / 100));
    UART_write(uart, 0x30 + (DS3231.year % 100 / 10));
    UART_write(uart, 0x30 + (DS3231.year % 10));
    UART_print(uart, ".");
    UART_write(uart, 0x30 + (DS3231.month / 10));
    UART_write(uart, 0x30 + (DS3231.month % 10));
    UART_print(uart, ".");
    UART_write(uart, 0x30 + (DS3231.day / 10));
    UART_write(uart, 0x30 + (DS3231.day % 10));
    UART_print(uart, " ");
    if (DS3231.AMPM == 0)
        UART_print(uart, "AM ");
    else if (DS3231.AMPM == 1)
        UART_print(uart, "PM ");
    UART_write(uart, 0x30 + (DS3231.hour / 10));
    UART_write(uart, 0x30 + (DS3231.hour % 10));
    UART_print(uart, ":");
    UART_write(uart, 0x30 + (DS3231.minute / 10));
    UART_write(uart, 0x30 + (DS3231.minute % 10));
    UART_print(uart, ":");
    UART_write(uart, 0x30 + (DS3231.second / 10));
    UART_write(uart, 0x30 + (DS3231.second % 10));
    UART_print(uart, "\r\n");
}

void DS3231_writeDateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
{
    unsigned char ye, mo, da, ho, mi, se;

    ye = ((year % 100 / 10) << 4) + (year % 10);
    mo = ((month / 10) << 4) + (month % 10);
    da = ((day / 10) << 4) + (day % 10);
    ho = ((hour / 10) << 4) + (hour % 10);
    mi = ((minute / 10) << 4) + (minute % 10);
    se = ((second / 10) << 4) + (second % 10);

    disableInterrupt();
    IIC_Start();
    IIC_Write(0xd0);
    IIC_Write(0x00);
    IIC_Write(se);
    IIC_Write(mi);
    IIC_Write(ho);
    IIC_Write(0x01);
    IIC_Write(da);
    IIC_Write(mo);
    IIC_Write(ye);
    IIC_Stop();
    enableInterrupt();
}