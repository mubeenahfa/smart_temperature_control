#ifndef HOTCOLD_CONTROL_H
#define HOTCOLD_CONTROL_H

#include "TM4C123GH6PM.h"

// Function declarations
void hotcold_init(void);
void peltier_on(void);
void peltier_off(void);
void heater_on(void);
void heater_off(void);

#endif // HOTCOLD_CONTROL_H
