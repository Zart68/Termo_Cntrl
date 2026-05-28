#ifndef EERTOSHAL_H
#define EERTOSHAL_H
#include "main.h"

#define STATUS_REG 			SREG
#define Interrupt_Flag		SREG_I
#define Disable_Interrupt	cli();
#define Enable_Interrupt	sei();

//RTOS Config
//#define RTOS_ISR  			TIMER2_COMPA_vect
#define	TaskQueueSize		16
#define MainTimerQueueSize	16

//void timer1_init();
extern void RunRTOS (void);

#endif
