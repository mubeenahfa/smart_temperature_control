#ifndef SENSOR_H
#define SENSOR_H

#include "TM4C123GH6PM.h"  // Include the same MCU specific header

// Function declarations
 void I2CINIT(void);
 int wtstat(void);
void t_data(int slave_address, char slave_memory_address, int bytes_count, char* data);
void r_data(int slave_address, char slave_memory_address, int bytes_count, char* data);
void sensor_init(void);
int get_temp(void);
int realinator(double adc_t);

// Global variables declarations
extern char write_data[100];
extern char read_data[2];
extern int average[128];
extern int average1[128];
extern long finalmsb;
extern long finallsb;
extern int sensor_value_notreal;
extern int sensor_value;

#endif // SENSOR_H
