#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

#define ESCAPE 0x01 //00000001
#define UCASE 0x02	//00000010
#define CTRL 0x04	//00000100
#define ALT 0x08	//00001000
#define DEL 0x10	//00010000
#define FN 0x20		//00100000
#define ACCENT 0x40	//01000000

#define CLEAR_BREAK_BIT(A) ((A) & 0x7F)
#define SHIFT_PRESSED() (kbFlags & UCASE)
#define IS_BREAK(A) ((A) & 0x80)
#define IS_ESCAPE() (kbFlags & ESCAPE)
#define IS_CTRL() (kbFlags & CTRL)
#define IS_ALT() (kbFlags & ALT)
#define IS_DEL() (kbFlags & DEL)
#define F_PRESSED(i) ((fKeys >> (i)) & 1)
#define IS_ACCENT() (kbFlags & ACCENT)
#define IS_SPANISH() (kbLayout)

#define EN 0
#define SP 1

char kbLayout = 0;
char kbFlags;
char fKeys;
char erase;

void handleScanCode(unsigned char scanCode);
int checkSpecialKey(unsigned char scanCode);
char translateSc(unsigned char scanCode);

void init_keyboard();

#endif
