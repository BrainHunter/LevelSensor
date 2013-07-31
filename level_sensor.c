/*
Filename:       	level_sensor.c
Project:        	Level-Sensor
Copyright:      	Nicolas Kaufmann  mailto: Mr.Spock.NK@gmx.de
Author:         	Nicolas Kaufmann
Creation:       	16.09.2011 
Version:        	22.09.2011
Description:    	Firmware für den Level-Sensor. 

Dieses Programm ist freie Software. Sie können es unter den Bedingungen der 
GNU General Public License, wie von der Free Software Foundation veröffentlicht, 
weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder 
(nach Ihrer Option) jeder späteren Version. 

Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, 
daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
Programm  erhalten haben. Der Text der GNU General Public License ist auch 
im Internet unter http://www.gnu.org/licenses/gpl.txt veröffentlicht.
Eine inoffizielle deutsche Übersetzung findet sich unter 
http://www.gnu.de/gpl-ger.html. Diese Übersetzung soll nur zu einem besseren 
Verständnis der GPL verhelfen; rechtsverbindlich ist alleine die englischsprachige
Version.
*/

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include <stdio.h>	

#include "level_sensor.h"

//MACROS
//TIMER
#define	START_TIMER		TCCR1B |= (1 << CS11)
#define STOP_TIMER		TCCR1B &=~ (1 << CS11)

// LOAD PIN: PD5
#define SET_LOAD_DDR	DDRD |= (1 << PD5)
#define	LOAD_ON			PORTD |= (1 << PD5)
#define LOAD_OFF 		PORTD &=~ (1 << PD5)

// DIVIDER SOURCE PIN: PD4
#define SET_DIVIDER_SOURCE_DDR	DDRD |= (1 << PD4)
#define DIVIDER_SOURCE_ON		PORTD |= (1 << PD4)
#define DIVIDER_SOURCE_OFF		PORTD &=~ (1 << PD4)

//OUT1 PIN:	PC2
#define SET_OUT1_DDR	DDRC |= (1 << PC2)
#define	OUT1_ON			PORTC |= (1 << PC2)
#define OUT1_OFF 		PORTC &=~ (1 << PC2)
#define OUT1_READ		((PORTC & (1 << PC2)) >> PC2)

//OUT2 PIN:	PC3
#define SET_OUT2_DDR	DDRC |= (1 << PC3)
#define	OUT2_ON			PORTC |= (1 << PC3)
#define OUT2_OFF 		PORTC &=~ (1 << PC3)
#define OUT2_READ		((PORTC & (1 << PC3)) >> PC3)

//IN1 PIN: PC0
#define SET_IN1_DDR		DDRC &=~ (1 << PC0)
#define IN1_PULLUP_ON	PORTC |= (1 << PC0)
#define IN1_PULLUP_OFF	PORTC &=~ (1 << PC0)
#define	IN1_READ		((PINC & (1 << PC0)) >> PC0)

//IN2 PIN: PC1
#define SET_IN2_DDR		DDRC &=~ (1 << PC1)
#define IN2_PULLUP_ON	PORTC |= (1 << PC1)
#define IN2_PULLUP_OFF	PORTC &=~ (1 << PC1)
#define	IN2_READ		((PINC & (1 << PC1)) >> PC1)

//global flag variable
volatile uint8_t 	flag;
// global error flag
volatile uint8_t 	error;
//global result variable
volatile uint16_t 	result;

// function declaration
void start_measure(void);
void stop_measure(void);

void calibrate_min(ls_config *config);
void calibrate_max(ls_config *config);

void init_level_sensor(){
	//levelsensor:
	/*
	PD6 AIN0:	refernz
	PD7 AIN1: 	Sense
	PD5:	Source
	*/
		
	//init variables:
	flag=0;
	result=0;
	

	//timer:
	TCCR1B |= (1 << ICNC1);		// This enables the noise canceler
	TCCR1B &=~ (1 << ICES1);	// This selects the falling edge as input catpure trigger
	
	/*
	TCCR1B |= (1 << CS10);		//Prescaler 1
	TCCR1B &=~ (1 << CS10); 	// countr stopped
	
	TCNT1 = 0;
	*/

	//ICR1 is the captured value
	
	TIMSK |= (1 << TICIE1);	//Enable the input capture interrupt.
	TIMSK |= (1 << TOIE1);
	//printf("Timer1 Init done\r\n");
	
	
	//comperator:
	SFIOR &=~ (1<<ACME);	// This sets negative input to AIN1:
	ACSR |= (1 << ACIC);	//  this bit enables the Input Capture function in Timer/Counter1 to be triggered by the Analog Comparator
	// Comperator = ACSR & (1 << ACO);
	//printf("comperator Init done\r\n");
	
	// ports:
	// set PD5 as output:
	SET_LOAD_DDR;
	SET_DIVIDER_SOURCE_DDR;
	SET_OUT1_DDR;
	SET_OUT2_DDR;
	
	LOAD_OFF;
	DIVIDER_SOURCE_OFF;
	OUT1_OFF;
	OUT2_OFF;
	
	SET_IN1_DDR;
	SET_IN2_DDR;
	IN1_PULLUP_ON;
	IN2_PULLUP_ON;
	
	// set PD6 & PD7 as input + disable pullup:
	DDRD &=~ ((1 << PD6) | (1 << PD7));
	PORTD &=~ ((1 << PD6) | (1 << PD7));
	
 	//printf("ports Init done\r\n");


}

uint16_t get_last_level(){
	return result;
}

