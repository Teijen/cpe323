/*------------------------------------------------------------------------------
* File:   lab1_assignment1_main.c
* Description:  This C program calculates the power for two integers
                and print out the result
* Input:    None
* Output:   None
* Author:   Logan Cannan
* Lab Section:  01
* Date: Aug 16, 2017


*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
long int myPower(int a, int p);//function declaration
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	int a = 15; //declare imputs from assignment pdf
	int p = 5;
	long int b = myPower(a,p); //function call
	printf("%d raised to the power %d is %ld",a,p,b); //output for console
	return 0;
}

long int myPower(int a , int p){ //function definition
    long int  sum = 0; //within scope output variable
    int i;//counter variable
    if (p == 0) //check for power raised to zero
        return 1;
    for (i = 1; i < p; i++) {//does the multiplication required for inputs
            if (sum == 0)
                    sum = a;
            sum = sum * a;
        };
    return sum;//output for function
}
