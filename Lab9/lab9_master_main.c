/*------------------------------------------------------------------------------
* File:   lab9_master_main.c
* Description:  This C program communicates via SPI to make a light blink faster
* Input:    SPI state values
* Output:   SPI state set
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 30, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//function declarations
void UART_initialize();
void UART_sendCharacter(char c);
char UART_getCharacter();
void UART_sendString(char* string);
void UART_getLine(char* buffer, int limit);
void SPI_setup(void);
int SPI_getState(void);
void SPI_setState(int);

char buffer[64];
const int limit = 64; //max limit
int count = 0; // bonus count variable
int stateNum;
char beaconAsk[] = "Beacon pause: ";
char invalid[] = "Invalid pause entered";
char currentPause[] = "Current Pause: ";



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
    UART_initialize();
    SPI_setup();
    int z;
    //for(z = 100; z > 0; z--);      // Delay to allow baud rate stabilize
    UART_sendString(beaconAsk);
    while(1)
    {
        UART_getLine(buffer,limit);
        UART_sendString("\n");
        if(!strcmp(buffer,"?")) //get state
                 {

                    int state = SPI_getState();
                    //UART_sendCharacter(state);
                    for(z = 100; z > 0; z--);      // Delay to allow baud rate stabilize
                    UART_sendString(currentPause);
                    char string[10];
                    sprintf(string,"%d\r\n",state);
                    UART_sendString(string);
                    UART_sendString(beaconAsk);

                 }
                 else if (!strcmp(buffer,"-")) // for input of 1000
                 {
                     SPI_setState(255);
                     UART_sendCharacter('\n');       // Newline
                     UART_sendCharacter('\r');       // Carriage return
                     UART_sendString(beaconAsk);
                 }

                 else if ( atoi(buffer) > 0 && atoi(buffer) < 101)//(buffer[0] == '\0')
                 {
                      int state = atoi(buffer);
                      SPI_setState(state);
                      UART_sendCharacter('\n');       // Newline
                      UART_sendCharacter('\r');       // Carriage return
                      UART_sendString(beaconAsk);
                 }
                 else if ( !strcmp(buffer,"\0"))
                                  {
                                       UART_sendCharacter('\n');       // Newline
                                       UART_sendCharacter('\r');       // Carriage return
                                       UART_sendString(beaconAsk);
                                  }
                 else
                 {
                     UART_sendString(invalid); // for blank input string
                     UART_sendCharacter('\n');       // Newline
                     UART_sendCharacter('\r');       // Carriage return
                     UART_sendString(beaconAsk);
                 }

    }

    return 0;
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

char UART_getCharacter()
{
    char c;
    while(!(IFG2 & UCA0RXIFG)); // Wait until can recieve
    c = UCA0RXBUF;    // TXBUF <= RXBUF
    UART_sendCharacter(c); //echos what is typed
    return c;
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
}

void UART_getLine(char* buffer, int limit)
{
    char c;
    int i = 0;
    do
    {
        c = UART_getCharacter(); // get characters until return or max limit is detected
        if(c == '\r' || i == 63)
        {
            buffer[i] = '\0';
            break;
        }
        else if (c == '\b')
        {
            i--;
            i--;
        }
        buffer[i] = c;
        i++;
    }while((strlen(buffer) < limit));

}

void SPI_setup(void) {
    UCB0CTL0 = UCMSB + UCMST + UCSYNC;// Sync. mode, 3-pin SPI, Master mode, 8-bit data
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // SMCLK and Software reset
    UCB0BR0 = 0x02;                // Data rate = SMCLK/2 ~= 500kHz
    UCB0BR1 = 0x00;
    P3SEL |= BIT1 + BIT2 + BIT3;   // P3.1,P3.2,P3.3 option select
    UCB0CTL1 &= ~UCSWRST;          // **Initialize USCI state machine**
}

int SPI_getState(void) {
    //while((P3IN & 0x01));          // Verifies busy flag
    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = 0;                 // Dummy write to start SPI
    while (!(IFG2 & UCB0RXIFG));   // USCI_B0 TX buffer ready?
    return UCB0RXBUF;
}

void SPI_setState(int State) {
    while(P3IN & 0x01);           // Verifies busy flag
    IFG2 &= ~UCB0RXIFG;
    UCB0TXBUF = State;            // Write new state
    while (!(IFG2 & UCB0RXIFG));  // USCI_B0 TX buffer ready?
}


