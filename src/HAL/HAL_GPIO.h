#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "hand.h"

#define INOUT			0//准双向
#define OUTPUT		1//推挽输出
#define INPUT			2//输入，高阻
#define OUTPUT_OD	3//开漏输出

#define HIGH			1
#define LOW				0

	
extern int pinMode(unsigned char,unsigned char);
extern int portMode(unsigned char port,unsigned mode);
extern void writePort(unsigned char port,unsigned char dat);
extern unsigned char readPort(unsigned char port);
extern bit readPin(unsigned char pin);
extern void writePin(unsigned char pin,unsigned char dat);

#endif