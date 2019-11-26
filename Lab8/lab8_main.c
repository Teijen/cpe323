/*------------------------------------------------------------------------------
* File:   lab8_main.c
* Description:  This C program communicates via UART to make a simple chat server
* Input:    Data for characters
* Output:   Data for characters
* Author:   Logan Cannan
* Lab Section:  01
* Date: Oct 23, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
#include <string.h>

//function declarations
void UART_initialize();
void UART_sendCharacter(char c);
char UART_getCharacter();
void UART_sendString(char* string);
void UART_getLine(char* buffer, int limit);

char hi[] = "Hey, Bot!"; //all phrases to check and output
char outputHi[] = "\e[34mBot: \e[39mHi, how old are you?";

char age[] = "1000";
char oldAgeOutput[] = "\e[34mBot: \e[39mThis cannot be true!";

char ageOutputOne[] = "\e[34mBot: \e[39mYou are so young! I am 1";
char ageOutputTwo[] = " years old!";
char returnUser[] = "\r\n\e[31mMe:\e[39m";
char user[] = "\e[31mMe:\e[39m";
char buffer[64];
int trash = 0;
int hi_flag = 0;
const int limit = 64; //max limit
int count = 0; // bonus count variable
int main(void)
{
	WDTCTL = WDT_ADLY_1000;	// stop watchdog timer
	IE1 |= WDTIE; // set timer interrupt
	_EINT();
	UART_initialize();
	UART_sendString(user);
	while(1)
	{
	     UART_getLine(buffer,limit);
	     UART_sendString("\n");

	     if(!strcmp(buffer,hi)) //hello trigger
	     {
	         UART_sendString(outputHi);
	         UART_sendString(returnUser);
	         count = 0;
	         hi_flag = 1;
	     }
	     else if (!strcmp(buffer,age) & hi_flag == 1) // for input of 1000
	     {
	         UART_sendString(oldAgeOutput);
	         UART_sendString(returnUser);
	         count = 0;
	         hi_flag = 0;
	     }

	     else if (hi_flag == 1)//(buffer[0] == '\0')
	     {
//	         UART_sendString(user); // for blank input string
//	         count = 0;
	         UART_sendString(ageOutputOne); // send out appropriate response
	         UART_sendString(buffer);
	         UART_sendString(ageOutputTwo);
	         UART_sendString(returnUser);
	         count = 0;
	         hi_flag = 0;
	     }
	     else
	     {
	         UART_sendString(user); // for blank input string
             count = 0;
             hi_flag = 0;
	     }
	}

	return 0;
}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void) {
    count++;
    if(count == 15)
    {
        count = 0;
        int i = 0;
        char waitingBot[] = "\r\n\e[34mBot: \e[39mIs anybody here?\r\n\e[31mMe:\e[39m";
        UART_sendString(waitingBot);
        for(i; i < 64; i++)
        {
            buffer[i] = '\0';
        }
        trash = 1;
        hi_flag = 0;
    }
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
        if(trash == 1)
        {
            i = 0;
            trash = 0;
        }
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
