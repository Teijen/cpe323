;-------------------------------------------------------------------------------
; File:   lab4_bonus_main.asm
;Description:  This ASM program calculates the number of mathematical operations in a string
; Input:    None
; Output:   None
; Author:   Logan Cannan
; Lab Section:  01
; Date: Sep 17, 2019
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
            .data
myStr:		.string		"I enjoy learning msp430",''
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
main:		bis.b	#0FFh,&P2DIR			;configure P1.x output
			mov.w	#myStr,R4				;load the starting address of the string into R4
nextchr:	mov.b	@R4+,R6
			cmp		#0,R6
			jeq		lend
			cmp.b 	#7Bh,R6
			jge		nextchr
			cmp.b	#5Bh,R6 	;if greater than 5A then it is lowercase in given string
			jge		cap
			jmp		nextchr

cap:		sub.b	#0x20,R6 ;uppercase letters have a value of 20 in hex lower than uppercase
			mov.b	R6,-1(R4)
			jmp		nextchr

lend:		mov.b	R4,&P2OUT
			bis.w	#LPM4,SR
			nop
                                            

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
            
