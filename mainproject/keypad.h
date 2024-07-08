#ifndef KEY_CONTROL_H
#define KEY_CONTROL_H

#include "TM4C123GH6PM.h"

// Function declarations

void key_init(void); // this will be used for initialisation purpose 
int check_press(void);
void debounce_delayy(void);
int get_key(void);
int get_keyval(void); //this will be used to get a value upon key release

#endif // KEY_CONTROL_H
