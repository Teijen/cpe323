;-------------------------------------------------------------------------------
;File:   HW_product.asm
;Description:  This ASM subroutine multiplies two arrays elementwise using hardware
;Input:     The input parameters are on the stack pushed as follows:
;                 starting address of the array 1
;                 array length
;                 starting address of the array 2
;				  starting address of resultant array
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
            .def    hw                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
hw:
			push	R4	;put those on stack
			push	R5
			push	R6
			push	R7
			mov.w	12(SP),R6				;get array length
			mov.w 	14(SP),R4				;get address for base
			mov.w	10(SP),R7				;get base address of res1
			mov.w	R4,0(R7)
			add		#2,R7
			dec.w	R6
nextnum:	mov.w	-2(R7),&MPYS				;get array ind 1 into mult
			mov.w	R4,&OP2
			mov.w	RESLO,0(R7)
			add		#2,R7					;get result into spot
			dec.w	R6
			cmp.w	#0,R6					;index
			jeq		done
			jmp		nextnum

done:		pop		R7						;restores registers and stack
			pop		R6
			pop		R5
			pop		R4
			ret
			.end



