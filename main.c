/*
Filename:       	main.c
Project:        	Level-Sensor
Copyright:      	Nicolas Kaufmann  mailto: Mr.Spock.NK@gmx.de
Author:         	Nicolas Kaufmann
Creation:       	10.08.2011 
Version:        	22.09.2011
Description:    	Firmware für den Level-Sensor. 

Dieses Programm verwendet die Uart Routine von Peter Fleury, die entsprechenden 
Lizenzinformationen sind in den Dateien uart.c und uart.h zu finden

Die TWI Slave Routine stammt von Martin Junghans. Eie entsprechenden 
Lizenzinfirmationen sind in den Dateien twislave.c und twislave.h zu finden.
Diese Routinen wurden dem Anwendungszweck entsprechend verändert bzw. erweitert.

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
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "level_sensor.h"
#include "config.h"


/*
Configuration:
*/
 
#define RS232			// This will enable the RS232 routines
#define TWI				// This will enable the TWI routines

//#define MEASURE_DEBUG	// Debug message after every automatic measurement


#ifdef RS232
	#include "uart.h"
	#include "parse_uart.h"
	#include <avr/pgmspace.h>
	static FILE mystdout = FDEV_SETUP_STREAM(uart_putc, NULL,_FDEV_SETUP_WRITE);
	#define UART_BAUD_RATE      9600
#endif

#ifdef TWI
	#include "twislave.h"
	#define SLAVE_ADDR 0x10
#endif

/*
TWI ADDRESS MAP:
0	:	last
1	:	command		: 1 measure, 2 save eeprom, 3 read eeprom 
2-3	:	last raw
4-5	:	max_value
6-7 :	min_value
8	:	out1_on_value
9	:	out1_off_value
10	:	out1_invert	
11	:	out2_on_value
12	:	out2_off_value
13	:	out2_invert	
14	:	in1_action
15	:	in2_action


*/


void init(void);
void timer2_init(void);

volatile uint8_t tick;
volatile uint16_t tick_count;

//unsigned long result=0;
	
int main (){
	//Vars
	ls_config *config;
	config = init_config();
	
	uint16_t sec_timer = 0;
	
	tick = 0;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tick_count = 0;		// this var has 2 byte.. so do this atomic
	}
	
	uint16_t internal_tick_count;
	
	// Init devices
	init();
	
	
	#ifdef RS232
	uart_puts_P("Init done\r\n\r\n");
	//_delay_ms(500);
	#endif
	
	
	//uint16_t temp;

	while(1){
				
		#ifdef RS232
		
		read_uart();
		#endif
		
		#ifdef TWI
		execute_twi_command();
		#endif
		
		if(tick) 	// this will be executed every 1 ms
		{		
			tick = 0;
			process_ioAction(config); // io action processing
		
			
		}
		
		
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			internal_tick_count = tick_count;
		}
		if(internal_tick_count >= 1000)	// this will be executed every 1 sec  
		{	
			ATOMIC_BLOCK(ATOMIC_FORCEON)
			{
				tick_count = 0;
			}
			sec_timer++;		// count the seconds for the measure interval
			//printf("sec_timer: %u\r\n", sec_timer);
		}	
		
		
		if((sec_timer >= config->measure_interval) && config->measure_interval){
			sec_timer=0;
			measure_level();
			#ifdef MEASURE_DEBUG
			printf("m:%u\r\n",get_last_level());
			#endif
		}
		
	}
	
	return 0;
}


void init(){
	sei();
	//init Uart
	#ifdef RS232
		uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
		stdout = &mystdout; //stdout auf uart setzen für printf
		uart_puts_P("Uart Init done\r\n");
	#endif
	
	//Init TWI Slave
	#ifdef TWI	
		init_twi_slave(SLAVE_ADDR);
		#ifdef RS232
		uart_puts_P("TWI Init done\r\n");
		#endif
	#endif

	init_level_sensor();
	#ifdef RS232
	uart_puts_P("level measure Init done\r\n");
	#endif
	
	timer2_init();
	#ifdef RS232
	uart_puts_P("Timer2 Init done\r\n");
	#endif
	
	
	//enable interrupts
	sei();
	
	return;
}

void timer2_init(){	
	TCCR2 |= (1 << CS22);	//Prescaler 64
	TCCR2 |= (1 << WGM21);	// CTC mode
	OCR2 = (uint8_t)(F_CPU / 64.0 * 1e-3 - 0.5);  
	TIMSK |= (1 << OCIE2);	//Output compare interrupt enable
	return;
}

ISR( TIMER2_COMP_vect ){
	tick=1;
	tick_count++;
}


