#ifndef __HAND_H__
#define __HAND_H__

#ifdef STC15
#include "stc15.h"
#endif

#ifdef STC8
#include "stc8.h"
#define INT_CLKO INTCLKO
#define	T3T4M T4T3M
#define CLK_DIV AUXR2
#endif

#define UNUSED(X)  ((void)(X))
	
//typedef u8 unsigned char;

#endif