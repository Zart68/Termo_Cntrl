/*
 * PWM_cntrl.c
 *
 * Created: 30.01.2026 6:42:00
 *  Author: admin
 */ 
#include "PWM_cntrl.h"



void PWM_cntrl_valve()
{
	SetTimerTask(Valve_On,1000);
	SetTimerTask(Valve_Off,1200);
	SetTask(PWM_cntrl_valve);
}

void Valve_On()
{
	//PORTC |=_BV(Valve_pin);//клапан открыт
}

void Valve_Off()
{
	//PORTC &=~ _BV(Valve_pin);//клапан закрыт
}
void Prst_pump_start()
{
	PORTB |= _BV(PrstPumpPin);
	SetTimerTask(Prst_pump_stop,1500);
}
void Prst_pump_stop()
{
	PORTB &=~ _BV(PrstPumpPin);
	SetTimerTask(Prst_pump_start,500);
}
void Prst_pump_change()
{
	
}
  void timer1_start()
  {  
   	 TCNT1 = 0;	// Установить начальное значение счётчиков
 	 TCCR1A |= _BV(WGM11);
  	 TCCR1B |= _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  	 ICR1 = 0x31E;//798 dec 20 kHz
  	 TIMSK1 |= _BV(OCIE1B);
  	 OCR1B = period; 
	 DDRB |= _BV(PrstPumpPin);	  
  }
ISR(TIMER1_COMPB_vect)
{
	PORTB |= _BV(PrstPumpPin);
	TIMSK1 &=~ _BV(OCIE1B);
	//SetTimerTask(prst_pump_off,20/*0*/);
	
	prst_pump_off();//prst_pump_off();
}
// ISR(TIMER1_COMPA_vect)
// {
//
//  	if(!(PORTB&_BV(PrstPumpPin))) PORTB |=_BV(PrstPumpPin);
//  	else PORTB &=~ _BV(PrstPumpPin);
// 	TIMSK1 &=~ _BV(OCIE1A);
// }
void prst_pump_on()
{		
	PORTB |=_BV(PrstPumpPin);
	if(prst_work>=10)SetTimerTask(prst_pump_off,prst_work);//10 min, 1000 max,  500 mid	
}
void prst_pump_off()
{
	PORTB &=~ _BV(PrstPumpPin);
	if(prst_pause<=1000)SetTimerTask(prst_pump_on,prst_pause);//1000 min, 10 max, 500 mid	
}
void timer1_stop()//отключение перестальтики
{
	TCCR1B = TCCR1A = TCNT1 = 0;
	
	DDRB &=~ _BV(PrstPumpPin);
}

