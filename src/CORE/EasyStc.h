/* please use encoding of Chinese GB2312(Simplified)
   EasyStc库文件 版本V0.1
	 使用请包含本头文件并在设置中定义参数
	 定义时钟频率 例：#define CLOCK 11059200U
	 定义单片机类型，支持 STC12 STC15 STC8 例:  #define STC15
*/
#ifndef __EASYSTC_H__
#define __EASYSTC_H__

//#ifndef STC
#if defined(STC15)||defined(STC12)||defined(STC8)
#else
#define STC15
#endif

#ifndef SCLOCK
#define SCLOCK 12000000L
#endif

#ifndef NOT_MAIN
unsigned long SetClock()
{
	return SCLOCK;
}
#else
extern unsigned long SetClock();
#endif

#define CLOCK SetClock()

#ifdef STC15
#include "stc15.h"
#endif

#ifdef STC8
#include "stc8.h"
#endif

#ifdef STC12
#include "STC12C5A60S2.h"
#endif

#define INOUT			0//准双向
#define OUTPUT		1//推挽输出
#define INPUT			2//输入，高阻
#define OUTPUT_OD	3//开漏输出
#define HIGH			1
#define LOW				0
#define UNUSED(X)  ((void)(X))
/*--------------------------------------
core functions
*/
void setup();
void loop();
/*--------------------------------------
delay functions
*/
void delay(unsigned int);
/*--------------------------------------
gpio functions
*/
int pinMode(unsigned char,unsigned char);
/*--------------------------------------
rgb_pixel functions
*/
//void pixel_begin(unsigned char,unsigned char);
//unsigned char pixel_color();
#ifndef NOT_MAIN
void main(){setup();UNUSED(CLOCK);while(1){loop();}}
#endif
#endif