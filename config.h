/*
Filename:       	config.h
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

#ifndef CONFIG_H
#define CONFIG_H



typedef struct configuration{
	uint16_t	max_value;			//raw calibration value for maximum
	uint16_t	min_value;			//raw calibration value for minimum
	uint8_t		out1_on_value;		//value where to switch out1 on. ex: 10 -> on at 10%
	uint8_t		out1_off_value;		//value where to switch out1 off. ex: 5 -> off at 5% 
									// this will result in a hysteresis behavior 
	uint8_t		out1_invert;		// if on==off we will switch without hysteresis ->
									// so we have to use this if the  polarisation does 
									// not fit. this is only in use if on==off!
	
	uint8_t		out2_on_value;		// same bahavior but for out1
	uint8_t		out2_off_value;
	uint8_t		out2_invert;
	uint8_t 	in1_action;			// specifies the action of in1
	uint8_t		in2_action;			
	
	uint16_t	measure_interval;	// measure interval in seconds. 0 = disabled
}ls_config;

#define	DISABLED	0
#define CALIB_MIN	1
#define CALIB_MAX	2
#define	SET_OUT1_ON		3		
#define	SET_OUT1_OFF	4		
#define	SET_OUT2_ON		5		
#define	SET_OUT2_OFF	6	
#define SAVE_EEPROM	10	


void write_config(ls_config *);
void read_config(ls_config *);
ls_config* init_config(void);
ls_config* get_config(void);

#endif