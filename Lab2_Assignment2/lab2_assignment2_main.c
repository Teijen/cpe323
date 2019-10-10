/*------------------------------------------------------------------------------
* File:   lab2_assignment2_main.c
* Description:  This C program manipulates a given array into a new array
* Input:    None
* Output:   None
* Author:   Logan Cannan
* Lab Section:  01
* Date: Sep 03, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	int x[] = {-1,2,5,3,5,6}; //declare required input array
	int m = 5; //set multiply constant
	int c = -2; // set addition constant
	int i; //declare count variable
	int y[sizeof(x)/sizeof(x[0])]; //calculate size for why
	for(i = 0; i < (sizeof(x)/sizeof(x[0]));i++) // loop through augmentation
	{
	    y[i] = x[i] * m + c ;
	}
	printf("Input Array X: ["); //print assigned phrase
	for(i = 0; i < (sizeof(x)/sizeof(x[0]));i++) // print original array
	    {
	        printf("%d ", x[i]);
	    }
	printf("]\n");
	printf("Input Constants m: %d and c:%d\n", m,c); // print augmentation constants
	printf("Output Array Y: [");
	for(i = 0; i < (sizeof(y)/sizeof(y[0]));i++)//print augmented array
	        {
	            printf("%d ", y[i]);
	        }
	printf("]\n");
	return 0;
}
