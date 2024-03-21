/*
 * lcd.h
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#define RS "PC0"
#define RW "PC1"
#define E  "PC2"
#define D0 "PB0"
#define D1 "PB1"
#define D2 "PD2"
#define D3 "PD3"
#define D4 "PD4"
#define D5 "PD5"
#define D6 "PD6"
#define D7 "PD7"

#define PXN char*

typedef struct{
	uint8_t* ddr[11];
	uint8_t pin[11];
	uint8_t* port[11];
	uint8_t* pinlec[11];
}lcd;

typedef lcd * lcdPtr;

//LCD instructions
#define funcSet 0, 0, 0, 0, 1, 1, 1, 0, 0, 0
#define dispOn  0, 0, 0, 0, 0, 0, 1, 1, 1, 0
#define dispOff 0, 0, 0, 0, 0, 0, 1, 0, 0, 0
#define entryM  0, 0, 0, 0, 0, 0, 0, 1, 1, 0
#define clearD  0, 0, 0, 0, 0, 0, 0, 0, 0, 1
#define retrnH  0, 0, 0, 0, 0, 0, 0, 0, 1, 0
#define shiftR  0, 0, 0, 0, 0, 1, 1, 1, 0, 0
#define shiftL  0, 0, 0, 0, 0, 1, 1, 0, 0, 0


#define cursor_instruction(row, col) 0, 0, 1, 1 & (row), 0, 0, (col) & _BV(3), (col) & _BV(2), (col) & _BV(1), (col) & _BV(0)

#define comp(x, y) (x & _BV(y)) >> y

#define char2inst(dchar) 1, 0, comp(dchar, 7), comp(dchar, 6), comp(dchar, 5), comp(dchar, 4), comp(dchar, 3), comp(dchar, 2), comp(dchar, 1), comp(dchar, 0)

void lcd_beginx(lcdPtr lcdptr);

void lcd_print_stringx(lcdPtr lcdptr, char* pstring, uint8_t pstringSize);

void lcd_clear_linex(lcdPtr lcdptr, uint8_t line);



#define lcd_begin(lcd) lcd_beginx(&lcd)

#define lcd_control(lcd, dispOnOff, cursorOnOff, cursorcharacterBlink) lcd_instruction(&lcd, 0, 0, 0, 0, 0, 0, 1, dispOnOff, cursorOnOff, cursorcharacterBlink)

#define lcd_off(lcd) lcd_instruction(&lcd, dispOff)

#define lcd_clear(lcd) lcd_instruction(&lcd, clearD)

//Row 0-1, col 0-15
#define lcd_cursor(lcd, row, col) lcd_instruction(&lcd, cursor_instruction(row != 0 ? 1 : 0, col))

#define lcd_returnHome(lcd) lcd_instruction(&lcd, retrnH)

#define lcd_print(lcd, dchar) lcd_instruction(&lcd, char2inst(dchar))

#define lcd_print_string(lcd, dstring) lcd_print_stringx(&lcd, dstring, sizeof(dstring))

//Line 0-1
#define lcd_clear_line(lcd, line) lcd_clear_linex(&lcd, line)

#define lcd_shift_right(lcd) lcd_instruction(&lcd, shiftR)

#define lcd_shift_left(lcd) lcd_instruction(&lcd, shiftL)

lcd lcd_assign(PXN RS_, PXN RW_, PXN E_, PXN D0_, PXN D1_, PXN D2_, PXN D3_, PXN D4_, PXN D5_, PXN D6_, PXN D7_);

void lcd_instruction(lcdPtr lcdptr, uint8_t RS_, uint8_t RW_, uint8_t D7_, uint8_t D6_, uint8_t D5_, uint8_t D4_, uint8_t D3_, uint8_t D2_, uint8_t D1_, uint8_t D0_);

#endif /* DISPLAY_H_ */

/*
#define lcd_print_string(lcd, dstring) \
do{ \
	for(uint8_t i = 0; i < sizeof(dstring) - 1; i++){ \
		lcd_print(lcd, dstring[i]); \
	} \
}while(0)

//Line 0-1
#define lcd_clear_line(lcd, line) \
do{ \
	lcd_cursor(lcd, line, 1); \
	for(uint8_t i = 0; i < 16; i++){ \
		lcd_print(lcd, 16); \
	} \
	lcd_cursor(lcd, line, 1); \
}while(0)
*/