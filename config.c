/*
Filename:       	config.c
Project:        	Level-Sensor
Copyright:      	Nicolas Kaufmann  mailto: Mr.Spock.NK@gmx.de
Author:         	Nicolas Kaufmann
Creation:       	20.09.2011 
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

#include <stdint.h>
#include <avr/eeprom.h>

#include "config.h"

ls_config config_ee EEMEM = {
	5000,			//max_value
	500,			//min_value
	
	10,				//out1_on_value
	5,				//out1_off_value
	0,				//out1_invert
	
	85,				//out2_on_value
	90,				//out2_off_value
	0,				//out2_invert
	
	DISABLED,		//in1_action = ;
	DISABLED,		//in2_action = ;
	
	60				//measure_interval
};

static ls_config config;

void write_config(ls_config *config){
	eeprom_write_block (config, &config_ee, sizeof(ls_config));	
	
	return;
}

void read_config(ls_config *config){
	eeprom_read_block (config, &config_ee, sizeof(ls_config));
	
	return;
}

ls_config* init_config(void){
	read_config(&config);
	return &config;
}

ls_config* get_config(void){
	return &config;

}