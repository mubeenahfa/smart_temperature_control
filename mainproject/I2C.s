SYSCTL_RCGCGPIO 	EQU		0x400FE608
SYSCTL_RCGCI2C	 	EQU		0x400FE620
SYSCTL_PRGPIO   	EQU 	0x400FEA08
SYSCTL_PRI2C   	    EQU 	0x400FEA20

GPIOA_DATA			EQU		0x40004000
GPIOA_DEN			EQU		0x4000451C
GPIOA_AFSEL			EQU		0x40004420
GPIOA_PCTL			EQU		0x4000452C
GPIOA_ODR			EQU		0x4000450C

I2C1_MSA			EQU		0x40021000
I2C1_MCR			EQU		0x40021020
I2C1_MTPR			EQU		0x4002100C
I2C1_MDR			EQU		0x40021008
I2C1_MCS			EQU		0x40021004	; I2C Master Control / Status Register
I2C1_MRIS			EQU		0x40021014	; I2C Master Raw Interrupt Status Register

;LABEL	DIRECTIVE	VALUE	COMMENT
		AREA    	init, READONLY, CODE
		THUMB
		IMPORT		wtstat
		EXPORT  	I2CINIT
				
I2CINIT
start	LDR		R1, =SYSCTL_RCGCGPIO	;Clock register
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0x01	;turn on clock PORT A
		STR 	R0, [R1]
		
wt_gpio	LDR		R1, =SYSCTL_PRGPIO
		LDR		R0, [R1]
		CMP		R0, #0x01
		BNE		wt_gpio
		
		LDR		R1, =SYSCTL_RCGCI2C		; Start clock for I2C
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0x02	; Enable and provide a clock to I2C module 1 (PA6) in Run mode.
		STR 	R0, [R1]
		
wt_i2c	LDR		R1, =SYSCTL_PRI2C
		LDR		R0, [R1]
		CMP		R0, #0x02
		BNE		wt_i2c
		
		LDR		R1, =GPIOA_DEN		; Enable Digital Operation for PA6 and PA7
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0xC0	; PA7(1) - PA6(1) - 00 - 0000
		STR 	R0, [R1]
		
		LDR		R1, =GPIOA_AFSEL		; Select Alternate Function for PA6 and PA7
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0xC0
		STR 	R0, [R1]
		
		LDR		R1, =GPIOA_PCTL		; Select I2C for PA6 and PA7
		LDR	 	R0, [R1]	
		MOV32   R0, #0x33000000	; 3 is encoded for I2C for PA6 and PA7 (both)
		STR 	R0, [R1]
		
		LDR		R1, =GPIOA_ODR		; Select open drain for PA7 (I2C1SDA)
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0x80
		STR 	R0, [R1]
		
		;LDR		R1, =GPIOA_PUR		; PA6 and PA7 should be pulled up
		;LDR	 	R0, [R1]
		;ORR 	R0, R0, #0xC0
		;STR 	R0, [R1]
		
		LDR		R1, =I2C1_MCR		; Select Master for I2C1
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0x10
		STR 	R0, [R1]
		
		;TPR = (System Clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1 = 7
		LDR		R1, =I2C1_MTPR		; Timer Period Configuration
		LDR	 	R0, [R1]
		ORR 	R0, R0, #0x07
		STR 	R0, [R1]
		;END OF I2C INITIALIZATION
		
		BX		LR
		
		ALIGN
		END