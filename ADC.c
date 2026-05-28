/*
 * ADC.c
 *
 * Created: 04.05.2025 6:55:33
 *  Author: admin
 */ 
#include "ADC.h"

//extern unsigned int adc_value;
char high_adc=0,low_adc=0;


/* Таблица суммарного значения АЦП в зависимости от температуры. От большего значения к меньшему
   Для построения таблицы использованы следующие парамертры:
     R1(T1): 100кОм(25°С)
     Таблица R/T характеристик: EPCOS R/T:4901; B25/100:3950K
     Схема включения: A
     Ra: 15кОм
     Напряжения U0/Uref: 5В/5В
*/
// const temperature_table_entry_type termo_table[] PROGMEM = {
// 	62603, 62459, 62310, 62155, 61995, 61828, 61655, 61475,
// 	61289, 61096, 60897, 60690, 60477, 60256, 60028, 59793,
// 	59550, 59299, 59041, 58775, 58501, 58215, 57920, 57618,
// 	57307, 56988, 56661, 56327, 55984, 55634, 55275, 54906,
// 	54530, 54146, 53753, 53353, 52944, 52527, 52102, 51670,
// 	51231, 50786, 50334, 49876, 49411, 48940, 48461, 47976,
// 	47485, 46989, 46489, 45975, 45458, 44936, 44410, 43880,
// 	43345, 42807, 42267, 41724, 41179, 40633, 40087, 39539,
// 	38991, 38442, 37892, 37341, 36791, 36242, 35694, 35145,
// 	34597, 34051, 33507, 32967, 32427, 31890, 31357, 30828,
// 	30303, 29779, 29259, 28745, 28235, 27730, 27228, 26732,
// 	26241, 25756, 25277, 24801, 24332, 23869, 23412, 22961,
// 	22515, 22075, 21642, 21216, 20796, 20386, 19983, 19586,
// 	19196, 18813, 18435, 18064, 17699, 17341, 16989
// };
//0-155 шаг 1градус
const temperature_table_entry_type termo_table[] PROGMEM = {
	62603, 62459, 62310, 62155, 61995, 61828, 61655, 61475,
	61289, 61096, 60897, 60690, 60477, 60256, 60028, 59793,
	59550, 59299, 59041, 58775, 58501, 58215, 57920, 57618,
	57307, 56988, 56661, 56327, 55984, 55634, 55275, 54906,
	54530, 54146, 53753, 53353, 52944, 52527, 52102, 51670,
	51231, 50786, 50334, 49876, 49411, 48940, 48461, 47976,
	47485, 46989, 46489, 45975, 45458, 44936, 44410, 43880,
	43345, 42807, 42267, 41724, 41179, 40633, 40087, 39539,
	38991, 38442, 37892, 37341, 36791, 36242, 35694, 35145,
	34597, 34051, 33507, 32967, 32427, 31890, 31357, 30828,
	30303, 29779, 29259, 28745, 28235, 27730, 27228, 26732,
	26241, 25756, 25277, 24801, 24332, 23869, 23412, 22961,
	22515, 22075, 21642, 21216, 20796, 20386, 19983, 19586,
	19196, 18813, 18435, 18064, 17699, 17341, 16989, 16641,
	16299, 15964, 15635, 15312, 14995, 14684, 14379, 14080,
	13787, 13501, 13221, 12947, 12679, 12416, 12157, 11904,
	11657, 11415, 11178, 10944, 10715, 10491, 10272, 10057,
	9847, 9641, 9440, 9243, 9050, 8861, 8675, 8494,
	8317, 8144, 7974, 7808, 7646, 7487, 7332, 7180,
	7031, 6886, 6744, 6605
};
// Функция вычисляет значение температуры в десятых долях градусов Цельсия
// в зависимости от суммарного значения АЦП.
int16_t calc_temperature(temperature_table_entry_type adcsum) {
	temperature_table_index_type left = 0;//
	temperature_table_index_type right = (sizeof(termo_table) / sizeof(termo_table[0])) - 1;//
	temperature_table_entry_type thigh = TEMPERATURE_TABLE_READ(right);//
	
	// Проверка выхода за пределы и граничных значений
	if (adcsum <= thigh) {
		#ifdef TEMPERATURE_UNDER
		if (adcsum < thigh)
		return TEMPERATURE_UNDER;
		#endif
		return TEMPERATURE_TABLE_STEP * right + TEMPERATURE_TABLE_START;
	}
	temperature_table_entry_type tlow = TEMPERATURE_TABLE_READ(0);
	if (adcsum >= tlow) {
		#ifdef TEMPERATURE_OVER
		if (adcsum > tlow)
		return TEMPERATURE_OVER;
		#endif
		return TEMPERATURE_TABLE_START;
	}

	// Двоичный поиск по таблице
	while ((right - left) > 1) {
		temperature_table_index_type m = (left + right)/2 /*>> 1*/;// вычисление средней точки
		temperature_table_entry_type mid = TEMPERATURE_TABLE_READ(m);
		if (adcsum > mid) {// Если ключ больше arr[mid], игнорируем левую половину
			right = m;
			} else {// Если ключ меньше или равен arr[mid],игнорируем правую половину
			left = m;
		}
	}
	temperature_table_entry_type vl = TEMPERATURE_TABLE_READ(left);
	if (adcsum >= vl) {
		return left * TEMPERATURE_TABLE_STEP + TEMPERATURE_TABLE_START;
	}
	temperature_table_entry_type vr = TEMPERATURE_TABLE_READ(right);
	temperature_table_entry_type vd = vl - vr;
	int16_t res = TEMPERATURE_TABLE_START + right * TEMPERATURE_TABLE_STEP;
	if (vd) {
		// Линейная интерполяция
		res -= ((TEMPERATURE_TABLE_STEP * (int32_t)(adcsum - vr) + (vd >> 1)) / vd);
		//res -= corr;
	}
	return res;
}


