#include "TSL2561.h"
#include "interrupt.h"
#include "HAL_GPIO.h"
#include "HAL_IIC.h"
#include "HAL_UART.h"
#include <math.h>

xdata TSL2561_config TSL2561;

void TSL2561_begin(unsigned char address, unsigned char gain, unsigned char time)
{
    TSL2561.address = address;
    TSL2561.gain = gain;
    TSL2561.time = time;
    TSL2561_setTiming();
}

void TSL2561_opwerOn()
{
    disableInterrupt();
    IIC_Start();
    IIC_Write(TSL2561.address);
    IIC_Write(0x80);
    IIC_Write(0x03);
    IIC_Stop();
    enableInterrupt();
}

void TSL2561_powerOff()
{
    disableInterrupt();
    IIC_Start();
    IIC_Write(TSL2561.address);
    IIC_Write(0x80);
    IIC_Write(0x00);
    IIC_Stop();
    enableInterrupt();
}

// If time = 0, integration will be 13.7ms
// If time = 1, integration will be 101ms
// If time = 2, integration will be 402ms
// If time = 3, use manual start / stop to perform your own integration

// If gain = false (0), device is set to low gain (1X)
// If gain = high (1), device is set to high gain (16X)
void TSL2561_setTiming()
{
    unsigned char dat = 0;

    if (TSL2561.gain != 0)
        dat = 0x10;
    else
        dat = 0x00;
    dat += TSL2561.time;

    disableInterrupt();
    IIC_Start();
    IIC_Write(TSL2561.address);
    IIC_Write(0x81);
    IIC_Write(dat);
    IIC_Stop();
    enableInterrupt();
}

int TSL2561_readData()
{
    unsigned char low0 = 0, high0 = 0, low1 = 0, high1 = 0;
    double ratio, d0, d1;
    unsigned int ms;

    disableInterrupt();
    IIC_Start();
    IIC_Write(TSL2561.address);
    IIC_Write(0x8c);
    IIC_Start();
    IIC_Write(TSL2561.address + 1); //readMode
    low0 = IIC_read();
    IIC_writeAsk();
    high0 = IIC_read();
    IIC_writeAsk();
    low1 = IIC_read();
    IIC_writeAsk();
    high1 = IIC_read();
    IIC_writeNoAsk();
    IIC_Stop();
    enableInterrupt();
    TSL2561.data1 = high1;
    TSL2561.data1 <<= 8;
    TSL2561.data1 += low1;
    TSL2561.data0 = high0;
    TSL2561.data0 <<= 8;
    TSL2561.data0 += low0;

    // Determine if either sensor saturated (0xFFFF)
    // If so, abandon ship (calculation will not be accurate)
    if ((TSL2561.data0 == 0xFFFF) || (TSL2561.data1 == 0xFFFF))
    {
        //TSL2561.lux = 0.0;
        return -1;
    }

    // Convert from unsigned integer to floating point
    d0 = TSL2561.data0;
    d1 = TSL2561.data1;

    // We will need the ratio for subsequent calculations
    ratio = d1 / d0;

    switch (TSL2561.time)
    {
    case 0:
        ms = 14;
        break;
    case 1:
        ms = 101;
        break;
    case 2:
    default:
        ms = 402;
        break;
    }
    // Normalize for integration time
    d0 *= (402.0 / ms);
    d1 *= (402.0 / ms);

    // Normalize for gain
    if (TSL2561.gain != 0)
    {
        d0 /= 16;
        d1 /= 16;
    }

    // Determine lux per datasheet equations:
    if (ratio < 0.5)
    {
        TSL2561.lux = 0.0304 * d0 - 0.062 * d0 * pow(ratio, 1.4);
        return 0;
    }
    if (ratio < 0.61)
    {
        TSL2561.lux = 0.0224 * d0 - 0.031 * d1;
        return 0;
    }
    if (ratio < 0.80)
    {
        TSL2561.lux = 0.0128 * d0 - 0.0153 * d1;
        return 0;
    }
    if (ratio < 1.30)
    {
        TSL2561.lux = 0.00146 * d0 - 0.00112 * d1;
        return 0;
    }

    TSL2561.lux = 0.0;
    return 0;
}

void TSL2561_printData(unsigned char uart)
{
    unsigned long dat;

    dat = (unsigned long)(TSL2561.lux * 100);
    UART_print(uart, "data0: ");
    UART_write(uart, 0x30 + (TSL2561.data0 / 10000));
    UART_write(uart, 0x30 + (TSL2561.data0 % 10000 / 1000));
    UART_write(uart, 0x30 + (TSL2561.data0 % 1000 / 100));
    UART_write(uart, 0x30 + (TSL2561.data0 % 100 / 10));
    UART_write(uart, 0x30 + (TSL2561.data0 % 10));
    UART_print(uart, " data1: ");
    UART_write(uart, 0x30 + (TSL2561.data1 / 10000));
    UART_write(uart, 0x30 + (TSL2561.data1 % 10000 / 1000));
    UART_write(uart, 0x30 + (TSL2561.data1 % 1000 / 100));
    UART_write(uart, 0x30 + (TSL2561.data1 % 100 / 10));
    UART_write(uart, 0x30 + (TSL2561.data1 % 10));
    UART_print(uart, " Lux: ");
    UART_write(uart, 0x30 + (dat / 100000));
    UART_write(uart, 0x30 + (dat % 100000 / 10000));
    UART_write(uart, 0x30 + (dat % 10000 / 1000));
    UART_write(uart, 0x30 + (dat % 1000 / 100));
    UART_print(uart, ".");
    UART_write(uart, 0x30 + (dat % 100 / 10));
    UART_write(uart, 0x30 + (dat % 10));
    UART_print(uart, "\r\n");
}