/*************************************************************************
 *   Microcontroladores   ***************                                *
 *   PIA*******                                                          *
 *   Dispositivo: ATmega328p                                             *
 *   Lenguaje C                                                          *
 *   Rev: 1.0                                                            *
 *                                                     Fecha: 26/10/22   *
 ************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "globaltime.h"
#include "lcdSIPO/lcdSIPO.h"
#include "keypad/keypad.h"
#include "GPIO/GPIO.h"

volatile uint8_t firstclick, pirsensor;

void init_interrupts(void);
/*
int main(void){
	lcdSIPO lcdsipo1 = lcdSIPO_assign(RSpin, Epin, signalpin, clkpin, latchpin);
	lcdSIPO_begin(lcdsipo1);
	lcdSIPO_cursor(lcdsipo1, 0, 0);
	lcdSIPO_print(lcdsipo1, "Presione # para");
	lcdSIPO_cursor(lcdsipo1, 1, 2);
	lcdSIPO_print(lcdsipo1, "contrasena");
	while(1){
	}
}
*/


int main(void){
	//Inicializaci�n
	firstclick = 1;
	cli();
	lcdSIPO lcdsipo1 = lcdSIPO_assign(RSpin, Epin, signalpin, clkpin, latchpin);
	keypad keypad1 = keypad_assign(R0,R1,R2,R3, C0,C1,C2,C3);
	SIPOshiftregister sipo1 = SIPO_assign(SIPOsignal, SIPOclk, SIPOlatch);
	init_input();

	lcdSIPO_begin(lcdsipo1);
	keypad_init(keypad1);
	SIPO_init(sipo1);
	
	//Desplegar informaci�n b�sica
	lcdSIPO_cursor(lcdsipo1, 0, 0);
	lcdSIPO_print(lcdsipo1, "Presione # para");
	lcdSIPO_cursor(lcdsipo1, 1, 2);
	lcdSIPO_print(lcdsipo1, "contrasena");
	SIPO_bitsend(sipo1, 0b1000000, 1);
	
	sei();
	
	init_interrupts();
	
	uint8_t digitCounter = 0;
	char password[4], passwordTry[4];
	
	//Esperar primera interacci�n
	while(firstclick){
	}
	cli();
	keypad_start(keypad1);
	lcdSIPO_clear(lcdsipo1);
	_delay_ms(1000);
	
	//Ingreso de la contrase�a
	lcdSIPO_print(lcdsipo1, "Inserte 4 chars:");
	lcdSIPO_cursor(lcdsipo1, 1,0);
	SIPO_bitsend(sipo1, 0b10000, 1);
	
	while(digitCounter < 4){
		password[digitCounter++] = keypad_readUntilInput(keypad1);
		lcdSIPO_printchar(lcdsipo1, password[digitCounter - 1]);
		_delay_ms(500);
		
		if(digitCounter == 4){
			lcdSIPO_clear_line(lcdsipo1, 0);
			lcdSIPO_print(lcdsipo1, "Esta seguro?");
			lcdSIPO_clear_line(lcdsipo1, 1);
			lcdSIPO_print(lcdsipo1, "Press # to redo");
			
			if(keypad_readUntilInput(keypad1) == '#'){
				_delay_ms(500);
				digitCounter = 0;
				lcdSIPO_clear(lcdsipo1);
				lcdSIPO_print(lcdsipo1, "Inserte 4 chars:");
				lcdSIPO_cursor(lcdsipo1, 1, 0);
			}
		}
			
	}
	
	
	
	while(1){
		
		//Esperando siguiente interacci�n para activar alarma
		_delay_ms(500);
		digitCounter = 0;
		SIPO_bitsend(sipo1, 0b100000, 1);
		lcdSIPO_clear(lcdsipo1);
		lcdSIPO_print(lcdsipo1, "Presione algo");
		lcdSIPO_cursor(lcdsipo1, 1, 0);
		lcdSIPO_print(lcdsipo1, "para activar");
		
		
		//Cambiar contrase�a de nuevo
		if(keypad_readUntilInput(keypad1) == '#'){		
			_delay_ms(500);
			SIPO_bitsend(sipo1, 0b10000, 1);
			lcdSIPO_clear(lcdsipo1);
			lcdSIPO_print(lcdsipo1, "Desea cambiar pw?");
			lcdSIPO_cursor(lcdsipo1, 1, 0);
			lcdSIPO_print(lcdsipo1, "Presione #");
			
			//Confirmar que se desea activar
			if(keypad_readUntilInput(keypad1) == '#'){
				_delay_ms(500);
				
				lcdSIPO_clear(lcdsipo1);
				lcdSIPO_print(lcdsipo1, "Ingrese pre pw:");
				lcdSIPO_cursor(lcdsipo1, 1, 0);
				
				//Verificar primero la contrase�a anterior
				while(digitCounter < 4){
					passwordTry[digitCounter++] = keypad_readUntilInput(keypad1);
					lcdSIPO_printchar(lcdsipo1, passwordTry[digitCounter - 1]);
					_delay_ms(500);
					
					//Repetir hasta que se ponga la correcta
					if(digitCounter == 4){
						if(!compare_pwds(passwordTry, password)){
							lcdSIPO_clear(lcdsipo1);
							lcdSIPO_print(lcdsipo1, "PSW incorrecta");
							lcdSIPO_cursor(lcdsipo1, 1, 0);
							lcdSIPO_print(lcdsipo1, "Try agn: ");
							digitCounter = 0;
						}
						
					}
				}
				//Selecci�n de nueva contrase�a
				lcdSIPO_clear(lcdsipo1);
				lcdSIPO_print(lcdsipo1, "Ingrese nueva");
				lcdSIPO_cursor(lcdsipo1, 1, 0);
				lcdSIPO_print(lcdsipo1, "PW: ");
				digitCounter = 0;
				
				//Ciclo de obtenci�n de la nueva contrase�a
				while(digitCounter < 4){
					password[digitCounter++] = keypad_readUntilInput(keypad1);
					lcdSIPO_printchar(lcdsipo1, password[digitCounter - 1]);
					_delay_ms(500);
					
					//Confirmar contrase�a
					if(digitCounter == 4){
						lcdSIPO_clear_line(lcdsipo1, 0);
						lcdSIPO_print(lcdsipo1, "Esta seguro?");
						lcdSIPO_clear_line(lcdsipo1, 1);
						lcdSIPO_print(lcdsipo1, "Press # to redo");
						
						//Regresar para volver a cambiar
						if(keypad_readUntilInput(keypad1) == '#'){
							_delay_ms(500);
							digitCounter = 0;
							lcdSIPO_clear(lcdsipo1);
							lcdSIPO_print(lcdsipo1, "Inserte 4 chars:");
							lcdSIPO_cursor(lcdsipo1, 1, 0);
						}
					}
				}
			}
			lcdSIPO_clear(lcdsipo1);
			lcdSIPO_print(lcdsipo1, "Se activar� la");
			lcdSIPO_cursor(lcdsipo1, 1, 0);
			lcdSIPO_print(lcdsipo1, "alarma en 3s!!!");
			_delay_ms(3000);
			
		}
		
		SIPO_bitsend(sipo1, 0b1100000, 1);
		lcdSIPO_clear(lcdsipo1);
		lcdSIPO_print(lcdsipo1, "!Alarma Encendida!");
		lcdSIPO_cursor(lcdsipo1, 1, 18);
		lcdSIPO_print(lcdsipo1, "!Alarma Encendida!");
		keypad_idle(keypad1);
		
		sei();
		firstclick = 1;
		pirsensor = 1;
		while(!read_input() && firstclick && pirsensor){
			lcdSIPO_shift_right(lcdsipo1);
			_delay_ms(200);
		}
		cli();
		_delay_ms(300);
		lcdSIPO_clear(lcdsipo1);
		lcdSIPO_print(lcdsipo1, "PSW para deact:");
		lcdSIPO_cursor(lcdsipo1, 1, 0);
		SIPO_bitsend(sipo1, 0b1111011, 1);
	
		digitCounter = 0;
		while(digitCounter < 4){
			passwordTry[digitCounter++] = keypad_readUntilInput(keypad1);
			lcdSIPO_printchar(lcdsipo1, passwordTry[digitCounter - 1]);
			_delay_ms(500);
			
			if(digitCounter == 4){
				if(!compare_pwds(passwordTry, password)){
					lcdSIPO_clear(lcdsipo1);
					lcdSIPO_print(lcdsipo1, "PSW incorrecta");
					lcdSIPO_cursor(lcdsipo1, 1, 0);
					lcdSIPO_print(lcdsipo1, "Try agn: ");
					digitCounter = 0;
				}
				else{
					lcdSIPO_clear(lcdsipo1);
					lcdSIPO_print(lcdsipo1, "PSW correcta!");
				}
			}
			
		}
	}
}

void init_interrupts(void){
	PCICR  |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT22);
	
	PCICR  |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT13);
}

ISR(PCINT2_vect){
	firstclick = 0;
}

ISR(PCINT1_vect){
	pirsensor = 0;
}
