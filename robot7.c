/*
Mini SumoBot 7 
left edge of object detection 
timer to switch between attack and search modes

by John Jennings johnrjennings at me dot com


reflectance sensors connected to PD2
led connected to onboard led PD1
start button connected to PD4
distance sensor connected to PC0

PARTS LIST
controller Baby Orangutan B328 Pololu #1220
adjustable Boost Regulator 2.5-9.5V Pololu #791
22T Track Set Pololu #1415
	-pair of 22-tooth silicone tracks.
	-two drive sprockets measuring 1.38" (35 mm) in diameter.
	-two matching idler sprockets along with mounting hardware.
two 150:1 Micro Metal Gearmotors MP Pololu #2368
Sharp GP2Y0A21YK0F Analog Distance Sensor 10-80cm Pololu #136
3-Pin Female JST PH-Style Cable (30cm) for Sharp Distance Sensors Pololu #117
two QTR-1A Reflectance Sensors Pololu # 2458  
dip switch (double) for turning off motors Philmore # 30-1002
electrolytic capacitor (on power rail near sensor leads) 330uF 10v
miniature slide switch (power) Philmore no.30-9185
*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "USART.h"

// -------- Inits --------- //
int mode = 0;
int t = 0;

// Button function
//****************************************************************************************
//****************************************************************************************
static inline void buttonPress(void) {
	DDRD &= ~(1 << PD4);				// setup button on PD4 for input 
	PORTD |= (1 << PD4);				// initialize pullup resistor on PD4
	DDRD |= (1 << PD1);				// set up LED  on PD1 for output  

	uint8_t i;
                       
	// ------ Event loop ------ // 
	loop_until_bit_is_clear(PIND, PD4);
	_delay_ms(100);
	loop_until_bit_is_clear(PIND, PD4);		// check to insure bit is not clear.
	for (i = 0; i < 5;i ++){			// blink led on PD1
		PORTD |= (1 << PD1); 
		_delay_ms(900);
		PORTD &= ~(1 << PD1);
		_delay_ms(100);
    }   
}  

// ADC function
//****************************************************************************************
//****************************************************************************************
static inline void initADC0(void) {
  ADMUX |= (1 << REFS0);				// reference voltage on AVCC 
  ADCSRA |= (1 << ADPS2);				// ADC clock prescaler /16 
  ADCSRA |= (1 << ADEN);				// enable ADC
}                  

// Motor Control Functions
//****************************************************************************************
//****************************************************************************************
void forwardRight(unsigned char pwm) {			// -- pwm is an 8-bit value
	OCR0A = 0;					// (i.e. ranges from 0 to 255)
	OCR0B = pwm * 3;
	OCR2A = 0;
	OCR2B = pwm;
}

void forwardLeft(unsigned char pwm) {
	OCR0A = 0;
	OCR0B = pwm;
	OCR2A = 0;
	OCR2B = pwm * 3;
}
void forward(unsigned char pwm) {
	OCR0A = 0;
	OCR0B = pwm;
	OCR2A = 0;
	OCR2B = pwm;
}
void reverse(unsigned char pwm) {
	OCR0A = pwm;
	OCR0B = 0;
	OCR2A = pwm;
	OCR2B = 0;	
}
void rotateRight(unsigned char pwm) {
	OCR0A = 0;
	OCR0B = pwm;
	OCR2A = pwm;
	OCR2B = 0;
}
void rotateLeft(unsigned char pwm) {
	OCR0A = pwm;
	OCR0B = 0;
	OCR2A = 0;
	OCR2B = pwm;
}
void stop(unsigned char pwm) {
	OCR0A = pwm;
	OCR0B = 0;
	OCR2A = 0;
	OCR2B = pwm;
}

// Motor Initialization routine 
//****************************************************************************************
//****************************************************************************************
//  this function must be called before you use any of the above motor functions
void motors_init(void) {
    TCCR0A = TCCR2A = 0xF3;						
    TCCR0B = TCCR2B = 0x02;    					
    OCR0A = OCR0B = OCR2A = OCR2B = 0;			
    DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);    
	DDRB |= (1 << PORTB3);						
}

// initialize timer1, interrupt 
//****************************************************************************************
//****************************************************************************************
void timer1_init(void) {
	TCCR1B |= (1 << WGM12);				// configure timer1 for CTC mode
	TIMSK1 |= (1 << OCIE1A);			// enable the CTC interrupt
	sei();						// enable global interrupts
	OCR1A   = 19531;				// set the CTC compare value
	TCCR1B |= ((1 << CS10) | (1 << CS12));		// start the timer at 20MHz/1024
}	

// TIMER1 service routine
//****************************************************************************************
//****************************************************************************************
// Called every time the timer reaches the threshold it toggles the Mode and LED
ISR(TIMER1_COMPA_vect) {
	t++;
	if (t == 5){
		PORTD ^= (1 << PD1);
		mode = (-1 * mode)+1;
   		t = 0;
	}
}

// Attack Routine
//****************************************************************************************
//****************************************************************************************
void attack(void) {
	    
	if (bit_is_clear(PIND, PD2)) {
  		turn();
  	}      
	else {                                              
		forward(255);    	
    }
}

// Turn Routine
//****************************************************************************************
//****************************************************************************************
void turn(void) {		
  	reverse(255);
  	_delay_ms(100);
	rotateRight(255);
	_delay_ms(900); 	
    }
 
// Main routine 
//****************************************************************************************
//****************************************************************************************
int main(void) {

// -------- Inits --------- //	
	DDRD &= ~(1<<PD2);				// Port D Pin 2 as input
	PORTD |= (1 << PD2);				// initialize pullup resistor on PD2
	motors_init();
  	uint16_t adcValue;
  	initADC0();
  	
 	buttonPress();
//	borderInterrupt(); 	
  	timer1_init();					// initialize timer1


// ------ Event loop ------ //
	while (1) {   
		ADCSRA |= (1 << ADSC);        		// start ADC conversion
		loop_until_bit_is_clear(ADCSRA, ADSC); 	// wait until done
		adcValue = ADC;                       	// read ADC in
   		adcValue=(6762/(adcValue-9))-4;      	// convert voltage to distance   
    	 
		if (mode == 0){
			attack();
		}
		
		if ((mode == 1) && (adcValue > 45)){
			rotateRight(255);
		}
		
		if ((mode == 1) && (adcValue > 0) && (adcValue <= 45)){
			attack();
		} 
	} 	
                                                  
// ------ End event loop ------//
	return (0);
}
