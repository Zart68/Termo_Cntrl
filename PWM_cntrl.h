/*
 * PWM_cntrl.h
 *
 * Created: 30.01.2026 6:42:26
 *  Author: admin
 */ 


#ifndef PWM_CNTRL_H_
#define PWM_CNTRL_H_

#include "main.h"

void PWM_cntrl_valve();
void Valve_On();
void Valve_Off();
void Prst_pump_start();
void Prst_pump_stop();
void timer1_start();
void prst_pump_on();
void prst_pump_off();
void timer1_stop();//отключение перестальтики



#endif /* PWM_CNTRL_H_ */