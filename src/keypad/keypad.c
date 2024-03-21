/*
 * keypad.c
 */ 

#include <avr/io.h>
#include "../globaltime.h"
#include "keypad.h"

keypad keypad_assign(PXN row0_, PXN row1_, PXN row2_, PXN row3_, PXN col0_, PXN col1_, PXN col2_, PXN col3_){
	keypad temp;
	
	PXN pins[] = {col0_, col1_, col2_, col3_, row0_, row1_, row2_, row3_};
	for(uint8_t i = 0; i < 8; i++){
		if(*(pins[i] + 1) == 'B'){
			temp.ddr[i] = &DDRB;
			temp.port[i] = &PORTB;
			if(i < 4) temp.pinr[i] = &PINB;
		}
		else if(*(pins[i] + 1) == 'C'){
			temp.ddr[i] = &DDRC;
			temp.port[i] = &PORTC;
			if(i < 4) temp.pinr[i] = &PINC;
		}
		else{
			temp.ddr[i] = &DDRD;
			temp.port[i] = &PORTD;
			if(i < 4) temp.pinr[i] = &PIND;
		}
		temp.pin[i] = char2dec(*(pins[i] + 2));
	}
	
	return temp;
}

void keypad_initx(keypadPtr keypadptr_){
	uint8_t i;
	//Cols como inputs y Rows con pullups para evitar cortos
	for(i = 0; i < 7; i++){
		*keypadptr_->ddr[i]  &= ~(1 << keypadptr_->pin[i]);
		//Resistencias pull up
		*keypadptr_->port[i] |=  (1 << keypadptr_->pin[i]);
	}
	*keypadptr_->ddr[7]  |=  (1 << keypadptr_->pin[7]);
	*keypadptr_->port[7] &= ~(1 << keypadptr_->pin[7]);
}

void keypad_idlex(keypadPtr keypadptr_){
	*keypadptr_->ddr[7]  |=  (1 << keypadptr_->pin[7]);
	*keypadptr_->port[7] &= ~(1 << keypadptr_->pin[7]);
}

void keypad_startx(keypadPtr keypadptr_){
	*keypadptr_->ddr[7]  &= ~(1 << keypadptr_->pin[7]);
	*keypadptr_->port[7] |=  (1 << keypadptr_->pin[7]);
}

char keys[4][4] = {	{'1','2','3','A'},
					{'4','5','6','B'},
					{'7','8','9','C'},
					{'*','0','#','D'}};

char keypad_scanKeyx(keypadPtr keypadptr_){
	char tempchar = '!';
	
	for(uint8_t rows = 4; rows < 8; rows++){
		//rows como output low
		*keypadptr_->ddr[rows]  |=  (1 << keypadptr_->pin[rows]);
		*keypadptr_->port[rows] &= ~(1 << keypadptr_->pin[rows]);
	
		for(uint8_t cols = 0; cols < 4; cols++){
			if(! (*keypadptr_->pinr[cols] & (1 << keypadptr_->pin[cols]) ) ){
				tempchar = keys[rows - 4][cols];
				break;
			}
		}
		//row de regreso como pullup
		*keypadptr_->ddr[rows]  &= ~(1 << keypadptr_->pin[rows]);
		*keypadptr_->port[rows] |=  (1 << keypadptr_->pin[rows]);
		if(tempchar != '!') break;
	}
	return tempchar;
}

char keypad_readUntilInputx(keypadPtr keypadptr_){
	char tempchar = '!';
	do{
		tempchar = keypad_scanKeyx(keypadptr_);
	}while(tempchar == '!');
	
	return tempchar;
}

uint8_t compare_pwds(char* psw1, char* psw2){
	for(uint8_t i = 0; i < 4; i++){
		if(psw1[i] != psw2[i]){
			return 0;
		}
	}
	return 1;
}

/*
char* keys[4][4] = {{"1","2","3","A"},
					{"4","5","6","B"},
					{"7","8","9","C"},
					{"*","0","#","D"}};

char* keypad_scanKey(keypadPtr keypadptr_){
	char* tempchar = "!";
	
	for(uint8_t rows = 4; rows < 8; rows++){
		//rows como low	
		*keypadptr_->port[rows] &= ~(1 << keypadptr_->pin[rows]);
		
		_delay_ms(1000);
		for(uint8_t cols = 0; cols < 4; cols++){
			if(! (*keypadptr_->pinr[cols] & (1 << keypadptr_->pin[cols]) ) ){
				tempchar = keys[rows - 4][cols];
				break;
			}
		}
		//row como high
		*keypadptr_->port[rows] |= (1 << keypadptr_->pin[rows]);
		if(tempchar != '!') break;
	}
	return tempchar;
}
*/