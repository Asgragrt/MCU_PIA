/*
 * lcd.c
 */ 
#include <avr/io.h>
#include "lcd.h"
#define F_CPU 1000000UL
#include <util/delay.h>

#define char2dec(a) (int) (a - '0')

lcd lcd_assign(PXN RS_, PXN RW_, PXN E_, PXN D0_, PXN D1_, PXN D2_, PXN D3_, PXN D4_, PXN D5_, PXN D6_, PXN D7_){
	lcd temp;
	PXN pins[] = {RS_, RW_, D7_, D6_, D5_, D4_, D3_, D2_, D1_, D0_, E_};
	for(uint8_t i = 0; i < 11; i++){
		if(*(pins[i] + 1) == 'B'){
			temp.ddr[i] = &DDRB;
			temp.port[i] = &PORTB;
			temp.pinlec[i] = &PINB;
		}
		else if(*(pins[i] + 1) == 'C'){
			temp.ddr[i] = &DDRC;
			temp.port[i] = &PORTC;
			temp.pinlec[i] = &PINC;
		}
		else{
			temp.ddr[i] = &DDRD;
			temp.port[i] = &PORTD;
			temp.pinlec[i] = &PIND;
		}
		temp.pin[i] = char2dec(*(pins[i] + 2));
	}
	
	return temp;
}

//D0 - D7
void lcd_io(lcdPtr lcdptr, uint8_t iovar){
	for(uint8_t i = 2; i < 10; i++){
		if(iovar){
			*lcdptr->ddr[i] |= (1 << lcdptr->pin[i]);
			*lcdptr->port[i] &= ~(1 << lcdptr->pin[i]);
			} else{
			*lcdptr->port[i] &= ~(1 << lcdptr->pin[i]);
			*lcdptr->ddr[i] &= ~(1 << lcdptr->pin[i]);
		}
	}
}

#define RS_High *lcdptr->port[0] |= (1 << lcdptr->pin[0])
#define RW_High *lcdptr->port[1] |= (1 << lcdptr->pin[1])
#define E_High *lcdptr->port[10] |= (1 << lcdptr->pin[10])

#define RS_Low *lcdptr->port[0] &= ~(1 << lcdptr->pin[0])
#define RW_Low *lcdptr->port[1] &= ~(1 << lcdptr->pin[1])
#define E_Low *lcdptr->port[10] &= ~(1 << lcdptr->pin[10])

#define B7_In  *lcdptr->ddr[2] &= ~(1 << lcdptr->pin[2])
#define B7_Out *lcdptr->ddr[2] |=  (1 << lcdptr->pin[2])
#define B7_Read bit_is_set(*lcdptr->pinlec[2], lcdptr->pin[2]) >> lcdptr->pin[2]

#define enablePin \
do{ \
	E_Low; \
	E_High; \
	E_Low; \
}while(0)

#define check_BF(lcdptr) \
do{ \
	RS_Low; \
	RW_High; \
	lcd_io(lcdptr, 0); \
	E_High; \
	while(B7_Read){ \
	} \
	E_Low; \
	RW_Low; \
	lcd_io(lcdptr, 1); \
	RS_Low; \
} while(0)

void lcd_beginx(lcdPtr lcdptr){
	*lcdptr->ddr[0]  |= (1 << lcdptr->pin[0]); 
	*lcdptr->ddr[1]  |= (1 << lcdptr->pin[1]); 
	*lcdptr->ddr[10] |= (1 << lcdptr->pin[10]); 
	lcd_io(lcdptr, 1); 
	lcd_instruction(lcdptr, funcSet); 
	lcd_instruction(lcdptr, dispOn); 
	lcd_instruction(lcdptr, entryM); 
	lcd_instruction(lcdptr, clearD);
	lcd_instruction(lcdptr, cursor_instruction(0, 0));
}

void lcd_instruction(lcdPtr lcdptr, uint8_t RS_, uint8_t RW_, uint8_t D7_, uint8_t D6_, uint8_t D5_, uint8_t D4_, uint8_t D3_, uint8_t D2_, uint8_t D1_, uint8_t D0_){
	check_BF(lcdptr);	
	uint8_t inst[10] = {RS_, RW_, D7_, D6_, D5_, D4_, D3_, D2_, D1_, D0_};
	for(uint8_t i = 0; i < 10; i++){
		if(inst[i]){
			*lcdptr->port[i] |= (1 << lcdptr->pin[i]);
		}
		else{
			*lcdptr->port[i] &= ~(1 << lcdptr->pin[i]);
		}
	}
	enablePin;
	//Quitar como comentario para la simulación
	//_delay_us(800);
}

void lcd_print_stringx(lcdPtr lcdptr, char* pstring, uint8_t pstringSize){
	for(uint8_t i = 0; i < pstringSize - 1; i++){ 
		lcd_print(*lcdptr, pstring[i]); 
	}
}

void lcd_clear_linex(lcdPtr lcdptr, uint8_t line){
	line = line != 0 ? 1 : 0;
	lcd_cursor(*lcdptr, line, 0); 
	for(uint8_t i = 0; i < 16; i++){ 
		lcd_print(*lcdptr, 16); 
	} 
	lcd_cursor(*lcdptr, line, 0); 
}