/*------------------------------------------------------------------------------
* File:   lab1_assignment2_main.c
* Description:  This C program calculates the number of mathematical operations in a string in C
* Input:    None
* Output:   None
* Author:   Logan Cannan
* Lab Section:  01
* Date: Aug 25, 2017


*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	int i; //counter variable
	char phrase[] = "Do 42+53/76%8=2*8-32+71 & you can sleep";//required phrase from assignment pdf
	int p = sizeof(phrase)/sizeof(phrase[0]);//gets total number of characters in array
	int sum = 0;//output variable
	for (i = 0; i < p; i++) { //loops through array for each index
	            if (phrase[i] == '%'){ //each if checks for acceptable character and if detected changes output variable
	                    sum = sum +1;}
	            else if (phrase[i] == '&'){
	                    sum = sum +1;}
	            else if (phrase[i] == '*'){
	                    sum = sum +1;}
	            else if (phrase[i] == '+'){
	                    sum = sum +1;}
	            else if (phrase[i] == '-'){
	                    sum = sum +1;}
	            else if (phrase[i] == '/'){
	                    sum = sum +1;}
	            else if (phrase[i] == '<'){
	                    sum = sum +1;}
	            else if (phrase[i] == '>'){
	                    sum = sum +1;}
                else if (phrase[i] == '='){
                        sum = sum +1;}
                else if (phrase[i] == '^'){
                        sum = sum +1;}
                else if (phrase[i] == '~'){
                        sum = sum +1;}
                else{
	                   }
	        };
	printf("String: %s\nContains: The string contains %d symbols that represent\nmathematical operations.",phrase,sum); //output for console in correct format
	return 0;
}