/*
int32_t measure_level(){

	enum stat {IDLE, STARTED};
	static enum stat status=IDLE;
	
	switch(status){
		case IDLE:		//printf("setting timer and PD5 to high\r\n");
						start_measure();
						status = STARTED;
						break;
		
		case STARTED:	if(flag){ // we have a finished measurement
							//printf("setting PD5 to low & wait\r\n");
							stop_measure();
							
							flag = 0;
							status = IDLE;
							return result;
						}
						if(error){
							stop_measure();
							error = 0;
							flag = 0;
							status = IDLE;
							return -1;
						}
						break;
	}
	
	return 0;
}
*/

uint16_t measure_level(){

	start_measure();
	
	while(1){
		if(flag){
			stop_measure();
			flag = 0;
			return result;
		} 
		if(error){
			stop_measure();
			error = 0;
			flag = 0;
			return 0;
		}
	}
	
}

void start_measure(){
	TCNT1 = 0;				//Set Timer to 0
	START_TIMER; 			//Start Timer
	LOAD_ON;				//set load pin
	return;
}

void stop_measure(){
	STOP_TIMER;				// stop timer
	LOAD_OFF;				// release load pin
}

ISR(TIMER1_CAPT_vect){		// The Comperator triggered the Input Capture event
	flag = 1;				// set the event.
	result = ICR1;			// save the result
	return;
}


ISR(TIMER1_OVF_vect){		// got overflow on timer1 
	error=1;				// we will handle this as an error (this could change some day!)
}

void process_ioAction(ls_config *config){
	/* variables */
	uint8_t out1 = OUT1_READ;
	uint8_t out2 = OUT2_READ;
	uint8_t percent;
	
	static uint8_t last_in1 = 0;
	static uint8_t last_in2 = 0;
	
	percent = convert_to_percent(result,config);
	
	/*output 1*/
	if(config->out1_on_value > config->out1_off_value){
		if(percent >= config->out1_on_value) out1 = 1;
		if(percent <= config->out1_off_value) out1 = 0;
	}
	if(config->out1_on_value < config->out1_off_value){
		if(percent <= config->out1_on_value) out1 = 1;
		if(percent >= config->out1_off_value) out1 =0; 
	}
	if(config->out1_on_value == config->out1_off_value){
		if(percent <= config->out1_on_value){
			out1 = 0 ^ config->out1_invert;
		}else{
			out1 = 1 ^ config->out1_invert;
		}
	}
	/*output 2*/
	if(config->out2_on_value > config->out2_off_value){
		if(percent >= config->out2_on_value) out2 = 1;
		if(percent <= config->out2_off_value) out2 = 0;
	}
	if(config->out2_on_value < config->out2_off_value){
		if(percent <= config->out2_on_value) out2 = 1;
		if(percent >= config->out2_off_value) out2 =0; 
	}
	if(config->out2_on_value == config->out2_off_value){
		if(percent <= config->out2_on_value){
			out2 = 0 ^ config->out2_invert;
		}else{
			out2 = 1 ^ config->out2_invert;
		}
	}
	/*input 1*/
	if(!IN1_READ){
		switch(config->in1_action){
			case CALIB_MIN: 	if(last_in1 && !IN1_READ) {// only do on falling edge!
									calibrate_min(config);
								}
								break;
			case CALIB_MAX: 	if(last_in1 && !IN1_READ) {// only do on falling edge!
									calibrate_max(config);
								}
								break;
			case SET_OUT1_ON:		out1 = 1;
								break;
			case SET_OUT1_OFF:		out1 = 0;
								break;
			case SET_OUT2_ON:		out2 = 1;
								break;
			case SET_OUT2_OFF:		out2 = 0;
								break;
			case SAVE_EEPROM:	//TODO
								break;
		}
	}

	/*input 2*/
		if(!IN2_READ){
		switch(config->in2_action){
			case CALIB_MIN: 	if(last_in2 && !IN2_READ) {// only do on falling edge!
									calibrate_min(config);
								}
								break;
			case CALIB_MAX: 	if(last_in2 && !IN2_READ) {// only do on falling edge!
									calibrate_max(config);
								}
								break;
			case SET_OUT1_ON:		out1 = 1;
								break;
			case SET_OUT1_OFF:		out1 = 0;
								break;
			case SET_OUT2_ON:		out2 = 1;
								break;
			case SET_OUT2_OFF:		out2 = 0;
								break;
			case SAVE_EEPROM:	// TODO
								break;
		}
	}
	
	last_in1 = IN1_READ;
	last_in2 = IN2_READ;
	
	/* set outputs */
	if(out1)OUT1_ON;
	else OUT1_OFF;
	if(out2)OUT2_ON;
	else OUT2_OFF;
	
}

void calibrate_min(ls_config *config){
	uint16_t temp;
		if((temp=measure_level())){
			config->min_value = temp;
			
		}
}

void calibrate_max(ls_config *config){
	uint16_t temp;
		if((temp=measure_level())){
			config->max_value = temp;	
		}
}

uint8_t convert_to_percent(uint16_t raw_value, ls_config *config){
	uint32_t temp;
	
	if(raw_value <= config->min_value) return 0;
	if(raw_value >= config->max_value) return 100;
	
	/*
	Formula:
               (raw - min) * 100
    Percent = -------------------
                 (max - min)
	
	with: min <= raw <= max
	*/
	
	//printf("raw_value: %u\r\n", raw_value);
	//printf("min_value: %u\r\n", config->min_value);
	//printf("max_value: %u\r\n", config->max_value);
		
	temp = (raw_value - config->min_value);
	temp = temp * 100;
	//printf("temp: %lu\r\n", temp);
	temp = temp / (config->max_value - config->min_value);
	//printf("temp: %lu\r\n", temp);
	
	return (uint8_t)temp;
}


