/*
Filename:       	level_sensor.h
Project:        	Level-Sensor
Copyright:      	Nicolas Kaufmann  mailto: Mr.Spock.NK@gmx.de
Author:         	Nicolas Kaufmann
Creation:       	16.09.2011 
Version:        	22.09.2011
Description:    	Firmware f�r den Level-Sensor. 

Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der 
GNU General Public License, wie von der Free Software Foundation ver�ffentlicht, 
weitergeben und/oder modifizieren, entweder gem�� Version 2 der Lizenz oder 
(nach Ihrer Option) jeder sp�teren Version. 

Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, 
da� es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
Programm  erhalten haben. Der Text der GNU General Public License ist auch 
im Internet unter http://www.gnu.org/licenses/gpl.txt ver�ffentlicht.
Eine inoffizielle deutsche �bersetzung findet sich unter 
http://www.gnu.de/gpl-ger.html. Diese �bersetzung soll nur zu einem besseren 
Verst�ndnis der GPL verhelfen; rechtsverbindlich ist alleine die englischsprachige
Version.
*/

#ifndef LEVEL_SENSOR_H
#define LEVEL_SENSOR_H

#include "config.h"

void init_level_sensor(void);


//------------------ OLD
/* 
return value: 
the measured value

0 if measure is not complete
<0 if error

 */
/*
int32_t measure_level(void);
*/
//_-------------------- END OLD



/*
return value:
measured value
0 if error

*/
uint16_t measure_level(void);

uint16_t get_last_level(void);
void process_ioAction(ls_config *);
uint8_t convert_to_percent(uint16_t, ls_config*);

#endif