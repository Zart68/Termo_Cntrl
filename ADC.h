/*
 * ADC.h
 *
 * Created: 04.05.2025 6:55:51
 *  Author: admin
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "main.h"



// Значение температуры, возвращаемое если сумма результатов АЦП больше первого значения таблицы
#define TEMPERATURE_UNDER 150
// Значение температуры, возвращаемое если сумма результатов АЦП меньше последнего значения таблицы
#define TEMPERATURE_OVER 1050
// Значение температуры соответствующее первому значению таблицы
#define TEMPERATURE_TABLE_START 150
// Шаг таблицы 
#define TEMPERATURE_TABLE_STEP 10

// Тип каждого элемента в таблице, если сумма выходит в пределах 16 бит - uint16_t, иначе - uint32_t
typedef uint16_t temperature_table_entry_type;
// Тип индекса таблицы. Если в таблице больше 256 элементов, то uint16_t, иначе - uint8_t
typedef uint8_t temperature_table_index_type;
// Метод доступа к элементу таблицы, должна соответствовать temperature_table_entry_type
#define TEMPERATURE_TABLE_READ(i) pgm_read_word(&termo_table[i])
//макрос для запуска преобразования
#define StartConvAdc()  ADCSRA |= _BV(ADSC)


int16_t calc_temperature(temperature_table_entry_type adcsum);
void ADC_init();
void Read_temp();
void Read_temp_one();
void Read_temp_two();
void Read_temp_three();
void Read_signal_codes();// Чтение кодов аварий
void Read_water_leakage();//чтение аварииного наличия жидкости
void Read_pressure();//чтение давления
void ads1115_init();//

#endif /* ADC_H_ */