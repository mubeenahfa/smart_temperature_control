#include "TM4C123GH6PM.h"

void hotcold_init(void)
{
		SYSCTL->RCGCGPIO |= 0x01;
		while((SYSCTL->PRGPIO & 0x01) != 0x01); //Waiting for port A ready
		GPIOA->DIR |= 0x0C; // PA2 & PA3 output
		GPIOA->AFSEL &= ~(0x0C); // Disable alternate function
		GPIOA->DEN |= 0x0C; // PA2 enable digital
		GPIOA->AMSEL &= 0x0C; // PA2 disable analog
	  GPIOA->DR8R |= 0x0C;
}

void peltier_on(){
 GPIOA->DATA |= (1<<2);
	
}
void peltier_off(){
 GPIOA->DATA &= ~(1<<2);

}
void heater_on(){
 GPIOA->DATA |= (1<<3);
	
}
void heater_off(){
 GPIOA->DATA &= ~(1<<3);

}
/*this is for debugging and testing each module code*/	
/*
int main()
{
		
	
		hotcold_init();
		while(1)
		{
		peltier_on();
		peltier_off();
		heater_on();
		heater_off();
			
		}
}
*/
