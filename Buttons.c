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
void button_pause_time_counter()
{
	switch(counter_lable)
	{
		case 1:btn_pause_state = 1;//подавление дребезга
		break;
		case 2:btn_pause_state = 2;//кнопека нажата
		break;
		case 3:btn_pause_state = 3;//кнопка удерживается
		break;
		case 4:btn_pause_state = 4;//автоповтор
		break;
	}
	
}
void Scan_keys()
{
	
	Read_pcf8475();
	
	if (pcf_result & (_BV(4)))key_code = 158;//P4 button1 158
	
	if (pcf_result & (_BV(5))) key_code = 174;//P4 button1 174
	
	if (pcf_result & (_BV(6))) key_code = 206;//P4 button1 206

}
void kbd_drv()
{
	Scan_keys();
	
	switch (key_state)
	{
		case 0:
		if (key_code>0)
		{
			_key_code = key_code;
			counter_lable=1;
			SetTimerTask(button_pause_time_counter,DEBOUNCE_DELAY);
			//btn_pause_state = 1;
			key_state = 1;
		}break;
		case 1:
		if(btn_pause_state ==1)
		key_state =	2;
		break;
		case 2:
		if (key_code == _key_code)
		{
			counter_lable=2;
			SetTimerTask(button_pause_time_counter,HOLD_DELAY);
			buttonstatus=1;// кнопка нажата
			//key_state = 3;
		}
		else
		{
			key_state = 0;
			buttonstatus=4;// кнопка отпущена
		}break;
 		case 3:
 		if (key_code == _key_code)
 		{
 			if(btn_pause_state == 2)
 			{
 				counter_lable =3;
 				SetTimerTask(button_pause_time_counter,REPEAT_DELAY);
 				buttonstatus = 2;// кнопка удерживается
 				key_state =4;
 			}
 			else
 			{
 				key_state = 0;
 			}
 		}break;
 		case 4:
 		if(key_code == _key_code)
 		{
 			if(btn_pause_state == 3)
 			{
 				buttonstatus = 3;// автоповтор
 				counter_lable=4;
 				SetTimerTask(button_pause_time_counter,REPEAT_DELAY);
 			}
 			else key_state = 0;
 		}
 		
 		
 		break;
	}
	
}

void Read_on_off_btn()
{
	
	{
	if (!(PINB & _BV(On_OffPin)))
		{
			if(butcount<3) {butcount++; PORTB |= _BV(OnPin);}
			else {PORTD |=_BV(ZummerPin); PORTB &=~ _BV(OnPin); }//Çâóêîâàÿ èíäèêàöèÿ ïðè âûêëþ÷åíèè
		}

 	}
	SetTimerTask(Read_on_off_btn,300);
}
void Read_keys()
{
	 kbd_drv();
	
	if (buttonstatus == 1)
	{
		switch (key_code)
		{

			case 158:
			{
	           Zummer_on();
			   setled();
			   ptemp = pcf_result;	
			   key_code = 0;
			}break;
			case 174:
			{
				Zummer_on();
				off_led();
				ptemp = pcf_result;	
				key_code = 0;
			}break;
			case 206:
			{
				Zummer_on();
				ptemp = pcf_result;	
				key_code = 0;
			}break;
		}		
 	}	
	
	SetTimerTask(Read_keys,300);
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
