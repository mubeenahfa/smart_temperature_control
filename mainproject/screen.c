#include "TM4C123GH6PM.h"
const unsigned char H[] = {0x00,0x7e, 0x7e, 0x18, 0x18, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x6c, 0x6c};
const unsigned char L[] = {0x00, 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x00, 0x00, 0x6c, 0x6c};
const unsigned char T[] = {0x00,0x06, 0x06, 0x06, 0x7e, 0x7e, 0x06, 0x06, 0x06, 0x00, 0x00, 0x6c, 0x6c};
const unsigned char num_patterns[10][7] = {
    {0x7C, 0x82, 0x82, 0x82, 0x7C, 0x00, 0x00}, // num_0
    {0x88, 0x84, 0xFE, 0x80, 0x80, 0x00, 0x00}, // num_1
    {0x84, 0xC2, 0xA2, 0x92, 0x8C, 0x00, 0x00}, // num_2
    {0x44, 0x82, 0x92, 0x92, 0x6C, 0x00, 0x00}, // num_3
    {0x20, 0x30, 0x28, 0x24, 0xFE, 0x00, 0x00}, // num_4
    {0x8E, 0x8A, 0x8A, 0x8A, 0x72, 0x00, 0x00}, // num_5
    {0x7C, 0xA4, 0x92, 0x92, 0x64, 0x00, 0x00}, // num_6
    {0x82, 0x42, 0x22, 0x12, 0x0E, 0x00, 0x00}, // num_7
    {0x44, 0xAA, 0x92, 0xAA, 0x44, 0x00, 0x00}, // num_8
    {0x0C, 0x12, 0x12, 0x12, 0x0A, 0xFC, 0x00}  // num_9
};
const unsigned char tick[] = {0x00, 0x00, 0x18, 0x30, 0x60, 0x60, 0x20, 0x30, 0x18, 0x08, 0x0e, 0x06};

void spi_init(){

SYSCTL->RCGCGPIO  |= 0x00000008 ; // Enable the clock for port D
SYSCTL->RCGCGPIO  |= (1<<4);
	
while((SYSCTL->PRGPIO & 0x08) != 0x08); //Waiting for port D ready

SYSCTL->RCGCSSI   |= 0x00000008 ; // Enable the clock for SSI module 3
while((SYSCTL->PRSSI & 0x08) != 0x08); //Waiting for SSI module 3 ready
GPIOD->AFSEL |= 0x0F;
GPIOD ->PCTL |= 0x01111;
GPIOD -> DEN |= 0xF;
GPIOD->AFSEL &= ~(1<<6);
GPIOD -> DEN |= (1<<6);
GPIOD -> DIR |= (1<<6);

GPIOE->AFSEL &= ~(1<<1);
GPIOE -> DEN |= (1<<1);
GPIOE -> DIR |= (1<<1);

SSI3 -> CR1 &= ~(1<<1); //SSI DISABLED
SSI3 -> CR1 &= ~(0xFFFFFFFF);
SSI3 -> CC &= ~(0x0F);
SSI3 -> CPSR |= 0x08;
SSI3 -> CR0 |= 0x07;
SSI3 -> CR1 |= (1<<1); //ENABLE Ssi
}	
void spi_sendByte(unsigned char data){
SSI3->DR = data;
GPIOD -> DATA &= ~(1<<1);
while ( (SSI3->SR & 0x10)	!= 0);
GPIOD -> DATA |= (1<<1);
}
 void setdisplay(){
GPIOE->DATA &= ~(1<<1);	
for (int i = 0;i<100000;i++);
GPIOE->DATA |= (1<<1);

GPIOD->DATA &= ~(1<<6);
spi_sendByte(0x21);
spi_sendByte(0xC0);
spi_sendByte(0x04);
spi_sendByte(0x13);
spi_sendByte(0x20);
spi_sendByte(0x0C);
spi_sendByte(0x80);
spi_sendByte(0x40);
	
GPIOD->DATA |= (1<<6);
	
 __ASM("NOP");
 __ASM("NOP");
 __ASM("NOP");
}
void reset(){
GPIOE->DATA &= ~(1<<1);	
for (int i = 0;i<100000;i++);
GPIOE->DATA |= (1<<1);
}
void disp_inverse(){

GPIOD->DATA &= ~(1<<6);
spi_sendByte(0x0D);
	
GPIOD->DATA |= (1<<6);
}
void set_cursor(int x, int y){
GPIOD->DATA &= ~(1<<6);
//spi_sendByte(0x20);
spi_sendByte(0x80 | x);
spi_sendByte(0x40 | y);
GPIOD->DATA |= (1<<6);

}
void disp_tick(int row){
	set_cursor(30,row);
	for (int i = 0; i < 12; i++){
    spi_sendByte(tick[i]);
  }
}
void tick_del(int row){
set_cursor(30,row);
	for (int i = 0; i < 12; i++){
    spi_sendByte(0x00);
  }

}

void disp_letter(){
	for (int i =0; i<12;i++){
		spi_sendByte(H[i]);
	}
	set_cursor(0,1);
	for (i =0; i<10;i++){
		spi_sendByte(L[i]);
		
	}
	set_cursor(0,2);
	for (i =0; i<13;i++){
		spi_sendByte(T[i]);
		
	}
	
}
void disp_numbers(int num, int row){
set_cursor(15,row);
int tens = num / 10;
int ones = num % 10;

 for (int i = 0; i < 7; i++){
    spi_sendByte(num_patterns[tens][i]);
  }
spi_sendByte(0x00);
	for (i = 0; i < 7; i++){
    spi_sendByte(num_patterns[ones][i]);
  }
}

void screen_blank(){
set_cursor(0,0);

		for (int i = 0; i < 504; i++){ 
		
		spi_sendByte(0x00);
		
		}

}
/*
int main(){

spi_init();
reset();
setdisplay();
disp_letter(); 
disp_numbers( 20, 0);
disp_numbers(21, 2);
disp_numbers(22,0);
//disp_inverse();
disp_tick(1);
tick_del(1);
	
while(1){



}


}
*/