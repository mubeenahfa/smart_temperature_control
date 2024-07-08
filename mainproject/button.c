#include "TM4C123GH6PM.h"

void btn_init(void)
{
		SYSCTL->RCGCGPIO |= 0x20;
		while((SYSCTL->PRGPIO & 0x20) != 0x20); //Waiting for port F ready
		GPIOF -> LOCK = 0X4C4F434B;
	  GPIOF->CR |= 0x01;
 	  GPIOF->DIR &= ~(0x01); // PF0 input
		GPIOF->AFSEL &= ~(0x01); // Disable alternate function
		GPIOF->DEN |= 0x01; // PF0 enable digital
		GPIOF->AMSEL &= ~(0x01); // PF0 disable analog
	  
}
void delay_debounce(){
for (int i =0; i < 1000;i++){
	;
}
}
int get_button(){
  if ((GPIOF->DATA & (0x01)) == 0){
	 delay_debounce();
   	if ((GPIOF->DATA & (0x01)) == 0){
		while ((GPIOF->DATA & (0x01)) != 1);
		return 1;
		}
    else{
		return 0;
		}			
	
	}
 else{
 return 0;
 }

}

/*
int main()
{
		
	
		int test = 0;
		btn_init();
		while(1)
		{
			test =get_button();
		}
}
*/