/*
 * keypad.h
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define C0		"PB0"
#define C1		"PD7"
#define C2		"PD6"
#define C3		"PD5"
#define R0		"PB4"
#define R1		"PB3"
#define R2		"PB2"
#define R3		"PB1"

typedef struct{
	uint8_t* ddr[8];
	uint8_t  pin[8];
	uint8_t* port[8];
	uint8_t* pinr[4];
}keypad;

typedef keypad * keypadPtr;

#define PXN char*

#define char2dec(a) (int) (a - '0')

keypad keypad_assign(PXN row0_, PXN row1_, PXN row2_, PXN row3_, PXN col0_, PXN col1_, PXN col2_, PXN col3_);

void keypad_initx(keypadPtr keypadptr_);

void keypad_idlex(keypadPtr keypadptr_);

void keypad_startx(keypadPtr keypadptr_);

char keypad_scanKeyx(keypadPtr keypadptr_);

char keypad_readUntilInputx(keypadPtr keypadptr_);

#define keypad_init(key) keypad_initx(&key)

#define keypad_idle(key) keypad_idlex(&key)

#define keypad_start(key) keypad_startx(&key)

#define keypad_scanKey(key) keypad_scanKeyx(&key)

#define keypad_readUntilInput(key) keypad_readUntilInputx(&key)

uint8_t compare_pwds(char* psw1, char* psw2);

#endif /* KEYPAD_H_ */