#ifndef __KEY_BOARD_H__
#define __KEY_BOARD_H__

#include "hand.h"


#define KEYBOARD_ONCE		0
#define KEYBOARD_REPEAT	1

typedef struct
{
	unsigned char port;
	unsigned char mode;
	unsigned int time;
	unsigned char keynum;
} KeyBoard;

extern int KEYBOARD_begin(KeyBoard *kb,unsigned char port,unsigned char mode,unsigned int time);
extern void KEYBOARD_getkey(KeyBoard *kb);

#endif