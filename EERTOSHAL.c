
#include "EERTOSHAL.h"

//RTOS Запуск системного таймера
inline void RunRTOS (void)
{
   TCNT0 = 0;// Установить начальное значение счётчиков	
   TIMSK0 = _BV(TOIE0);//переполнеие таймера
   TCCR0B = _BV(CS00)|_BV(CS01); //	req = CK/64, Normal mode, переполнеие таймера каждую 1ms		    
  
   Enable_Interrupt;
}
