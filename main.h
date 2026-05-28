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
#include "SPI.h"

/*EEMEM*/// uint8_t   e_t1_max ;//предел температуры 1 в eeprom

/**Converts a bit number into a 1-byte value. */
#define _BV(bit) (1 << (bit))


//#define WaterPort  PORTD
#define PumpPort    PORTC //управление насосом
#define MixPort	    PORTC //управление мешалкой
//#define CoolerPort PORTD
#define WaterPin    PD5 //
//#define Valve_pin   PC5 //Клапан отбора
#define PumpPin	    PC5 //Насос 
#define MixPin	    PC6 //Мешалка
#define CoolerPin   PD4 //Кулер привола мешалки
#define ZeroCross   PD2 //Переход через 0
#define ScrPort     PC2  // управление симстором
#define On_OffPin   PB4
#define OnPin       PB5
#define Red_warn    0xFB //red
#define Blue_warn   0x7F //blue
#define Yell_warn   0xF7 //yellow
#define Green_warn  0xFD //green
//#define EnPin       PC3
#define StPin       PC4 // индикация соединения по Bluetooth
#define LevelPin    PD6 // уровень в кубе
#define ErrorPin    PB1 // уровень в ёмкости
#define ZummerPin   PD3 // зуммер
#define PrstPumpPin PB2 // управление перестальтикой (Разгон / ШИМ)
#define PrstPumpStp PB3 // управление перестальтикой (Стоп)
//#define AlertPin    PD7 // вход компаратора ADS1115
#define SS          PB4 //
#define MOSI        PB5 //
#define MISO        PB6 //
#define SCK         PB7 //
#define CS          PC7 //
#define CS_LOW()    PORTC &= ~ _BV(CS);
#define CS_HIGH()   PORTC |= _BV(CS);

#define t_pulse_triac 20    //Длительность отпирающего импульса симистора 20 мкс 

#define DIVIDER_RELOAD 2 //Количество повторных запусков для делителя системного таймера,
                           // чтобы получить 1 мс из 16 мкс,частота будет разделена на 64
                           
uint16_t adc_value;////читаем только старший байт
//float new_adc_value;

//EEMEM uint8_t    e_t1_min ;//предел температуры 1 в eeprom


uint8_t power_level;        //Уровень мощности (конвертируемый)
uint8_t _power_level;       //Уровень мощности (неконвертируемый)
uint8_t value_rx;
uint8_t rx_flag;
uint8_t start_flag;         //
uint8_t dist_flag;
uint8_t beer_flag;          //
uint8_t eeprom_save_flag;
int16_t temp1;//температура1
int16_t temp2;//температура2
int16_t temp3;//температура3
int16_t temp4;//
uint16_t pressure;//давление
uint8_t butcount;
uint8_t water;//вода вкл/выкл
uint8_t pump;//насос вкл/выкл
uint8_t t1_max ;//установка предела температуры 1
uint8_t t2_max;//установка предела температуры 2
//uint8_t t3_max;//установка предела температуры 3
uint8_t t1_pause;//уст-ка темп. паузы 1
uint8_t t2_pause;//уст-ка темп. паузы 2
uint8_t t1_time;//уст-ка времен темп. паузы 1
uint8_t t2_time;//уст-ка времен темп. паузы 2
uint8_t mixer;//миксер вкл/выкл
uint8_t cooler;//кулер миксера вкл/выкл
uint8_t signal_flag;//флаг аварии
uint8_t mode;//режим
uint8_t main_temp1;//
//float p;//давление в Паскалях
uint16_t ptemp;//давление в мм рт. ст.
//float k;//коэффициент перерасчёта из Паскалей в мм рт. ст.
uint16_t adc_res;//чтение АЦП (Датчик давления)
//uint8_t borehole;//скважность ШИМ
uint16_t period;// период ШИМ перестальтики
//uint8_t decr;//декремент ШИМ
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
int16_t prst_work;//перестальтика время работы
int16_t _prst_work;//предыдущее значение времени работы
int16_t prst_pause;//перестальтика время паузы
int8_t prst_spd_value;//значение скорости подачи в процентах
uint16_t clpn1_work;//клапан отбора1 время работы
uint16_t clpn1_pause;//клапан отбора1 время паузы
int32_t x1, x2;
uint8_t corr;//коррекция значения температуры
// typedef struct
// {
// 	uint8_t water;//вода вкл/выкл
// 	uint8_t pump;//насос вкл/выкл
// 	uint8_t mixer;//миксер вкл/выкл
// 	uint8_t cooler;//кулер миксера вкл/выкл
// }my_data;

#endif /* MAIN_H_ */