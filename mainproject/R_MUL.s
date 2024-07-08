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
			AREA    	receive, READONLY, CODE
			THUMB
			IMPORT		wtstat
			EXPORT  	r_data
			
r_data
			PUSH	{LR}
			
			;check if data number is 0
			CMP		R2, #0
			BLE		ERROR_EXIT	;branch if number of data to be transmitted is 0
			
			MOV		R8, R0
			
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
			
			;Change bus from write to read
			LSL		R8, R8, #1
			ADD		R8, R8, #1
			LDR		R4, =I2C1_MSA
			STR		R8,	[R4]
			
			CMP		R2, #1
			BEQ		mcs_7	;if it is the last byte, dont acknowledge
			BNE		mcs_b	;acknowledge, start and run slave
			
			; Read Status register and wait until I2C Master is not busy
label		BL		wtstat
			CBNZ 	R1, ERROR_EXIT     		; Check for error and exit if an error occurred
			
			LDR		R4, =I2C1_MDR
			LDR		R5, [R4]
			STR		R5, [R3]
			ADD		R3, R3, #1
			
			SUBS	R2, R2, #1
			CMP		R2, #0
			BEQ		wait_bus
			
mulread		
			;Write 9 to Control register
			MOVS	R0, #9
			LDR		R4, =I2C1_MCS
			STR		R0, [R4]
			
			; Read Status register and wait until I2C Master is not busy
			BL		wtstat
			CBNZ 	R1, ERROR_EXIT     		; Check for error and exit if an error occurred
			
			SUBS	R2, R2, #1
			
			LDR		R4, =I2C1_MDR
			LDR		R5, [R4]
			STR		R5, [R3]
			ADD		R3, R3, #1
			
			CMP		R2, #0
			BGT		mulread
			
			;Write 5 to Control register
			MOVS	R0, #5
			LDR		R4, =I2C1_MCS
			STR		R0, [R4]
			
			; Read Status register and wait until I2C Master is not busy
			BL		wtstat
			
			LDR		R4, =I2C1_MDR
			LDR		R5, [R4]
			STR		R5, [R3]
			
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
			
mcs_7
			LDR		R4, =I2C1_MCS
			MOV		R0, #0x03
			STR		R0,	[R4]
			B		label
			
mcs_b
			LDR		R4, =I2C1_MCS
			MOV		R0, #0xB
			STR		R0,	[R4]
			B		label
			
			ALIGN
			END