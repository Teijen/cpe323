/*------------------------------------------------------------------------------
* File:   lab2_bonus_main.c
* Description:  This C program calculates matrix multiplication
* Input:    None
* Output:   None
* Author:   Logan Cannan
* Lab Section:  01
* Date: Sep 04, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	int mat1[8][8] = { //declare first matrix
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1},
	                 {1,1,1,1,1,1,1,1}
	                 };

	int mat2[8][8] = { // declare second matrix
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2},
	                 {2,2,2,2,2,2,2,2}
	                 };
	int mat3[8][8]; //initialize all count var, result matrix, and dot product var
	int count1;
	int count2;
	int count3;
	int dotPro;

	for (count1 = 0; count1 < 8; count1++) { //each loop goes through the array to perform dot
	                                         //product, multiplication, and assignment into third array
	      for (count2 = 0; count2 < 8; count2++) {
	        for (count3 = 0; count3 < 8; count3++) {
	          dotPro = dotPro + mat1[count1][count3] * mat2[count3][count2];
	        };

	        mat3[count1][count2] = dotPro;
	        dotPro = 0;
	      };
	};

	printf("Input1:\n"); //prints
	for (count1 = 0; count1 < 8; count1++) { //goes through each row and col to print result
	            for (count2 = 0; count2 < 8; count2++){
	              printf("%d\t", mat1[count1][count2]);


	            };
	                       printf("\n");
	                     };
	printf("Input2:\n"); //prints
	            for (count1 = 0; count1 < 8; count1++) { //goes through each row and col to print result
	                for (count2 = 0; count2 < 8; count2++){
	                    printf("%d\t", mat2[count1][count2]);


	                };
	                           printf("\n");
	                         };

	 printf("Result:\n"); //prints result

	      for (count1 = 0; count1 < 8; count1++) { //goes through each row and col to print result
	        for (count2 = 0; count2 < 8; count2++){
	          printf("%d\t", mat3[count1][count2]);


	          };
	        printf("\n");
	      };
	return 0;
}
