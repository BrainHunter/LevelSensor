/*
Filename:       	parse_uart.h
Project:        	Level-Sensor
Copyright:      	Nicolas Kaufmann  mailto: Mr.Spock.NK@gmx.de
Author:         	Nicolas Kaufmann
Creation:       	20.09.2011 
Version:        	22.09.2011
Description:    	Firmware f�r den Level-Sensor. 

Dieses Programm verwendet die Uart Routine von Peter Fleury, die entsprechenden 
Lizenzinformationen sind in den Dateien uart.c und uart.h zu finden

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

#ifndef PRARSE_UART_H
#define PRARSE_UART_H

#define BUFFER_LEN 30

void read_uart(void);


#endif