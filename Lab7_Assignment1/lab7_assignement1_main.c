/*------------------------------------------------------------------------------
* File:   lab7_assignment1_main.c
* Description:  This C program dims/brightens LEDs with switch input
* Input:    SW1 , SW2
* Output:   LED Brightness
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 16, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
int clock = 1;
int flag = 0;
#define   SW1 BIT0&P1IN             // SW1 is P1IN&BIT0
#define   SW2 BIT1&P1IN             // SW1 is P1IN&BIT1
void main(void)
{
	WDTCTL = WDT_MDLY_32;           // 32ms interval (default)
	P2DIR |= BIT2;                  // Set LED1 as output
	P2SEL |= BIT2;                  //links led to clock
	IE1 |= WDTIE;


	_EINT();                        // enable interrupts
	P1IE |= BIT0+BIT1;                   // P1.0 interrupt enabled
	P1IES |= BIT0+BIT1;                  // P1.0 hi/low edge
	P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared
	
    TB0CCTL1 = OUTMOD_7;        // TB0 count triggers interrupt

    TB0CCR0 = 500;        // Set TB0 (and maximum) count value
  //TB0CCTL1 = CCIE;        // TB1 count triggers interrupt
    TB0CCR1 = 15;         // Set TB1 count value
    TB0CTL = TBSSEL_1 | MC_1;// ACLK is clock source, UP


    _BIS_SR(LPM0);         // Enter LPM0 w/ interrupt
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
             if ((SW1) == 0 && clock < 5)
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

     if ((SW2) == 0 & (SW1) == 0 & flag == 0)
          {
              for (j = 0; j < 2000; j++);    // Debounce ~20 ms
              if ((SW2) == 0 & (SW1) == 0 & flag == 0)
              {
                flag = 1;
              }
          }

     switch(clock)
     {
         case 1: TB0CCR1 = 15;         // Set TB1 count value
                 break;
         case 2: TB0CCR1 = 100;         // Set TB1 count value
                 break;
         case 3: TB0CCR1 = 200;         // Set TB1 count value
                 break;
         case 4: TB0CCR1 = 300;         // Set TB1 count value
                 break;
         case 5: TB0CCR1 = 400;         // Set TB1 count value
                 break;
     }
     return;
 }
#pragma vector=WDT_VECTOR //bonus achieved
__interrupt void watchdog_timer (void) {
unsigned int i;
if ((SW1) == 0 & (SW2) == 0 & flag ==1)
{
    while((SW1) == 0 & (SW2) == 0 & flag == 1)
    {
        for(i = 0; i < 63000; i++)
        {
            if((SW1) == 1 | (SW2) == 1)
                break;
        }
        P2DIR ^= BIT2;
    }
    P2DIR |= BIT2;
    flag = 0;
}
}
