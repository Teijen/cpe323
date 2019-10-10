/*------------------------------------------------------------------------------
* File:   lab6_assignment2_main.c
* Description:  This C program blinks LEDs with switch input multiplying clock
* Input:    SW1 , SW2
* Output:   LED Blinks
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 9, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 

#define   SW1 BIT0&P1IN             // SW1 is P1IN&BIT0
#define   SW2 BIT1&P1IN             // SW1 is P1IN&BIT1


int clock = 1;
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	FLL_CTL0 |= XCAP18PF;          // Set load capacitance for xtal 30 no change
	//P1DIR |= 0x32;                 // P1.1, P1.4 & P1.5 to output direction 33 no change

	SCFI0 |= FN_2;                 // DCO range control 31
	SCFQCTL = 30;                  // (74+1) x 32768 = 2.45MHz 32

	P2DIR |= BIT2 + BIT1;                  // Set LED1 as output
	P2OUT &= BIT2 + BIT1;
	//P2OUT |= ~0x02;
	//P2OUT |= 0x04;                   // clear LED1 status
	P2OUT ^= BIT1;
	_EINT();                        // enable interrupts
	P1IE |= BIT0+BIT1;                   // P1.0 interrupt enabled
	P1IES |= BIT0+BIT1;                  // P1.0 hi/low edge
	P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared


	    unsigned  long int i;

	    while(1)
	    {
	     for(i = 0; i < (50000); i++);
	         {
	                                                     // LEDs is TOGGLE
	         }
	         P2OUT ^= (BIT1 + BIT2);
	    }
}
// Port 1 interrupt service routine
 #pragma vector = PORT1_VECTOR
 __interrupt void Port1_ISR (void)
 {
     P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared
     unsigned int j;
     if ((SW1) == 0)
         {
             for (j = 0; j < 2000; j++);    // Debounce ~20 ms
             if ((SW1) == 0 && clock < 4)
             {
                clock++; //manipulates state flag

             }
         }
     if ((SW2) == 0)
          {
              for (j = 0; j < 2000; j++);    // Debounce ~20 ms
              if ((SW2) == 0 && clock > 1)
              {
                 clock--;
              }
          }

     switch(clock)
     {
         case 1: FLL_CTL0 &= ~DCOPLUS;          // Set load capacitance for xtal 30 no change
                 SCFI0 &= ~FN_4;                 // DCO range control 31
                 SCFQCTL = 30;                  // (30+1) x 32768 = 0.99MHz
                 break;
         case 2: FLL_CTL0 &= ~DCOPLUS;          // Set load capacitance for xtal 30 no change
                 SCFI0 &= ~FN_4;                 // DCO range control 31
                 SCFQCTL = 60;                  // (60+1) x 32768 = 2.01MHz
                 break;
         case 3: FLL_CTL0 &= ~DCOPLUS;          // Set load capacitance for xtal 30 no change
                 SCFI0 &= ~FN_4;                // DCO range control
                 SCFQCTL = 121;                 // (121+1) x 32768 = 3.96MHz
                 break;
         case 4: FLL_CTL0 |= DCOPLUS + XCAP18PF; // DCO+ set, freq = xtal x D x N+1 31
                 SCFI0 |= FN_4 + FLLD_2;         // DCO range control 32
                 SCFQCTL = 121;                  // (121+1) x 32768 x 2 = 7.99 MHz
                 break;
     }
     return;
 }
