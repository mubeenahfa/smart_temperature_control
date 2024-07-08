#include "TM4C123GH6PM.h"


#define SAMPLE_MASK 0xFFF


int high = 27;
int low = 18;
int state = 0;

void TIMER0A_Handler (void);
int scaledval(int reading, int h, int l){
	int retval  = (((reading-l)*40)/ (h-l));
	return retval;
}
void pulse_init(void){
	volatile int *NVIC_EN0 = (volatile int*) 0xE000E100;
	volatile int *NVIC_PRI4 = (volatile int*) 0xE000E410;
	SYSCTL->RCGCGPIO |= 0x20; // turn on bus clock for GPIOF
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
  GPIOF->DIR			|= 0x08; //set PF3 as output
  GPIOF->AFSEL		&= (0xFFFFFFFB);  // Regular port function
	GPIOF->PCTL			&= 0xFFFF0FFF;  // No alternate function
	GPIOF->AMSEL		&= ~(1<<3); //Disable analog
	GPIOF->DEN			|=0x08; // Enable port digital
	
	GPIOF-> DIR |= ((1<<2) | (1<<1)); //set pf2 as output for blue light and pf1 as red
	GPIOF->DEN	|=((1<<2) | (1<<1)); // Enable port digital
	
	
	SYSCTL->RCGCTIMER	|=0x01; // Start timer0
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	TIMER0->CTL			&=0xFFFFFFFE; //Disable timer during setup
	TIMER0->CFG			=0x04;  //Set 16 bit mode
	TIMER0->TAMR		=0x02; // set to periodic, count down
	TIMER0->TAILR		=low; //Set interval load as LOW
	TIMER0->TAPR		=63; // Divide the clock by 16 to get 1us
	TIMER0->IMR			=0x01; //Enable timeout intrrupt	
	
	//Timer0A is interrupt 19
	//Interrupt 16-19 are handled by NVIC register PRI4
	//Interrupt 19 is controlled by bits 31:29 of PRI4
	*NVIC_PRI4 &=0x00FFFFFF; //Clear interrupt 19 priority
	*NVIC_PRI4 |=0x40000000; //Set interrupt 19 priority to 2
	
	//NVIC has to be neabled
	//Interrupts 0-31 are handled by NVIC register EN0
	//Interrupt 19 is controlled by bit 19
	*NVIC_EN0 |=0x00080000;
	
	//Enable timer
	TIMER0->CTL			 |=0x03; // bit0 to enable and bit 1 to stall on debug
	return;
}


void delay_led(){
for (int i = 0; i <=1600000;i++){
;
}
}



void setpwm(int val){
	high = val;
	low = 45 - val;
}


void led_handler(int currtemp, high, low){
if (currtemp <= low){
  setpwm(0);
	GPIOF->DATA &= ~(1<<2);
	GPIOF->DATA = (1<<1); //TURN RED ON
}
else if (currtemp >= high){
  GPIOF->DATA &= ~(1<<1);
	setpwm(0);
	GPIOF->DATA = (1<<2); //turn blue on
}
else{
GPIOF->DATA &= ~(1<<1);
GPIOF->DATA &= ~(1<<2);
int scval = scaledval(currtemp, high, low);
setpwm(scval);
}
}
/* //this is for component testing purposes DO NOT UNCOMMENT
int main(){

	pulse_init();
	int counter = 0;
	const int h = 30;
	const int low = 15;
	int temper[] = {10,11,16,19,20,22,24,26,28,30,40,50,60};
	
	
	while(1){
	led_handler(temper[counter%12], h, low);
	counter ++;
	delay();
	}
}
*/

void TIMER0A_Handler (void){
	if (state == 0){
	GPIOF->DATA  &= ~0x08;  //toggle PF3 pin
	TIMER0->TAILR = low;
		
		state =1;
	}
	else{
	GPIOF->DATA  |= 0x08;  //toggle PF3 pin
	TIMER0->TAILR	= high;
		state = 0;
	}
	TIMER0->ICR |=0x01; //Clear the interrupt
	return;
}

