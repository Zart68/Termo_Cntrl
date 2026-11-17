/*
 * Buttons.c
 *
 * Created: 09.05.2025 8:26:45
 *  Author: admin
 */ 
#include "Buttons.h"

uint8_t key_state;// граф автомата
uint8_t btn_pause_state;//
uint8_t counter_lable;//

void Scan_keys()
{
	
	Read_pcf8475();
	Read_pcf8475();
	
	if (pcf_result & (_BV(4))) key_code = 158;//P4 button3 158
	
	if (pcf_result & (_BV(5))) key_code = 175;//P5 button2 175
	
	if (pcf_result & (_BV(6))) key_code = 207;//P6 button1 207
	
	if (pcf_result & (_BV(LevelPin))) {key_code = 208;signal_code = 1;}//P7 датчик уровня в кубе 207
	
 	//if (pcf_result & ((_BV(6)))&&(_BV(4))) key_code = 223;//button1 + button3 223
 	
 	//if (pcf_result & ((_BV(6)))&&(_BV(5))) key_code = 239;//button1 + button2 239
 	
 	//if (pcf_result & ((_BV(4)))&&(_BV(5))) key_code = 191;//button2 + button3 191
     
	if (!(pcf_result & (_BV(ErrorPin1)))) {key_code = 15;signal_code = 2;}//P1 датчик наличия жидкости 1 key_code = 15
	
	if (!(pcf_result & (_BV(ErrorPin2)))) {key_code = 14;signal_code = 3;}//P0 датчик наличия жидкости 2 key_code = 14
	
}


void Read_on_off_btn()
{
	if (!(PINB & _BV(On_OffPin)))
	{
		if(butcount<5) {butcount++; PORTB |= _BV(OnPin);}
		else {PORTD |=_BV(ZummerPin); PORTB &=~ _BV(OnPin); }//Звуковая индикация при выключении
	}														 //Отключение питания
	SetTimerTask(Read_on_off_btn,300);
}
void Read_keys()
{
	Scan_keys();
	
	{
		switch (key_code)
		{

			case 158:
			{
	           Zummer_on();
			   setled();
			  // PORTD |= _BV(Valve1_pin);
			   key_code = 0;
			}break;
			case 175:
			{
				Zummer_on();
				off_led();
				//PORTD &=~ _BV(Valve1_pin);
				//PORTD &=~ _BV(Valve2_pin);	
				key_code = 0;
			}break;
			case 207:
			{
				Zummer_on();
				//PORTD |= _BV(Valve2_pin);	
				key_code = 0;
			}break;
		    case 15:
			{
				Zummer_on();
				key_code = 0;
			}break;
			case 14:
			{
				Zummer_on();
				key_code = 0;
			}break;
			case 208:
			{
				Zummer_on();
				key_code = 0;
			}
		}		
 	}	

	SetTimerTask(Read_keys,1000);
}
void Zummer_off()
{
	PORTD &=~ _BV(ZummerPin);//
}
void Zummer_on()
{
	PORTD |=_BV(ZummerPin);//
	SetTimerTask(Zummer_off,150);
}
void Zummer_long_on()
{
	PORTD |=_BV(ZummerPin);//
	SetTimerTask(Zummer_off,550);
}
