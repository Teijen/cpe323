/*------------------------------------------------------------------------------
* File:   lab6_bonus_main.c
* Description:  This C program blinks LEDs using switches
* Input:    SW1 , SW2
* Output:   LED Blinks
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 9, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#define   SW1 BIT0&P1IN             // SW1 is P1IN&BIT0
#define   SW2 BIT1&P1IN             // SW1 is P1IN&BIT1

int flag;
int flag = 0;
int hold;
int hold = 0;
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P2DIR |= BIT2 + BIT1;                  // Set LED1 as output
	P2OUT &= ~(BIT2 + BIT1);
	_EINT();                        // enable interrupts
	P1IE |= BIT0+BIT1;                   // P1.0 interrupt enabled
	P1IES |= BIT0+BIT1;                  // P1.0 hi/low edge
	P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared
	int i;
	unsigned long int j;
	while(1){
	        if((flag == 1))
	        {
	            for(i = 0; i<6;i++ ){
	                for (j = 0; j < 50000; j++)
	                {

	                }
	                P2OUT ^= BIT2;
	            }
	            P2OUT ^= BIT1;
	            flag = 0;
	        }

	}
	return 0;
}
#pragma vector = PORT1_VECTOR
 __interrupt void Port1_ISR (void)
 {
     P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared
     unsigned long int j;

     if (((SW1) == 0) & ((SW2) == 0)){ // if both are pressed
                                for( j = 0; j < 2000; j++);

                                if (((SW1) == 0) & ((SW2) == 0)){
                                   while(((SW1) == 0) & ((SW2) == 0)){
                                       P2OUT |= BIT1 + BIT2;
                                   }
                                   P2OUT &= ~(BIT1 + BIT2);
                                   for( j = 0; j < 160000; j++);


                                }
                                }


      else  if ((SW1) == 0)
                    {
                        for (j = 0; j < 4000; j++);    // Debounce ~20 ms
                        if ((SW1) == 0)
                        {
                           flag = 1;
                           hold = 0;
                        }
                    }
      else if ((SW2) == 0)
                     {
                         for (j = 0; j < 4000; j++);    // Debounce ~20 ms
                         if ((SW2) == 0)
                         {
                             P2OUT ^= BIT1;
                             hold = 0;
                         }
                     }
     return;
 }

