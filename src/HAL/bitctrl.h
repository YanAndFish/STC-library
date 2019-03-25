#ifndef __BITCTRL_H__
#define __BITCTRL_H__

#define BIT_VOID 0x00
#define	BIT_0		0x01
#define	BIT_1		0x02
#define	BIT_2		0x04
#define	BIT_3		0x08
#define	BIT_4		0x10
#define	BIT_5		0x20
#define	BIT_6		0x40
#define	BIT_7		0x80
#define BIT_ALL 0xff

#define sh |=
#define sl &=

extern code unsigned char setHigh[8];
extern code unsigned char setLow[8];

#endif