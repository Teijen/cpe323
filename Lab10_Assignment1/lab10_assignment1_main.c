/*------------------------------------------------------------------------------
* File:   lab10_assignment2_main.c
* Description:  This C program reads in accel values and displays them to UAH Serial app
* Input:    Accel values
* Output:   Accel values
* Author:   Logan Cannan
* Lab Section:  01
* Date: Nov 11, 2019
*-----------------------------------------------------------------------------*/
#include <msp430xG46x.h>
#include <math.h>
#define SW1 P1IN&BIT0
volatile long int ADCXval, ADCYval, ADCZval;
volatile float Xper, Yper, Zper,Xv, Yv, Zv,M;

void TimerA_setup(void) {
    TACCR0 = 3277;                      // 3277 / 32768 Hz = 0.1s
    TACTL = TASSEL_1 + MC_1;            // ACLK, up mode
    TACCTL0 = CCIE;                     // Enabled interrupt
}

void ADC_setup(void) {
    int i =0;

    P6DIR &= ~BIT0 + ~BIT1 + ~BIT7;             // Configure P6.3, 6.4 and P6.7 as input pins
    P6SEL |= BIT0 + BIT1+ BIT7;               // Configure P6.3, 6.4 and P6.7 as analog pins
    ADC12CTL0 = ADC12ON + SHT0_6 + MSC; // configure ADC converter
    ADC12CTL1 = SHP + CONSEQ_1;         // Use sample timer, single sequence
    ADC12MCTL0 = INCH_0;                // ADC A3 pin -  X-axis
    ADC12MCTL1 = INCH_1;                // ADC A5 pin -  Z-axis
    ADC12MCTL2 = INCH_7 + EOS;          // ADC A7 pin -  Y-axis
                                        // EOS - End of Sequence for Conversions
    ADC12IE |= 0x04;                    // Enable ADC12IFG.1
    for (i = 0; i < 0x3600; i++);       // Delay for reference start-up
    ADC12CTL0 |= ENC;                   // Enable conversions
}

void UART_putCharacter(char c) {
    while(!(IFG2 & UCA0TXIFG));         // Wait for previous character to be sent
    UCA0TXBUF = c;                      // Send byte to the buffer for transmitting
}

void UART_setup(void) {
    P2SEL |= BIT4 + BIT5;               // Set up Rx and Tx bits
    UCA0CTL0 = 0;                       // Set up default RS-232 protocol
    UCA0CTL1 |= BIT0 + UCSSEL_2;        // Disable device, set clock
    UCA0BR0 = 27;                       // 1048576 Hz / 38400
    UCA0BR1 = 0;
    UCA0MCTL = 0x94;
    UCA0CTL1 &= ~BIT0;                  // Start UART device
}

void sendData(void) {
    int i;
    Xv = (ADCXval*3.3/4095);    // Calculate voltage
    Zv = (ADCZval*3.3/4095);
    Yv = (ADCYval*3.3/4095);
    Xper = ((Xv-1.7)/0.3);    // Calculate percentage outputs
    Zper = ((Zv-1.65)/0.3);
    Yper = ((Yv-1.7)/0.3);
    // Use character pointers to send one byte at a time
    M = sqrt(pow(Xper,2) + pow(Yper,2) + pow(Zper,2));
//    M = Xper*Xper + Yper*Yper + Zper*Zper;
    if(M > 2)
    {
        P2OUT |= BIT2;
    }

    char *xpointer=(char *)&Xper;
    char *zpointer=(char *)&Zper;
    char *ypointer=(char *)&Yper;


    UART_putCharacter(0x55);            // Send header
    for(i = 0; i < 4; i++) {            // Send x percentage - one byte at a time
        UART_putCharacter(xpointer[i]);
    }
    for(i = 0; i < 4; i++) {            // Send z percentage - one byte at a time
            UART_putCharacter(zpointer[i]);
        }
    for(i = 0; i < 4; i++) {            // Send y percentage - one byte at a time
        UART_putCharacter(ypointer[i]);
    }
}

int main(void) {
    WDTCTL = WDTPW +WDTHOLD;            // Stop WDT
    TimerA_setup();                     // Setup timer to send ADC data
    ADC_setup();                        // Setup ADC
    UART_setup();                       // Setup UART for RS-232
    _EINT();
    P1IE |= BIT0;                  // P1.0 interrupt enabled
    P1IES |= BIT0;                  // P1.0 hi/low edge
    P1IFG &= ~BIT0;              // P1.0 IFG cleared

    P2DIR |= BIT2;                  // Set LED1 as output
    P2OUT &= BIT2;                  // clear LED1 status


    while (1){
        ADC12CTL0 |= ADC12SC;               // Start conversions
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
    }
    return 0;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCXval = ADC12MEM0;                  // Move results, IFG is cleared
    ADCYval = ADC12MEM2;
    ADCZval = ADC12MEM1;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}

#pragma vector = TIMERA0_VECTOR
__interrupt void timerA_isr() {
    sendData();                           // Send data to serial app
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
    P1IFG &= ~BIT0;                 // P1.0 IFG cleared
    unsigned int j;
    if ((SW1) == 0)
        {
            for (j = 0; j < 2000; j++);    // Debounce ~20 ms
            if ((SW1) == 0)
            {
                P2OUT &= ~BIT2;
            }
        }


    return;
}
