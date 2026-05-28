/*
 * Buttons.c
 *
 * Created: 09.05.2025 8:26:45
 *  Author: admin
 */ 
#include "Buttons.h"





void Read_keys()
{
	
	{
	if (!(PINB & _BV(On_OffPin)))
		{
			if(butcount<3) {butcount++; PORTB |= _BV(OnPin);}
			else {PORTD |=_BV(ZummerPin); PORTB &=~ _BV(OnPin); }//Звуковая индикация при выключении
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