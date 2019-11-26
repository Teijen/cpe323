/*------------------------------------------------------------------------------
* File:   final_main.c
* Description:  This C program acts as a proximity alarm
* Input:    IR Prox sensor
* Output:   Data for distance
* Author:   Logan Cannan
* Lab Section:  01
* Date: Nov 25, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
//variables/symbolics
#define SW1 P1IN&BIT0
#define SW2 P1IN&BIT1
volatile long int ADCXval;
volatile float Xper;

//flags
int alarmFlag = 0;
int leftCount = 0;
int rightCount = 0;
int count = 0;


//messages
char help[] = "Barrier Broken. Alarm Activated."; //all phrases to check and output
char welcome[] = "Barrier Set. Alarm On."; //all phrases to check and output
char output[14]; //output distance to uart

//functions
void UART_initialize();
void UART_sendCharacter(char c);
void UART_sendString(char* string);

void TimerA_setup(void) {
    TACCR0 = 3277;                      // 3277 / 32768 Hz = 0.1s
    TACTL = TASSEL_1 + MC_1;            // ACLK, up mode
    TACCTL0 = CCIE;                     // Enabled interrupt
}
void TimerB_setup(void) {
    //buzzer  is port 3 bit 5
       TB0CCTL4 = OUTMOD_4;        // TB0 count triggers interrupt
       TB0CCR0 = 524;        // Set TB0 (and maximum) count value
       //TB0CCTL1 = CCIE;        // TB1 count triggers interrupt
       TB0CCR1 = 500;         // Set TB1 count value
       TB0CTL = TBSSEL_2 | MC_1;// SMCLK is clock source, UP


       P3DIR |= BIT5;
       P3SEL &= ~BIT5;                  //links buzzer to clock
}

void ADC_setup(void) {
    int i =0;
    P6DIR &= ~BIT7;             // Configure  P6.7 as input pin
    P6SEL |= BIT7;               // Configure  P6.7 as analog pin
    ADC12CTL0 = ADC12ON + SHT0_6; // configure ADC converter
    ADC12CTL1 = SHP;         // Use sample timer, single sequence
    ADC12MCTL0 = INCH_7;          // ADC A7 pin -  only data line
    ADC12IE |= 0x01;                    // Enable ADC12IFG.1
    for (i = 0; i < 0x3600; i++);       // Delay for reference start-up
    ADC12CTL0 |= ENC;                   // Enable conversions
}

void sendData(void) {
    Xper = ((47125)/(ADCXval + 78.7))-7;    // Calculate distance

    if(Xper < 15) // if distance is under the 15cm threshhold for three checks trigger
    {
        count++;
    }
    if(count > 2)
    {
        P3SEL |= BIT5;                  //turns buzzer on
        P2OUT |= BIT2;                   //turn on led
        alarmFlag = 1;                  //turns on alarm message instead of distance output
    }
    if (alarmFlag == 1)
    {
        UART_sendString(help);
    }
    else{
        sprintf(output, "Distance: %.2f cm",Xper);
        UART_sendString(output);
    }
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    TimerA_setup();                     // Setup timer to send ADC data
    TimerB_setup();                     // Setup timer for buzzer
    ADC_setup();                        // Setup ADC
    UART_initialize();                  //setup UART
    _EINT();
    P1IE |= BIT0 + BIT1;                  // P1.0 interrupt enabled
    P1IES |= BIT0 + BIT1;                  // P1.0 hi/low edge
    P1IFG &= ~BIT0 + BIT1;              // P1.0 IFG cleared

    P2DIR |= BIT2;                  // Set LED1 as output
    P2OUT &= ~BIT2;                 //turn led off

    UART_sendString(welcome);       //send welcome string
    alarmFlag = 0;
    while (1){
            ADC12CTL0 |= ADC12SC;               // Start conversions
            __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
        }
	
	return 0;
}
#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void) {
    ADCXval = ADC12MEM0;                  // Move results, IFG is cleared
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}
#pragma vector = TIMERA0_VECTOR
__interrupt void timerA_isr() {
    sendData();                           // Send data to UART
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
}
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
    //sets logic for reset password
    //password = right, right, left
    P1IFG &= ~BIT0+BIT1;                 // P1.0 IFG cleared
    unsigned long int j;
    if ((SW1) == 0)
        {
            for (j = 0; j < 2000; j++);    // Debounce ~20 ms
            if ((SW1) == 0 && leftCount == 0 && (rightCount == 0 || rightCount == 1))
            {
                rightCount++;
            }
            else
            {
                rightCount = 0;
                leftCount = 0;
            }
        }
    if ((SW2) == 0)
            {
                for (j = 0; j < 2000; j++);    // Debounce ~20 ms
                if ((SW2) == 0 && rightCount == 2)
                {
                    P3SEL &= ~BIT5;                  //links buzzer to clock
                    P2OUT &= ~BIT2;                   //turn off led
                    alarmFlag = 0;
                    UART_sendString(welcome);
                    for (j = 0; j < 300000; j++);    // Debounce ~20 ms
                    count = 0;
                    rightCount = 0;
                    leftCount = 0;
                }
                else
                {
                    rightCount = 0;
                    leftCount = 0;
                }
            }
    return;
}
void UART_initialize()
{
    UCA0CTL1 |= UCSWRST;// Set software reset during initialization
    P2SEL |= BIT4+BIT5; // set UC0TXD and UC0RXD to transmit and receive data
    UCA0CTL1 |= UCSSEL_2; // BRCLK=SMCLK
    UCA0BR0 = 0x09;       // 1MHz/115200 (lower byte)
    UCA0BR1 = 0x00;       // 1MHz/115200 (upper byte)
    UCA0MCTL |= BIT2;     // Modulation (UCBRS0=0x01)(UCOS16=0)
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
   // IE2 |= UCA0RXIE;      // Enable USCI_A0 RX interrupt
}

void UART_sendCharacter(char c)
{
    while(!(IFG2 & UCA0TXIFG)); // Wait until can transmit
    UCA0TXBUF = c;  //sets output register to send out the character
}

void UART_sendString(char* string)
{
    int i;
    i = strlen(string);
    int j = 0;
    while (j<i) //itterate through string and call sendCharacter for each character
         {
           UART_sendCharacter(string[j]);
           j++;
         }
    UART_sendCharacter('\n');
    UART_sendCharacter('\r');
}

