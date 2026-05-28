/*
 * ADS11151.c
 *
 * Created: 15.04.2026 6:18:26
 *  Author: admin
 */ 

#include "ADS1115.h"




//write register of ADC

uint8_t ads1115_write_register(uint8_t addr, uint8_t reg, uint16_t data)
{
	i2c_start_wait((addr << 1) + I2C_WRITE);
	i2c_write((uint8_t)reg);
	i2c_write((uint8_t)(data >> 8));
	i2c_write((uint8_t)(data & 0xFF));
	i2c_stop();
	
	return 0;
}

//read register from ADC

uint16_t ads1115_read_register(uint8_t addr, uint8_t reg)
{
	i2c_start_wait((addr << 1) + I2C_WRITE);
	i2c_write(reg);
	i2c_stop();
	
	i2c_rep_start((addr << 1) + I2C_READ);
	uint8_t msb = i2c_readAck();
	uint8_t lsb = i2c_readNak();
	i2c_stop();
	
	uint16_t data = (msb  << 8 | lsb) ;//
	return data;
}

// считывание необработанных данных с одного канала АЦП

uint16_t ads1115_readADC_SingleEnded(uint8_t addr, uint8_t channel, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	// Check channel number
	if(channel > 3)
	{
		return 0;
	}
	//DIS
	uint16_t adc_config = ADS1115_COMP_QUE_CON1	|
				 ADS1115_COMP_LAT_NonLatching |
				 ADS1115_COMP_POL_3_ACTIVELOW |	
				 ADS1115_COMP_MODE_TRADITIONAL | 
				 dr |
				 //DR_128SPS |
				 ADS1115_MODE_CONTINUOUS | 
				 gain;
				 //FSR_6_144;
	
	if(channel == 0)
	{
		adc_config |= ADS1115_MUX_AIN0_GND;
	} 
//  	else if(channel == 1)
//  	{
//  		adc_config |= ADS1115_MUX_AIN1_GND;
//  	} 
//  	else if(channel == 2)
//  	{
//  		adc_config |= ADS1115_MUX_AIN2_GND;
//  	} 
//  	else if(channel == 3)
//  	{
//  		adc_config |= ADS1115_MUX_AIN3_GND;
//  	}
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	_delay_ms(8);
	
	return ads1115_read_register(addr, ADS1115_REG_CONVERSION) >> 0;
}

// считывание необработанных данных / результат преобразования АЦП:
// разница между P: AIN0 и N: AIN1

int16_t ads1115_readADC_Diff_A0_1(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN0_AIN1;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	_delay_ms(8);
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}

//read raw data / conversion result of ADC:
//разница между P: AIN0 и N: AIN3

int16_t ads1115_readADC_Diff_A0_3(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
ADS1115_COMP_LAT_NonLatching |
ADS1115_COMP_POL_3_ACTIVELOW |
ADS1115_COMP_MODE_TRADITIONAL |
dr |
//ADS1115_DR_128SPS |
ADS1115_MODE_SINGLE |
gain;
//FSR_6_144;

adc_config |= ADS1115_MUX_AIN0_AIN3;

adc_config |= ADS1115_OS_SINGLE;

ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
_delay_ms(8);

return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}


//    считайте необработанные данные / результат преобразования АЦП:
//    разница между P: AIN1 и N: AIN3

int16_t ads1115_readADC_Diff_A1_3(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN1_AIN3;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	_delay_ms(8);
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}

//  считайте необработанные данные / результат преобразования АЦП:
//  разница между P: AIN2 и N: AIN3

int16_t ads1115_readADC_Diff_A2_3(uint8_t addr, ads1115_datarate dr, ads1115_fsr_gain gain)
{
	uint16_t adc_config = ADS1115_COMP_QUE_DIS	|
						  ADS1115_COMP_LAT_NonLatching |
						  ADS1115_COMP_POL_3_ACTIVELOW |	
						  ADS1115_COMP_MODE_TRADITIONAL | 
						  dr |
						  //ADS1115_DR_128SPS |
						  ADS1115_MODE_SINGLE | 
						  gain;
						  //FSR_6_144;
	
	adc_config |= ADS1115_MUX_AIN2_AIN3;
	
	adc_config |= ADS1115_OS_SINGLE;	
	
	ads1115_write_register(addr, ADS1115_REG_CONFIG, adc_config);
	_delay_ms(8);
	
	return (int16_t)ads1115_read_register(addr, ADS1115_REG_CONVERSION);
}