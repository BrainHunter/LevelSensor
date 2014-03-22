/*
Filename:       	parse_uart.c
Project:        	Level-Sensor
Copyright:      	Nicolas Kaufmann  mailto: Mr.Spock.NK@gmx.de
Author:         	Nicolas Kaufmann
Creation:       	20.09.2011 
Version:        	22.09.2011
Description:    	Firmware für den Level-Sensor. 

Dieses Programm verwendet die Uart Routine von Peter Fleury, die entsprechenden 
Lizenzinformationen sind in den Dateien uart.c und uart.h zu finden

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

#include <stdint.h>
#include <stdio.h>
#include <string.h>	
#include <avr/pgmspace.h>

#include "parse_uart.h"
#include "uart.h"
#include "level_sensor.h"
#include "config.h"
#include "main.h"
#include "twislave.h"


void parse(char*);
void print_help(void);
void print_status(void);
void measure_raw(void);
void measure_percent(void);
uint8_t set_var(char*);

void print_OK(void){
	uart_puts_P("OK\r\n");
}

void print_error(void){
	uart_puts_P("ERROR WHILE MESURING\r\n");
}

void parse(char* buffer){
	if(!strcmp("h\r",buffer)){
		print_help();
		//buffer[0] = '\0';
		return;
	}
	if(!strcmp("raw\r",buffer)){
		measure_raw();
		//buffer[0] = '\0';
		return;
	}
	if(!strcmp("s\r",buffer)){		// get status
		print_status();
		//buffer[0] = '\0';
		return;
	}
	if(!strncmp("set ",buffer,4)){	//set variables
		if(set_var(buffer+4)){
			print_OK();
			return;
		}//buffer[0] = '\0';
	}
	
	if(!strcmp("w\r",buffer)){		//Write to eeprom
		write_config(get_config());
		print_OK();
		//buffer[0] = '\0';
		return;
	}
	if(!strcmp("r\r",buffer)){		//read from eeprom
		read_config(get_config());
		print_OK();
		//buffer[0] = '\0';
		return;
	}
	if(!strcmp("m\r",buffer)){		//measure and convert to percentage
		measure_percent();
		//buffer[0] = '\0';
		return;
	}
	#ifdef TWI_TIMEOUT
	if(!strcmp("e\r",buffer)){		//
		printf("twi error: %d\r\n", twi_error);
		//buffer[0] = '\0';
		return;
	}
	#endif
	if(!strcmp("\r",buffer)){
		//buffer[0] = '\0';
		return;
	}
	uart_puts_P("Parse Error\r\n");
}

uint8_t set_var(char* buffer){
	ls_config *config;
	config = get_config();
	unsigned int temp;
	/*
	uart_puts_P("\r\n");
	uart_puts(buffer);
	uart_puts_P("\r\n");
	*/
	if(sscanf_P(buffer,PSTR("min_value %u\r"),&temp)){
		//buffer[0] = '\0';
		config->min_value = (uint16_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("max_value %u\r"),&temp)){
		//buffer[0] = '\0';
		config->max_value = (uint16_t)temp;
		return 1;
	}	
	if(sscanf_P(buffer,PSTR("out1_on_value %u\r"),&temp)){
		//buffer[0] = '\0';
		config->out1_on_value = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("out1_off_value %u\r"),&temp)){
		//buffer[0] = '\0';
		config->out1_off_value = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("out1_invert %u\r"),&temp)){
		//buffer[0] = '\0';
		config->out1_invert = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("out2_on_value %u\r"),&temp)){
		//buffer[0] = '\0';
		config->out2_on_value = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("out2_off_value %u\r"),&temp)){
		//buffer[0] = '\0';
		config->out2_off_value = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("out2_invert %u\r"),&temp)){
		//buffer[0] = '\0';
		config->out2_invert = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("in1_action %u\r"),&temp)){
		//buffer[0] = '\0';
		config->in1_action = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("in2_action %u\r"),&temp)){
		//buffer[0] = '\0';
		config->in2_action = (uint8_t)temp;
		return 1;
	}
	if(sscanf_P(buffer,PSTR("measure_interval %u\r"),&temp)){
		//buffer[0] = '\0';
		config->measure_interval = (uint16_t)temp;
		return 1;
	}
	return 0;
}

