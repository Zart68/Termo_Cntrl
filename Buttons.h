/*
 * Buttons.h
 *
 * Created: 09.05.2025 8:27:06
 *  Author: admin
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "main.h"

#define DEBOUNCE_DELAY 10//10mS
#define HOLD_DELAY  10//10mS
#define REPEAT_DELAY 100//10000mS//1Sec
#define BLINK_DELAY 1//10mS
#define SWITCH_DELAY 10//20mS

void Read_keys();
void Zummer_on();
void Read_on_off_btn();

#endif /* BUTTONS_H_ */
