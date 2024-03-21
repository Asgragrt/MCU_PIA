/*
 * lcdSIPO.h
 *
 * Created: 25/11/2022 04:41:30 p. m.
 *  Author: Asgragrt
 */ 


#ifndef LCDSIPO_H_
#define LCDSIPO_H_

#define RSpin	  "PC3"
#define Epin	  "PC4"
#define signalpin "PC0"
#define clkpin	  "PC2"
#define latchpin  "PC1"

typedef struct{
	uint8_t* ddr[5];
	uint8_t pin[5];
	uint8_t* port[5];
}lcdSIPO;

typedef lcdSIPO * lcdSIPOPtr;

#define PXN char*

#define char2dec(a) (int) (a - '0')

lcdSIPO lcdSIPO_assign(PXN RS_, PXN E_, PXN signal_, PXN clk_, PXN latch_);

void lcdInit(lcdSIPOPtr lcdptr);

//uint8_t D7_, uint8_t D6_, uint8_t D5_, uint8_t D4_, uint8_t D3_, uint8_t D2_, uint8_t D1_, uint8_t D0_
void lcdSIPO_instruction(lcdSIPOPtr lcdptr, uint8_t inst);

#define funcSet 0b00111000
#define dispOn  0b00001110
#define dispOff 0b00001000
#define entryM  0b00000110
#define clearD  0b00000001
#define retrnH  0b00000010
#define shiftR  0b00011100
#define shiftL  0b00011000

#define cursor_instruction(row, col) 0b10000000 | (row ? 0x40 : 0) | (col >= 0 && col <= 15 ? col : 15)

void lcdSIPO_beginx(lcdSIPOPtr lcdptr);

void lcdSIPO_clearx(lcdSIPOPtr lcdptr);

void lcdSIPO_printx(lcdSIPOPtr lcdptr, char printchar);

void lcdSIPO_print_stringx(lcdSIPOPtr lcdptr, char* printstring, uint8_t stringsize);

void lcdSIPO_clear_linex(lcdSIPOPtr lcdptr, uint8_t line);

void lcdSIPO_returnHomex(lcdSIPOPtr lcdptr);

#define lcdSIPO_begin(lcd) lcdSIPO_beginx(&lcd)

//Enciende el display con el cursor activo
#define lcdSIPO_on(lcd) lcdSIPO_instruction(&lcd, dispOn)

#define lcdSIPO_off(lcd) lcdSIPO_instruction(&lcd, dispOff)

//Esto enciende al display junto con la configuraci�n deseada de cursor y de blink
#define lcdSIPO_cursor_blink(lcd, cursorOnOff, cursorcharacterBlink) lcdSIPO_instruction(&lcd, 0x0C | (cursorOnOff ? 1: 0) << 1 | (cursorcharacterBlink ? 1: 0) << 0)

//#define lcdSIPO_clear(lcd) lcdSIPO_instruction(&lcd, clearD)
#define lcdSIPO_clear(lcd) lcdSIPO_clearx(&lcd)

#define lcdSIPO_cursor(lcd, row, col) lcdSIPO_instruction(&lcd, cursor_instruction(row, col))

#define lcdSIPO_returnHome(lcd) lcdSIPO_returnHomex(&lcd)

#define lcdSIPO_print(lcd, stringprint) lcdSIPO_print_stringx(&lcd, stringprint, sizeof(stringprint))

#define lcdSIPO_printchar(lcd, charprint) lcdSIPO_printx(&lcd, charprint)

#define lcdSIPO_clear_line(lcd, line) lcdSIPO_clear_linex(&lcd, line)

#define lcdSIPO_shift_right(lcd) lcdSIPO_instruction(&lcd, shiftR)

#define lcdSIPO_shift_left(lcd) lcdSIPO_instruction(&lcd, shiftL)

#endif /* LCDSIPO_H_ */