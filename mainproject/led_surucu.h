#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "TM4C123GH6PM.h"



// Function declarations
void TIMER0A_Handler(void);
int scaledval(int reading, int h, int l);
void pulse_init(void);
void delay_led(void);
void setpwm(int val);
void led_handler(int currtemp, int high, int low);

#endif // LED_CONTROL_H
