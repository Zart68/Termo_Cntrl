/*
 * main.h
 *
 * Created: 15.03.2025 6:19:40
 *  Author: Zart68
 */ 
 

#ifndef MAIN_H_
#define MAIN_H_


#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/BAUD/16-1


#define SCL_CLOCK  100000UL // I2C clock in Hz 100kHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include <avr/wdt.h>
#include <compat/twi.h>

#include "avrlibtypes.h"
#include "avrlibdefs.h"
#include "EERTOS.h"
#include "EERTOSHAL.h"
#include "ADC.h"
#include "usart.h"
#include "buttons.h"
#include "Convert.h"
#include "twi.h"
#include "PWM_cntrl.h"
#include "ADS1115.h"
#include "lcdtwi.h"

/*EEMEM*/// uint8_t   e_t1_max ;//ïðåäåë òåìïåðàòóðû 1 â eeprom

/**Converts a bit number into a 1-byte value. */
#define _BV(bit) (1 << (bit))


//#define WaterPort  PORTD
#define PumpPort    PORTC //óïðàâëåíèå íàñîñîì
#define MixPort	    PORTC //óïðàâëåíèå ìåøàëêîé
//#define CoolerPort PORTD
#define WaterPin    PD5 //
//#define Valve_pin   PC5 //Êëàïàí îòáîðà
#define PumpPin	    PC5 //Íàñîñ 
#define MixPin	    PC6 //Ìåøàëêà
#define CoolerPin   PD4 //Êóëåð ïðèâîëà ìåøàëêè
#define ZeroCross   PD2 //Ïåðåõîä ÷åðåç 0
#define ScrPort     PC2  // óïðàâëåíèå ñèìñòîðîì
#define On_OffPin   PB4
#define OnPin       PB5
#define Red_warn    0xFB //red
#define Blue_warn   0x7F //blue
#define Yell_warn   0xF7 //yellow
#define Green_warn  0xFD //green
//#define EnPin       PC3
#define StPin       PC4 // èíäèêàöèÿ ñîåäèíåíèÿ ïî Bluetooth
#define LevelPin    PD6 // óðîâåíü â êóáå
#define ErrorPin    PB1 // óðîâåíü â ¸ìêîñòè
#define ZummerPin   PD3 // çóììåð
#define PrstPumpPin PB2 // óïðàâëåíèå ïåðåñòàëüòèêîé (Ðàçãîí / ØÈÌ)
#define PrstPumpStp PB3 // óïðàâëåíèå ïåðåñòàëüòèêîé (Ñòîï)
//#define AlertPin    PD7 // âõîä êîìïàðàòîðà ADS1115
#define SS          PB4 //
#define MOSI        PB5 //
#define MISO        PB6 //
#define SCK         PB7 //
#define CS          PC7 //
#define CS_LOW()    PORTC &= ~ _BV(CS);
#define CS_HIGH()   PORTC |= _BV(CS);

#define t_pulse_triac 20    //Äëèòåëüíîñòü îòïèðàþùåãî èìïóëüñà ñèìèñòîðà 20 ìêñ 

#define DIVIDER_RELOAD 2 //Êîëè÷åñòâî ïîâòîðíûõ çàïóñêîâ äëÿ äåëèòåëÿ ñèñòåìíîãî òàéìåðà,
                           // ÷òîáû ïîëó÷èòü 1 ìñ èç 16 ìêñ,÷àñòîòà áóäåò ðàçäåëåíà íà 64
                           
uint16_t adc_value;////÷èòàåì òîëüêî ñòàðøèé áàéò
//float new_adc_value;

//EEMEM uint8_t    e_t1_min ;//ïðåäåë òåìïåðàòóðû 1 â eeprom


uint8_t power_level;        //Óðîâåíü ìîùíîñòè (êîíâåðòèðóåìûé)
uint8_t _power_level;       //Óðîâåíü ìîùíîñòè (íåêîíâåðòèðóåìûé)
uint8_t value_rx;
uint8_t rx_flag;
uint8_t start_flag;         //
uint8_t dist_flag;
uint8_t beer_flag;          //
uint8_t eeprom_save_flag;
int16_t temp1;//òåìïåðàòóðà1
int16_t temp2;//òåìïåðàòóðà2
int16_t temp3;//òåìïåðàòóðà3
int16_t temp4;//
uint16_t pressure;//äàâëåíèå
uint8_t butcount;
uint8_t water;//âîäà âêë/âûêë
uint8_t pump;//íàñîñ âêë/âûêë
uint8_t t1_max ;//óñòàíîâêà ïðåäåëà òåìïåðàòóðû 1
uint8_t t2_max;//óñòàíîâêà ïðåäåëà òåìïåðàòóðû 2
//uint8_t t3_max;//óñòàíîâêà ïðåäåëà òåìïåðàòóðû 3
uint8_t t1_pause;//óñò-êà òåìï. ïàóçû 1
uint8_t t2_pause;//óñò-êà òåìï. ïàóçû 2
uint8_t t1_time;//óñò-êà âðåìåí òåìï. ïàóçû 1
uint8_t t2_time;//óñò-êà âðåìåí òåìï. ïàóçû 2
uint8_t mixer;//ìèêñåð âêë/âûêë
uint8_t cooler;//êóëåð ìèêñåðà âêë/âûêë
uint8_t signal_flag;//ôëàã àâàðèè
uint8_t mode;//ðåæèì
uint8_t main_temp1;//
//float p;//äàâëåíèå â Ïàñêàëÿõ
uint16_t ptemp;//äàâëåíèå â ìì ðò. ñò.
//float k;//êîýôôèöèåíò ïåðåðàñ÷¸òà èç Ïàñêàëåé â ìì ðò. ñò.
uint16_t adc_res;//÷òåíèå ÀÖÏ (Äàò÷èê äàâëåíèÿ)
//uint8_t borehole;//ñêâàæíîñòü ØÈÌ
uint16_t period;// ïåðèîä ØÈÌ ïåðåñòàëüòèêè
//uint8_t decr;//äåêðåìåíò ØÈÌ
uint8_t mis;
char lcd_buffer[16];
char temp1_buffer[16];
char temp2_buffer[16];
char temp3_buffer[16];
char low_adc;
char hight_adc;
uint16_t adc_calibration_value;
uint8_t adc_measure_flag;
uint16_t press;
int16_t prst_work;//ïåðåñòàëüòèêà âðåìÿ ðàáîòû
int16_t _prst_work;//ïðåäûäóùåå çíà÷åíèå âðåìåíè ðàáîòû
int16_t prst_pause;//ïåðåñòàëüòèêà âðåìÿ ïàóçû
int8_t prst_spd_value;//çíà÷åíèå ñêîðîñòè ïîäà÷è â ïðîöåíòàõ
uint16_t clpn1_work;//êëàïàí îòáîðà1 âðåìÿ ðàáîòû
uint16_t clpn1_pause;//êëàïàí îòáîðà1 âðåìÿ ïàóçû
int32_t x1, x2;
uint8_t corr;//êîððåêöèÿ çíà÷åíèÿ òåìïåðàòóðû
// typedef struct
// {
// 	uint8_t water;//âîäà âêë/âûêë
// 	uint8_t pump;//íàñîñ âêë/âûêë
// 	uint8_t mixer;//ìèêñåð âêë/âûêë
// 	uint8_t cooler;//êóëåð ìèêñåðà âêë/âûêë
// }my_data;

#endif /* MAIN_H_ */
