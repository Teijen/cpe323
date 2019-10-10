;-------------------------------------------------------------------------------
;File:   SW_product.asm
;Description:  This ASM subroutine multiplies two arrays elementwise using software
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
            .def    sw_product                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
sw_product:
			push	R4	;put those on stack
			push	R5
			push	R6
			push	R7
			push 	R8
			push	R9
			push 	R10
			push	R11
			mov.w	22(SP),R6				;get array length
			mov.w 	24(SP),R4				;get starting address of arr1
			mov.w	20(SP),R5 				;get starting address of arr2
			mov.w	18(SP),R7				;get base address of res2

nextnum:	mov.w	#8,R11
			mov.w	@R4+,R8				;get array ind 1 into mult
			mov.w	@R5+,R9
			sxt		R8			;set up for rotation
			sxt		R9

checkadd:	bit.w	#1,R9		;check if i need to add a to c
			jnz		add
			jmp		rot

add:		add.w	R8,R10
			jmp		rot

rot:		clrc				;follws rotate commands for mult
			rrc		R9
			clrc
			rlc		R8
			dec.w	R11
			cmp.w	#0,R11
			jeq		subcheck
			jmp		checkadd

subcheck:	bit.w	#1,R9		;check sign for negative num
			jnz		sub
			jmp		ind

sub:		sub.w	R8,R10		;to undo a negative number
			jmp		ind


ind:		mov.w	R10,0(R7)	;store into res2
			mov.w	#0,R10		;reset r10
			add		#2,R7
			dec.w	R6			;decrement for index
			cmp.w	#0,R6
			jeq		done
			jmp		nextnum


done:		pop		R11 ;resets stack and restores registers
			pop		R10
			pop		R9
			pop		R8
			pop		R7
			pop		R6
			pop		R5
			pop		R4
			ret
			.end
