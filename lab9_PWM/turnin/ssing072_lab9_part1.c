/*	Author: ngupt009
 *  Partner(s) Name: surya
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
    static double current_frequency;
    if(frequency != current_frequency){
        if(!frequency){ TCCR3B &= 0x08; }
        else{ TCCR3B |= 0x03; }
        if(frequency < .954){ OCR3A = 0xFFFF;}
        else if(frequency > 31250) { OCR3A = 0x0000; }
        else { OCR3A = (short)(8000000 / (128* frequency)) -1; }
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on(){
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off(){
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

#define button (~PINA & 0x07)
enum STATES { INIT, SOUND1, SOUND2, SOUND3 } state;

void sound() {
	switch(state) {
		case INIT:
		switch(button) {
			case 0x01:
			state = SOUND1;
			break;
			case 0x02:
			state = SOUND2;
			break;
			case 0x04:
			state = SOUND3;
			break;
			default:
			state = INIT;
			break;
		}
		break;
		case SOUND1:
		if (button == 0x01) {
			state = SOUND1;
			} else {
			state = INIT;
		}
		break;
		case SOUND2:
		if (button == 0x02) {
			state = SOUND2;
			} else {
			state = INIT;
		}
		break;
		case SOUND3:
		if (button == 0x04) {
			state = SOUND3;
			} else {
			state = INIT;
		}
		break;
	}
	switch(state) {
		case INIT:
		set_PWM(0);
		break;
		case SOUND1:
		set_PWM(261.63);
		break;
		case SOUND2:
		set_PWM(293.66);
		break;
		case SOUND3:
		set_PWM(329.63);
		break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	state = INIT;
	while(1) {
		sound();
	}
	return 1;
}
