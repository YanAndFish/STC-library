/* please use encoding of Chinese GB2312(Simplified)
   EasyStc���ļ� �汾V0.1
	 ʹ���������ͷ�ļ����������ж������
	 ����ʱ��Ƶ�� ����#define CLOCK 11059200U
	 ���嵥Ƭ�����ͣ�֧�� STC12 STC15 STC8 ��:  #define STC15
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

#define INOUT			0//׼˫��
#define OUTPUT		1//�������
#define INPUT			2//���룬����
#define OUTPUT_OD	3//��©���
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