;-------------------------------------------------------------------------------
; File:   lab4_assignment2_main.asm
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
            .data                               ; make it known to linker.
myStr:		.string		"-4-3+5",''
sum:		.int	0						;sum variable in mem
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
			clr.b	R5						;set sum register
nextchr:	mov.b	@R4+,R6					;get new char and test for end
			cmp		#0,R6
			jeq		lend
			cmp.b	#43,R6 					;if add go to next char
			jeq		nextchr
			cmp.b	#45,R6					;if sub jump to sub
			jeq		sub
			sub.b	#0x30,R6				;change character value to int value
			add		R6,R5					;add result to sum
			jmp		nextchr

sub:		mov.b	@R4+,R6					;when neg sign detected get next value
			sub.b	#0x30,R6				;get next value to int
			sub		R6,R5					;sub int from sum
			jmp		nextchr

lend:		mov		R5,sum					;store value into sum variable
			mov.b	R5,&P2OUT
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
            
