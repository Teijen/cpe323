/*------------------------------------------------------------------------------
* File:   lab9_slave_main.c
* Description:  This C program communicates via SPI to make a simple chat server
* Input:    SPI state set
* Output:   SPI state values
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 30, 2019
*-----------------------------------------------------------------------------*/
#include "msp430x20x3.h"

#define SET_LED()       P1OUT |= 0x01
#define RESET_LED()     P1OUT &= ~0x01
#define SET_BUSY_FLAG()   P1OUT |= 0x10
#define RESET_BUSY_FLAG() P1OUT &= ~0x10

int count = 0;
int countMax = 100;
int NextState;

void SPI_setup(void) {
    USICTL0 |= USISWRST;          // Set UCSWRST -- needed for re-configuration process
    USICTL0 |= USIPE5 + USIPE6 + USIPE7 + USIOE; // SCLK-SDO-SDI port enable,MSB first
    USICTL1 = USIIE;              // USI  Counter Interrupt enable
    USICTL0 &= ~USISWRST;         // **Initialize USCI state machine**
}

void SPI_initComm(void) {
    USICNT = 8;                   // Load bit counter, clears IFG
    USISRL = countMax;            // Set LED state
    RESET_BUSY_FLAG();            // Reset busy flag
}


void main(void) {
 ///has DCO stuff on ontop
    P1DIR |= BIT0;                // P1.0 as output - LED3
    P1DIR |= BIT4;                // P1.4 as output - Busy flag
    P1OUT |= BIT0;

    SPI_setup();                  // Setup USI module in SPI mode
    SPI_initComm();

    WDTCTL = WDT_MDLY_32; // stop watchdog timer
    IE1 |= WDTIE; // set timer interrupt
    _EINT();


    while(1)
    {
    _BIS_SR(LPM0_bits + GIE );// Enter LPM0 with interrupt
    switch(NextState)
           {
               case 255:   countMax = 255;
                           break;

               case 0:     USISRL = countMax;
                           break;

               default:    countMax = NextState;
                           break;

           }
    RESET_BUSY_FLAG();            // Reset busy flag
    }
}
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void) {
    RESET_LED();
    count++;
    if(count > (countMax +1)  && countMax != 255)
    {
        count = 0;
        SET_LED();
    }

}
#pragma vector = USI_VECTOR;
__interrupt void USI_ISR(void){
    SET_BUSY_FLAG();
    NextState = USISRL;           // Read new command
    USICNT = 8;                   // Load bit counter for next TX
    //RESET_BUSY_FLAG();            // Reset busy flag
    _BIC_SR_IRQ(LPM0_bits);       // Exit from LPM0 on RETI
}
