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
		EXPORT  	wtstat
				
wtstat  
		LDR 	R1, =I2C1_MCS
		LDRB 	R0, [R1]    ; Load the value of I2C3_MCS into R0
		TST 	R0, #1            ; Test the busy bit (bit 0)
		BNE 	wtstat     ; Branch back if the I2C master is still busy
			
		LDR 	R0, =I2C1_MCS ; Return the I2C error code (0 if no error)
		LDR		R1, [R0]
		AND 	R1, R1, #0xE
		
		BX		LR
		
		ALIGN
		END