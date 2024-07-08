#ifndef POTENTIOMETER_CONTROL_H
#define POTENTIOMETER_CONTROL_H

#include "TM4C123GH6PM.h"

// Define SAMPLE_MASK if it's not defined elsewhere
#ifndef SAMPLE_MASK
#define SAMPLE_MASK 0xFFF
#endif



// Function declarations
int scaledva_pot(int reading);
void initpot(void);
int get_potval(void);

#endif // POTENTIOMETER_CONTROL_H
