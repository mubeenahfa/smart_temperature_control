#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

#include "TM4C123GH6PM.h"



// Function declarations
void spi_init(void);
void spi_sendByte(unsigned char data);
void setdisplay(void);
void reset(void);
void disp_inverse(void);
void set_cursor(int x, int y);
void disp_tick(int row);
void tick_del(int row);
void disp_letter(void);
void disp_numbers(int num, int row);
void screen_blank(void);


#endif // DISPLAY_CONTROL_H
