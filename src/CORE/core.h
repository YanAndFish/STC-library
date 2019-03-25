#ifndef __CORE_H__
#define __CORE_H__

#include "hand.h"

#include "HAL_GPIO.h"
#include "HAL_DELAY.h"
#include "HAL_TIMER.h"
#include "HAL_EXTI.h"
#include "HAL_UART.h"
#include "HAL_PWM.h"
#include "HAL_ADC.h"
#include "HAL_IAP.h"
#include "HAL_IIC.h"
#include "HAL_POWER.h"
#include "HAL_WDT.h"
#include "HAL_PCA.h"
#include "HAL_SPI.h"
#include "interrupt.h"

extern void setup();
extern void loop();

#endif