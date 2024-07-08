#include "TM4C123GH6PM.h"


void key_init(){
		SYSCTL->RCGCGPIO |= 0x2;
		while((SYSCTL->PRGPIO & 0x2)!= 0x2); //Waiting for port B ready
 	  GPIOB->DIR &= ~(0xE8); // PB0 input
		GPIOB->DIR |= 0x7;
		GPIOB->AFSEL &= ~(0xEF); // Disable alternate function
		GPIOB->DEN |= 0xEF; // PB0 enable digital
		GPIOB->AMSEL &= ~(0xEF); // PB0 disable analog	
		GPIOB->PUR |= 0xE8;
}
const unsigned char cols[] = {0x6, 0x5, 0x3};

int check_press(){
int retval;
	
retval = (GPIOB->DATA & 0xE8);
if (retval == 0xE0){
return 0;
}
else if (retval == 0xC8){
return 1;
}
else if (retval == 0xA8){
return 2;
}
else if (retval == 0x68){
return 3;
}
else{
return 10;
}	
}
void debounce_delayy(){
for (int i = 0; i < 1000; i++);
}
int get_key(){
	int retval;
for (int i = 0; i < 3; i++){
GPIOB->DATA = cols[i];
retval = check_press();
if (retval != 10){
retval += 4*i;
return retval;
}

}
return 10;
}
int get_keyval(){
int retvalue;
	int debval;
	retvalue = get_key();
	debounce_delayy();
	debval = get_key();
	if (debval == retvalue){
		while ( get_key() != 10);
		return retvalue;
	}
	else{
	return 10;
	
	}

}

/*
int main(){
	key_init();
	int keyval;
while(1){

	keyval = get_keyval(); 

	
}


}
*/