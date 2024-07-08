#include "TM4C123GH6PM.h" 
#include "mainfile.h"
#include "led_surucu.h"
#include "button.h"
#include "screen.h"
#include "hot_cold.h"
#include "pot_handler.h"
// some global variables that will be used to control the flow of the control system
int buttoncount =0;
int highval;
int lowval;


int keyvalue;
int temp_reading;
int button_val;
int pot_val;
typedef enum {
    in_high,
    in_low,
    work
} state;
// three states for the finite state like control
int main(){
sensor_init(); // initialisations for all components
initpot(); 
btn_init();
pulse_init();
hotcold_init();	
spi_init();
key_init();
setdisplay();
	screen_blank(); // erase screen completely
disp_letter(); 
highval = 30;
lowval = 20;
state mystate = work;
while(1){
	
if (mystate == work){
temp_reading = get_temp();
led_handler(temp_reading, highval, lowval);
//write display
disp_numbers( highval, 0);
disp_numbers(lowval, 1);
disp_numbers(temp_reading,2);
tick_del(0);
tick_del(1);
disp_tick(2);



	
 if (temp_reading <= lowval){
 peltier_off();
heater_on();
 }
 else if (temp_reading >= highval){
 peltier_on();
heater_off();
 
 }
else{
peltier_off();
heater_off();
}
button_val = get_button();
if (button_val != 0x00){
mystate = in_high;
}

}
else if (mystate == in_high){
temp_reading = get_temp();
	highval = get_potval();
disp_numbers( highval, 0);
disp_numbers(lowval, 1);
disp_numbers(temp_reading,2);
tick_del(2);
tick_del(1);
disp_tick(0);
button_val = get_button();
if ( (button_val != 0x00) && (highval > lowval)){
	mystate = in_low;
}

}
else if (mystate == in_low) {
temp_reading = get_temp();
	lowval = get_potval();
disp_numbers( highval, 0);
disp_numbers(lowval, 1);
disp_numbers(temp_reading,2);
tick_del(2);
tick_del(0);
disp_tick(1);
button_val = get_button();
if ( (button_val != 0x00) && (highval > lowval)){
	mystate = work;
}

}	
	
}



}

