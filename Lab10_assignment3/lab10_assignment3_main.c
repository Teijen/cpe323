/*------------------------------------------------------------------------------
* File:   lab10_assignment3_main.c
* Description:  This C program reads matlab for table
* Input:    matlab header files
* Output:   tables on DAC
* Author:   Logan Cannan
* Lab Section:  01
* Date: Nov 11, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h>
#include <sine_lut_512.h>  /*256 samples are stored in this table */
#include <saw_lut_512.h>
#define SW1 P1IN&BIT0
#define SW2 P1IN&BIT1
int sawFlag = 0;
int halfFlag = 0;
int halfState = 0;
void TimerA_setup(void) {
    TACTL = TASSEL_2 + MC_1;              // SMCLK, up mode
    TACCR0 = 81;                         // Sets Timer Freq
    TACCTL0 = CCIE;                       // CCR0 interrupt enabled
}

void DAC_setup(void) {
    ADC12CTL0 = REF2_5V + REFON;          // Turn on 2.5V internal ref volage
    unsigned int i = 0;
    for (i = 50000; i > 0; i--);          // Delay to allow Ref to settle
    DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC;   //Sets DAC12
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;             // Stop WDT
    TimerA_setup();                       // Set timer to uniformly distribute the samples
    DAC_setup();
    _EINT();                        // enable interrupts
    P1IE |= BIT0+BIT1;                   // P1.0 interrupt enabled
    P1IES |= BIT0+BIT1;                  // P1.0 hi/low edge
    P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared// Setup DAC
    unsigned int i = 0;
    while (1) {
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled

        i=(i+1)%512;
        if (sawFlag == 1 && halfFlag == 0){ //if 1 is pressed but 2 is not
                DAC12_0DAT = SAW512[i];
               }
        else if (sawFlag == 1 && halfFlag >= 1){ // if 2 is pressed and 1 is not
                DAC12_0DAT = (SAW512[i]) >> 1;
//                DAC12_0DAT = ((SAW512[i])/(2 * halfFlag));
                //TACCR0 = 80/(2 * halfFlag);                         // Sets Timer Freq
               }
        else if (sawFlag == 0 && halfFlag >= 1){ // if 2 is pressed and 1 is not
                DAC12_0DAT = (SINE512[i]) >> 1;
//                DAC12_0DAT = (SINE512[i])/(2 * halfFlag);
                //TACCR0 = 80/(2 * halfFlag);                         // Sets Timer Freq
                }
        else
        {
            DAC12_0DAT = SINE512[i];
        }
    }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void) {
    __bic_SR_register_on_exit(LPM0_bits);  // Exit LPMx, interrupts enabled
}
// Port 1 interrupt service routine
 #pragma vector = PORT1_VECTOR
 __interrupt void Port1_ISR (void)
 {
     P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared
     unsigned long int j;
     for (j = 0; j < 2000; j++);    // Debounce ~20 ms

     if ((SW1) == 0)
         {
                sawFlag = 1;
                P1IES &= BIT0;                  // P1.0 low/hi edge
         }
     else if ((SW1) != 0)
              {
                 sawFlag = 0;
                 P1IES |= BIT0;                  // P1.0 hi/low edge
              }
     if ((SW2) == 0)
          {
              halfFlag = 1;
              P1IES &= BIT1;                  // P1.0 low/hi edge
          }
     else if ((SW2) != 0)
                  {
                    halfFlag = 0;
                    P1IES |= BIT1;                  // P1.0 hi/low edge
                  }
     return;
 }

