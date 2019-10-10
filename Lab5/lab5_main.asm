;-------------------------------------------------------------------------------
;File:   lab5_main.asm
;Description:  This ASM program multiplies two arrays elementwise
; Input:    None
; Output:   None
; Author:   Logan Cannan
; Lab Section:  01
; Date: Sep 30, 2019
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
            .ref	hw
            .ref	sw_product              ; make it known to linker.
            .data
res1:		.int	0,0,0,0,0,0,0,0
res2:		.int	0,0,0,0,0,0,0,0
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
main:		push	#arr1				;mov address of array
			push	#8					;mov size into R5
			push	#arr2
			push	#res1
			call	#hw
			pop 	R6					;stores hardware mult result array location
			push	#res2
            call	#sw_product
            pop 	R7

arr1:		.int	1, 2, 3, 4, 1, 2, -3, 4  ; the first array
arr2:		.int	4, -3, 5, 4, 6, -2, 3, 1  ; the second array

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
            
