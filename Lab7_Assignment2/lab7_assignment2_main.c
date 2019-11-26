/*------------------------------------------------------------------------------
* File:   lab7_assignment1_main.c
* Description:  This C program buzzes buzzer and blinks led
* Input:    None
* Output:   LED Brightness and Sound
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 16, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
void main(void)
{
    WDTCTL = WDT_ADLY_1000;           // 32ms interval (default)
    P2DIR |= BIT2;                  // Set LED1 as output
    P2OUT &= BIT2;
    P3DIR |= BIT5;
    P3SEL |= BIT5;                  //links led to clock
    IE1 |= WDTIE;

    //buzzer  is port 3 bit 5
    TB0CCTL4 = OUTMOD_4;        // TB0 count triggers interrupt

    TB0CCR0 = 524;        // Set TB0 (and maximum) count value
    //TB0CCTL1 = CCIE;        // TB1 count triggers interrupt
    TB0CCR1 = 500;         // Set TB1 count value
    TB0CTL = TBSSEL_2 | MC_1;// SMCLK is clock source, UP
    _BIS_SR(LPM0_bits + GIE);         // Enter LPM0 w/ interrupt
}
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void) {
        P2OUT ^= BIT2; //toggles led
        P3SEL ^= BIT5; //toggles buzzer with buzzer toggle
}
