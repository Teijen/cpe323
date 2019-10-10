/*------------------------------------------------------------------------------
* File:   lab3_assignment1_main.c
* Description:  This C program blinks LEDs with switch input
* Input:    SW1 , SW2
* Output:   LED Blinks
* Author:   Logan Cannan
* Lab Section:  01
* Date: Sep 11, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#define SW1 P1IN&BIT0
#define SW2 P1IN&BIT1
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P2DIR |= BIT1; //set LEDs on
	P2DIR |= BIT2;
	P2OUT |= BIT1;
	P2OUT |= BIT2;
	long int i;
	for(;;){
	    if (((SW1) == 0) & ((SW2) != 0)){ //if 1 is pressed but 2 is not
	        while(((SW1) == 0) & ((SW2) != 0)){
	            //P2OUT &= ~BIT2;
                for( i = 0; i < 10000; i++){
                    P2OUT &= ~BIT2;  //sets LED1 off
                }
                for( i = 0; i < 10000; i++){
                    P2OUT |= BIT2; //sets LED1 on
                }
	        }
	    }
        else if (((SW1) != 0) & ((SW2) == 0)){ // if 2 is pressed and 1 is not
                    while(((SW1) != 0) & ((SW2) == 0)){
                        for( i = 0; i < 25000; i++){
                            P2OUT &= ~BIT1; //sets LED2 off
                        }
                        for( i = 0; i < 25000; i++){
                            P2OUT |= BIT1; ////sets LED2 on
                        }
                    }
        }
        else if (((SW1) == 0) & ((SW2) == 0)){ // if both are pressed
                    while(((SW1) == 0) & ((SW2) == 0)){
                        for( i = 0; i < 50000; i++){
                            P2OUT &= ~BIT2; //LEDs off
                            P2OUT &= ~BIT1;
                        }
                        for( i = 0; i < 50000; i++){
                            P2OUT |= BIT2; //LEDs on
                            P2OUT |= BIT1;
                        }
                    }
	    }
	 }
	return 0;
}
