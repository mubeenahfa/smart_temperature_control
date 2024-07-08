SYSCTL_RCGCGPIO 	EQU		0x400FE608
SYSCTL_RCGCI2C	 	EQU		0x400FE620

GPIOA_DATA			EQU		0x40004000
GPIOA_DEN			EQU		0x4000451C
GPIOA_AFSEL			EQU		0x40004420
GPIOA_PCTL			EQU		0x4000452C
GPIOA_ODR			EQU		0x4000450C
GPIOA_PUR			EQU		0x40004510

I2C1_MSA			EQU		0x40021000
I2C1_MCR			EQU		0x40021020
I2C1_MTPR			EQU		0x4002100C
I2C1_MDR			EQU		0x40021008
I2C1_MCS			EQU		0x40021004	; I2C Master Control / Status Register
I2C1_MRIS			EQU		0x40021014	; I2C Master Raw Interrupt Status Register
	
;LABEL		DIRECTIVE	VALUE	COMMENT
			AREA    	transmit, READONLY, CODE
			THUMB
			IMPORT		wtstat
			EXPORT  	t_data
			
t_data
			PUSH	{LR}
			
			;check if data number is 0
			CMP		R2, #0
			BLE		ERROR_EXIT	;branch if number of data to be transmitted is 0
			
			;Slave address should be sent
			LSL		R0, R0, #1
			LDR		R4, =I2C1_MSA
			STR		R0,	[R4]
			
			;Memory address should be sent
			LDR		R4, =I2C1_MDR
			STR		R1,	[R4]
			
			;Write 3 to Control Register
			LDR		R4, =I2C1_MCS
			MOV		R0, #0x03
			STR		R0,	[R4]
			
			; Read Status register and wait until I2C Master is not busy
			BL		wtstat
			CBNZ 	R1, ERROR_EXIT     		; Check for error and exit if an error occurred
			
mulbyte		
			;Write data to data register
			LDRB	R0, [R3], #1
			LDR		R4, =I2C1_MDR
			STR		R0, [R4]
			
			;Write 1 to Control register
			MOVS	R0, #1
			LDR		R4, =I2C1_MCS
			STR		R0, [R4]
			
			; Read Status register and wait until I2C Master is not busy
			BL		wtstat
			CBNZ 	R1, ERROR_EXIT     		; Check for error and exit if an error occurred
			
			;Decrement byte count and check if there are more bytes to send
			SUBS	R2, R2, #1
			CMP		R2, #0
			BGT		mulbyte
			
			;Send last byte and a STOP
			;LDRB	R0, [R3]
			;LDR		R4, =I2C1_MDR
			;STR		R0, [R4]
			MOVS	R0, #4
			LDR		R4, =I2C1_MCS
			STR		R0, [R4]
			
			; Read Status register and wait until I2C Master is not busy
			BL		wtstat
			CBNZ 	R1, ERROR_EXIT     		; Check for error and exit if an error occurred
			
wait_bus	LDR		R4, =I2C1_MCS
			LDR		R0, [R4]
			AND		R0, R0, #0x40
			CMP		R0, #0x40
			BEQ		wait_bus
			
			MOVS	R0, #0
			POP     {LR}
			BX		LR
			
ERROR_EXIT	;Load R0 with -1 to indicate no data
			MOVS	R0, #-1
			POP		{LR}
			BX		LR
			
			ALIGN
			END