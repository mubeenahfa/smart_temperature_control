#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include "TM4C123GH6PM.h"

// Function declarations
void btn_init(void);
void delay_debounce(void);
int get_button(void);

#endif // BUTTON_CONTROL_H
