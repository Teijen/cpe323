;-------------------------------------------------------------------------------
;File:   lab6_Assignment_main.asm
;Description:  This ASM program manipulates LEDS
; Input:    Switch One and Two
; Output:  LED 1 and 2
; Author:   Logan Cannan
; Lab Section:  01
; Date: Oct 7, 2019
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
Setup:
			bis.b	#BIT1|BIT2, &P2DIR	; Set P2.2 to output ; direction (0000_0100)
			bic.b	#BIT1|BIT2, &P2OUT	; Set P2OUT to 0x0000_0100 ; (ensure LED1/2 is off)
			bis.w	#GIE, SR		; Enable Global Interrupts
			bic.b	#BIT0|BIT1, &P1DIR	; set switch to input
			bis.b	#BIT0|BIT1, &P1IE	; Enable Port 1 interrupt from bit 0
			bis.b	#BIT0|BIT1, &P1IES	; Set interrupt to call from hi to low
			bic.b	#BIT0|BIT1, &P1IFG	; Clear interrupt flag

InfLoop:
			cmp #6,R6
			jeq	loop
			jmp	InfLoop	; Loop here until interrupt

loop:		mov.w	#50000, R15		;Set to (2000 * 10 cc )
SWDms:		dec.w	R15				; Decrement R15
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz		SWDms			; Delay over?
			dec.b	R6
			xor.b	#BIT2,&P2OUT ;toggle led 1
			cmp #0,R6
			jnz	loop
			xor.b	#BIT1,&P2OUT ;toggle led 2
			jmp		InfLoop
;----------------------------------------------------------------------------
; P1_0 (SW1) interrupt service routine (ISR)
;
;----------------------------------------------------------------------------
SW1_ISR:
			bic.b	#BIT1, &P1IFG	; Clear interrupt flag
			bic.b	#BIT0, &P1IFG	; Clear interrupt flag
			;bic.b	#BIT0|BIT1, &P1IES	; Set interrupt to call from hi to low

switch:		bit.b   #BIT0, &P1IN
			jnz		Sw2
			bit.b	#BIT1, &P1IN
			jnz		Sw1


Sw1:		mov.w	#2000, R15		;Set to (2000 * 10 cc )
SWD20ms:	dec.w	R15				; Decrement R15
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz		SWD20ms			; Delay over?
			bit.b	#00000001b,&P1IN; Verify SW1 is still pressed
			jnz		setFlag
			jmp		LExit

Sw2:		mov.w	#2000, R15		;Set to (2000 * 10 cc )
SWD30ms:	dec.w	R15				; Decrement R15
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz		SWD30ms			; Delay over?
			bit.b	#00000010b,&P1IN; Verify SW2 is still pressed
			jnz		LEDtoggle			; If not, wait for SW2 press
			jmp		LExit
setFlag:	mov.b	#6,R6 ;set flag for main loop
			jmp		LExit
LEDtoggle:	xor.b	#BIT1,&P2OUT ;toggle led 2
LExit:		reti	; Return from interrupt

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .sect	".int20"
            .short	SW1_ISR
