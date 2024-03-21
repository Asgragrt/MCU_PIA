/*
 * GPIO.c
 */ 

#include <avr/io.h>
#include "../globaltime.h"
#include "GPIO.h"

SIPOshiftregister SIPO_assign(PXN signalpin_, PXN clkpin_, PXN latchpin_){
	SIPOshiftregister temp;
	
	PXN pins[] = {signalpin_, clkpin_, latchpin_};
	for(uint8_t i = 0; i < 3; i++){
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

void SIPO_initx(SIPOshiftregisterPtr SIPOptr){
	for(uint8_t i = 0; i < 3; i++){
		*SIPOptr->ddr[i] |= (1 << SIPOptr->pin[i]);
	}
}


void SIPO_bitsendx(SIPOshiftregisterPtr SIPOptr, uint8_t data, uint8_t LSBfirst){
	LSBfirst = LSBfirst ? 1 : 0;
	
	if(LSBfirst){
		for(uint8_t i = 0; i < 8; i++){
			//Mandar el bit adecuado al pin de data
			if(data & (1 << i)){
				*SIPOptr->port[0] |= (1 << SIPOptr->pin[0]);
			}
			else{
				*SIPOptr->port[0] &= ~(1 << SIPOptr->pin[0]);
			}
			
			//Mandar la señal del clk
			*SIPOptr->port[1] |=  (1 << SIPOptr->pin[1]);
			_delay_us(1);
			*SIPOptr->port[1] &= ~(1 << SIPOptr->pin[1]);
		}
	}
	else{
		for(int8_t i = 7; i >= 0; i--){
			//Mandar el bit adecuado al pin de data
			if(data & (1 << i)){
				*SIPOptr->port[0] |= (1 << SIPOptr->pin[0]);
			}
			else{
				*SIPOptr->port[0] &= ~(1 << SIPOptr->pin[0]);
			}
			
			//Mandar la señal del clk
			*SIPOptr->port[1] |=  (1 << SIPOptr->pin[1]);
			_delay_us(1);
			*SIPOptr->port[1] &= ~(1 << SIPOptr->pin[1]);
		}
	}
	
	//Activar el latch (desplegar los valores en el sipo)
	*SIPOptr->port[2] |= (1 << SIPOptr->pin[2]);
	_delay_us(1);
	*SIPOptr->port[2] &= ~(1 << SIPOptr->pin[2]);
}

void init_input(void){
	getDDR(pir)  &= ~(1 << getPin_(pir));
	getPORT(pir) &= ~(1 << getPin_(pir));
}

uint8_t read_input(void){
	return (getPIN(pir) & (1 << getPin_(pir))) >> getPin_(pir);
}