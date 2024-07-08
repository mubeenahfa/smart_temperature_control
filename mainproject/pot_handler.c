#include "TM4C123GH6PM.h"

#define SAMPLE_MASK 0xFFF


int scaledval_pot(int reading){
	int s_integer = (reading*30)/4095; //this is to scale the reading between -165 to 165
	s_integer += 10;
	return s_integer;
}

void initpot(){
/*CLOCK CONFIG*/
SYSCTL->RCGCGPIO |= (1<<1);
SYSCTL->RCGCADC |= 0x01;
	
/*Port configurations*/
GPIOB->DIR &= ~(1<<4);
GPIOB->DEN &= ~(1<<4);
GPIOB->AFSEL |= (1<<4);	
GPIOB->AMSEL |= (1<<4);
	
/*ATD configuration*/	
ADC0->ACTSS &= ~(1<<3);
ADC0->EMUX &= ~(0xF000);
ADC0->SSMUX3 = 0x0A;
ADC0->SSCTL3 |= ((1<<1)|(1<<2));
ADC0->PC = 0x01;
ADC0->ACTSS |= (1<<3);	
}

int get_potval(){
int retval;
	int reading;
	
ADC0->PSSI |= (1<<3);
while ((ADC0->RIS & (1<<3)) == 0){}
reading = ADC0->SSFIFO3 & SAMPLE_MASK;
retval = scaledval_pot(reading);
ADC0->ISC |= (1<<3);
return retval;

}
/*
int main(){
	initpot();
	int readings;
	while(1){
  readings = get_potval();		
	}
}
*/
 