void print_status(){
	ls_config *config;
	config=get_config();
	
	printf_P(PSTR("last: %u%%\r\n"),convert_to_percent(get_last_level(),get_config()));
	printf_P(PSTR("last raw: %u \r\n"),get_last_level());
	
	printf_P(PSTR("MAX value: %u \r\n"),config->max_value);
	printf_P(PSTR("MIN value: %u \r\n"),config->min_value);
	
	printf_P(PSTR("IN1  action: %u \r\n"),config->in1_action);
	printf_P(PSTR("IN2  action: %u \r\n"),config->in2_action);
	
	printf_P(PSTR("OUT1 on_value: %u \r\n"),config->out1_on_value);
	printf_P(PSTR("OUT1 off_value: %u \r\n"),config->out1_off_value);
	printf_P(PSTR("OUT1 invert: %u \r\n"),config->out1_invert);	
	printf_P(PSTR("OUT2 on_value: %u \r\n"),config->out2_on_value);
	printf_P(PSTR("OUT2 off_value: %u \r\n"),config->out2_off_value);
	printf_P(PSTR("OUT2 invert: %u \r\n"),config->out2_invert);		
	
	printf_P(PSTR("measure Interval: %u \r\n"),config->measure_interval);		
	
	/*
	printf_P(PSTR("IN1  status: %u \r\n"),321);
	printf_P(PSTR("IN2  status: %u \r\n"),321);
	printf_P(PSTR("OUT1  status: %u \r\n"),321);
	printf_P(PSTR("OUT2  status: %u \r\n"),321);
	*/
	return;
}

void print_help(){
	// HILFE
	uart_puts_P("HELP: Type\r\n");
	uart_puts_P("\"raw\"  raw measure\r\n");
	uart_puts_P("\"m\"    measure percentage\r\n");
	uart_puts_P("\"set\"  ex: set max_value 4000\r\n");
	uart_puts_P("\"r\"    read eeprom\r\n");
	uart_puts_P("\"w\"    write eeprom\r\n");
	uart_puts_P("\"s\"    Display all config variables\r\n");
	uart_puts_P("\"h\"    to display this help\r\n");
}

void measure_raw(){
		/*int32_t temp;
		while(!(temp=measure_level()));
		if(temp < 0){
			print_error();
		}else{
			printf_P(PSTR("ICR1 = %u \r\n"), (uint16_t)temp);
		}
		*/
		uint16_t temp;
		if((temp=measure_level())){
			printf_P(PSTR("ICR1 = %u \r\n"), (uint16_t)temp);
		}else{
			print_error();
		}
		
}

void measure_percent(){
		/*int32_t temp;
		while(!(temp=measure_level()));
		if(temp < 0){
			print_error();
		}else{
			printf_P(PSTR("level = %u %% \r\n"), convert_to_percent((uint16_t)temp,get_config()));
		}
		*/
		uint16_t temp;
		if((temp=measure_level())){
			printf_P(PSTR("level = %u %% \r\n"), convert_to_percent((uint16_t)temp,get_config()));
		}else{
			print_error();
		}
}

void read_uart(){
	static char buffer[BUFFER_LEN];
	static uint8_t i=0;
	unsigned int c;
	
	// Char von der uart einlesen falls vorhanden
	c = uart_getc();
	if ( !(c & UART_NO_DATA) ){						// es liegt ein neuer char an 
		if ( c & UART_FRAME_ERROR ){
			uart_puts_P("UART Frame Error: ");		//UART Frame Error: 
		}
		if ( c & UART_OVERRUN_ERROR ){
			uart_puts_P("UART Overrun Error: "); 	//UART Overrun Error: 
		}
		if ( c & UART_BUFFER_OVERFLOW ){
		   uart_puts_P("Buffer overflow error: "); 	//Buffer overflow error: 
		}
		uart_putc( (unsigned char)c );
				
		buffer[i++] = (unsigned char)c;		// in den String schreiben
		if(i==BUFFER_LEN){					// auf überlauf prüfen
			i=0;
			uart_puts_P("What? try again!\r\n");
		}
		buffer[i] = '\0';					// String terminieren
							
		if(buffer[i-1] == '\r'){			// überprüfen auf newline -> rücksetzen
			uart_puts_P("\r\n");
			i=0;
			parse(buffer);					// string durchsuchen und entsprechend reagieren
		}	
						
	}	
	return;
}