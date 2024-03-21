/*
 * lcdSIPO.c
 */ 
#include <avr/io.h>
#include "../globaltime.h"
#include "lcdSIPO.h"

lcdSIPO lcdSIPO_assign(PXN RS_, PXN E_, PXN signal_, PXN clk_, PXN latch_){
	lcdSIPO temp;
	PXN pins[] = {RS_, E_, signal_, clk_, latch_};
	for(uint8_t i = 0; i < 5; i++){
		if(*(pins[i] + 1) == 'B'){
			temp.ddr[i] = &DDRB;
			temp.port[i] = &PORTB;
		}
		else if(*(pins[i] + 1) == 'C'){
			temp.ddr[i] = &DDRC;
			temp.port[i] = &PORTC;
		}
		else{
			temp.ddr[i] = &DDRD;
			temp.port[i] = &PORTD;
		}
		temp.pin[i] = char2dec(*(pins[i] + 2));
	}
	return temp;
}

void lcdInit(lcdSIPOPtr lcdptr){
	for(uint8_t i = 0; i < 5; i++){
		*lcdptr->ddr[i] |= 1 << lcdptr->pin[i];
	}
}

void E_High(lcdSIPOPtr lcdptr){
	*lcdptr->port[1] |= (1 << lcdptr->pin[1]);
}

void E_Low(lcdSIPOPtr lcdptr){
	*lcdptr->port[1] &= ~(1 << lcdptr->pin[1]);
}

void RS_High(lcdSIPOPtr lcdptr){
	*lcdptr->port[0] |= (1 << lcdptr->pin[0]);
}

void RS_Low(lcdSIPOPtr lcdptr){
	*lcdptr->port[0] &= ~(1 << lcdptr->pin[0]);
}

void enablePin(lcdSIPOPtr lcdptr){
	E_Low(lcdptr);
	E_High(lcdptr);
	E_Low(lcdptr);
}

void SIPO_bitsend(lcdSIPOPtr lcdptr, uint8_t data, uint8_t LSBfirst){
	LSBfirst = LSBfirst ? 1 : 0;
	
	if(LSBfirst){
		for(uint8_t i = 0; i < 8; i++){
			//Mandar el bit adecuado al pin de data
			if(data & (1 << i)){
				*lcdptr->port[2] |= (1 << lcdptr->pin[2]);
			}
			else{
				*lcdptr->port[2] &= ~(1 << lcdptr->pin[2]);
			}
			
			//Mandar la señal del clk
			*lcdptr->port[3] |=  (1 << lcdptr->pin[3]);
			//---------------------------------------
			_delay_us(1);
			*lcdptr->port[3] &= ~(1 << lcdptr->pin[3]);
		}
	}
	else{
		for(int8_t i = 7; i >= 0; i--){
			//Mandar el bit adecuado al pin de data
			if(data & (1 << i)){
				*lcdptr->port[2] |= (1 << lcdptr->pin[2]);
			}
			else{
				*lcdptr->port[2] &= ~(1 << lcdptr->pin[2]);
			}
			
			//Mandar la señal del clk
			*lcdptr->port[3] |=  (1 << lcdptr->pin[3]);
			_delay_us(1);
			*lcdptr->port[3] &= ~(1 << lcdptr->pin[3]);
		}
	}
	
	//Activar el latch (desplegar los valores en el sipo)
	*lcdptr->port[4] |= (1 << lcdptr->pin[4]);
	_delay_us(1);
	*lcdptr->port[4] &= ~(1 << lcdptr->pin[4]);
}

//uint8_t D7_, uint8_t D6_, uint8_t D5_, uint8_t D4_, uint8_t D3_, uint8_t D2_, uint8_t D1_, uint8_t D0_
void lcdSIPO_instruction(lcdSIPOPtr lcdptr, uint8_t inst){
	SIPO_bitsend(lcdptr, inst, 0);
	enablePin(lcdptr);
	//_delay_us(1000);
}

void lcdSIPO_beginx(lcdSIPOPtr lcdptr){
	lcdInit(lcdptr);
	E_Low(lcdptr);
	RS_Low(lcdptr);
	lcdSIPO_instruction(lcdptr, funcSet);
	//_delay_ms(20);
	lcdSIPO_instruction(lcdptr, dispOn);
	//_delay_ms(20);
	lcdSIPO_instruction(lcdptr, entryM);
	//_delay_ms(20);
	//lcdSIPO_instruction(lcdptr, clearD);
	lcdSIPO_clearx(lcdptr);
	lcdSIPO_instruction(lcdptr, cursor_instruction(0, 0));
}

void lcdSIPO_clearx(lcdSIPOPtr lcdptr){
	lcdSIPO_instruction(lcdptr, clearD);
	_delay_ms(4);
}

void lcdSIPO_returnHomex(lcdSIPOPtr lcdptr){
	lcdSIPO_instruction(lcdptr, retrnH); 
	_delay_ms(4);
}

void lcdSIPO_printx(lcdSIPOPtr lcdptr, char printchar){
	RS_High(lcdptr);
	lcdSIPO_instruction(lcdptr, printchar);
	RS_Low(lcdptr);
}

void lcdSIPO_print_stringx(lcdSIPOPtr lcdptr, char* printstring, uint8_t stringsize){
	RS_High(lcdptr);
	for(uint8_t i = 0; i < stringsize - 1; i++){
		lcdSIPO_instruction(lcdptr, printstring[i]);
	}
	RS_Low(lcdptr);
}

void lcdSIPO_clear_linex(lcdSIPOPtr lcdptr, uint8_t line){
	line = line ? 1 : 0;
	lcdSIPO_instruction(lcdptr, cursor_instruction(line, 0));
	lcdSIPO_print_stringx(lcdptr, "                ", 17);
	lcdSIPO_instruction(lcdptr, cursor_instruction(line, 0));
}