ISR(ADC_vect)
{
 	switch(ADMUX)
 	{		
		case 0x40: ADMUX = 0x41; break;// 4 channel ADC 		
  		case 0x41: ADMUX = 0x42; break;// 0 channel ADC					 
  		case 0x42: ADMUX = 0x40; break;// 1 channel ADC 		 
     	
 	}
}
void ADC_init()
{	 
	 ADMUX = 0x40;// Настроим опорное напряжение на VCC  ADC0
	 // Включим АЦП и установим коэффициент деления на 128
	 ADCSRA =_BV(ADEN)/*|_BV(ADSC)*/|_BV(ADIE)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
}

void Read_temp_one()
{
	temperature_table_entry_type summ1 = 0;
	for (uint8_t i = 0; i < 64; i++)
	{		
		StartConvAdc();// Начнем преобразование
		do {summ1 += ADC;}
		while (!(ADCSRA & _BV(ADSC)));
	}
	temp1 = calc_temperature(summ1) - corr;				
}

void Read_temp_two()
{
	temperature_table_entry_type summ2 = 0;
	for (uint8_t i = 0; i < 64; i++) {
		StartConvAdc();// Начнем преобразование
		do {summ2 += ADC;}
		while (!(ADCSRA & _BV(ADSC)));
	}
	temp2 = calc_temperature(summ2) - corr;	
}

void Read_temp_three()
{
	temperature_table_entry_type summ3 = 0;
	for (uint8_t i = 0; i < 64; i++) {
		StartConvAdc();// Начнем преобразование;
		do {summ3 += ADC;}
		while (!(ADCSRA & _BV(ADSC)));
	}
	temp3 = calc_temperature(summ3) - corr;	
}





void Read_pressure()
{
	


	adc_res = ads1115_readADC_SingleEnded(ADS1115_ADDR_GND,0,ADS1115_DR_8SPS,ADS1115_PGA_4_096);
	//adc_res = Read_MCP3208(7);
	adc_value = adc_res	>> 8;//читаем только старший байт
	ptemp = adc_value ;

 	switch(adc_value)
 	{
// 		case 4: ptemp = 0; break;//атмосферное давление
// 		case 6: ptemp = 1;break;
// 		case 8: ptemp = 2;break;
// 		case 10:ptemp = 3;break;
// 		case 12:ptemp = 4;break;
// 		case 14:ptemp = 5;break;
// 		case 16:ptemp = 6;break;
// 		case 18:ptemp = 7;break;
// 		case 20:ptemp = 8;break;
// 		case 22:ptemp = 9;break;
// 		case 24:ptemp = 10;break;
// 		case 26:ptemp = 11;break;
// 		case 28:ptemp = 12;break;
// 		case 30:ptemp = 13;break;
// 		case 32:ptemp = 14;break;
// 		case 34:ptemp = 15;break;
// 		case 36:ptemp = 16;break;
// 		case 38:ptemp = 17;break;
// 		case 40:ptemp = 18;break;
// 		case 42:ptemp = 19;break;
// 		case 44:ptemp = 20;break;
// 		case 46:ptemp = 21;break;
// 		case 48:ptemp = 22;break;
// 		case 50:ptemp = 23;break;
// 		case 52:ptemp = 24;break;
// 		case 54:ptemp = 25;break;
// 		case 56:ptemp = 26;break;
// 		case 58:ptemp = 27;break;
// 		case 60:ptemp = 28;break;
// 		case 62:ptemp = 29;break;
// 		case 64:ptemp = 30;break;
// 		case 66:ptemp = 31;break;
// 		case 68:ptemp = 32;break;
// 		case 70:ptemp = 33;break;
// 		case 72:ptemp = 34;break;
// 		case 74:ptemp = 35;break;
// 		case 76:ptemp = 36;break;
// 		case 78:ptemp = 37;break;
// 		case 80:ptemp = 38;break;//38 mmHg
// 	
 	}
	
	SetTimerTask(Read_pressure,150);
}
void Read_temp()
{
    switch(ADMUX)
  	{
 		case 0x40:Read_temp_one();break;
 		case 0x41:Read_temp_two();break;
 		case 0x42:Read_temp_three();break;
 	}

 	SetTimerTask(Read_temp,750);
}


void Water_Off()
{
	PORTD &=~_BV(WaterPin);water=0;//выключение вода
}
void Read_signal_codes()// Чтение кодов аварий
{		
	if(!(PINB & _BV(ErrorPin))) //высокий уровень в ёмкости
	{
		signal_flag = 1;
		PORTB |=_BV(ZummerPin);
	}
	else 	
	{
		signal_flag = 0;//отмена
		//PORTB &=~ _BV(ZummerPin);
	}
    if(!(PIND & _BV(LevelPin))) //низкий уровень в 1 контуре
	{   
		signal_flag = 2;
		_power_level =147;// 0% стартовое значение уровня мощности(147)
		SetTimerTask(Water_Off,6000);//выключение воды через 6 сек
	}	
	if ((signal_flag == 1)||(signal_flag == 2))
	{
		I2C_SendByteByADDR(Red_warn,pcf8574_addr);//индикация  аварийного события
	}	                                                                        
  	SetTimerTask(Read_signal_codes,250); 	   	  	  
 }	
















