;-------------------------------------------------------------------------------
; File:   lab4_assignment1_main.asm
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
myStr:		.string		"Do 42+53/76%8=2*8-32+71 & you can sleep",''
count:		.int	25
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT:     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
			mov.w	#myStr,R4				;load the starting address of the string into R4
			clr.b	R5						;set R
nextchr:	mov.b	@R4+,R6					;move current character into R6 and increment
			cmp		#0,R6					;jump if end character detected
			jeq		lend
			cmp.b	#37,R6					;comparison to lower special character range
			jl		nextchr
			cmp.b	#126,R6					;comparison for ~
			jeq		addcount
			cmp.b	#61,R6					;comparison for above special character
			jeq		addcount
			cmp.b	#48,R6					;comparison for above special character range
			jge		nextchr
			jmp		addcount

addcount:	inc.w	R5						;adds to count variable
			jmp		nextchr

lend:		mov		R5,count				;moves value into count variable
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
            .end
