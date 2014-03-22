/*
Filename:       	main.h
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

#ifndef MAIN_H
#define MAIN_H

/*
Configuration:
*/
 
#define RS232			// This will enable the RS232 routines
#define TWI				// This will enable the TWI routines
#define TWI_TIMEOUT		// Enable a Timeout for TWI

//#define MEASURE_DEBUG	// Debug message after every automatic measurement
//#define TWI_DEBUG		// Debug messages for TWI
#define TWI_TIMEOUT_DEBUG		// Debug messages for TWI_TIMEOUT



int main(void);


#endif