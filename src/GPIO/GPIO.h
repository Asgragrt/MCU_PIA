/*
 * GPIO.h
 *
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#define SIPOsignal "PD4"
#define SIPOclk    "PD2"
#define SIPOlatch  "PD3"

#define pir "PC5"
#define  getDDR(x) *(x[1] == 'B' ? &DDRB : (x[1] == 'C' ? &DDRC : &DDRD))
#define getPORT(x) *(x[1] == 'B' ? &PORTB : (x[1] == 'C' ? &PORTC : &PORTD))
#define  getPIN(x) *(x[1] == 'B' ? &PINB : (x[1] == 'C' ? &PINC : &PIND))
#define getPin_(x) char2dec(x[2])

typedef struct{
	uint8_t* ddr[3];
	uint8_t  pin[3];
	uint8_t* port[3];
}SIPOshiftregister;

typedef SIPOshiftregister * SIPOshiftregisterPtr;

#define PXN char*

#define char2dec(a) (int) (a - '0')

SIPOshiftregister SIPO_assign(PXN signalpin_, PXN clkpin_, PXN latchpin_);

void SIPO_initx(SIPOshiftregisterPtr SIPOptr);

void SIPO_bitsendx(SIPOshiftregisterPtr SIPOptr, uint8_t data, uint8_t LSBfirst);

#define SIPO_init(sipo) SIPO_initx(&sipo)

#define SIPO_bitsend(sipo, data, LSBfirst) SIPO_bitsendx(&sipo, data, LSBfirst)

void init_input(void);

uint8_t read_input(void);

#endif /* GPIO_H_ */