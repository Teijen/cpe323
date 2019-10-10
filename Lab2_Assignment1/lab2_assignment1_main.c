/*------------------------------------------------------------------------------
* File:   lab2_assignment1_main.c
* Description:  This C program calculates the max and min values of data types for MSP430
* Input:    None
* Output:   None
* Author:   Logan Cannan
* Lab Section:  01
* Date: Sep 03, 2019
*-----------------------------------------------------------------------------*/
#include <msp430.h> 
#include <stdio.h>
#include <float.h>
#include <limits.h>

int main(void)
 {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	int min = 0; //for unsigned min due to not having constant defined in limits
	//print according to assignment doc
	printf("----------------------------------------------------------------------------------------\n"
	       "| Data Type             | Size (in bytes)  | Minimum             | Maximum             |\n"
	       "----------------------------------------------------------------------------------------\n");
	printf("| char                  | %d                | %-20d| %-20d|\n"
	            , sizeof(char),SCHAR_MIN,SCHAR_MAX); //used defined value in limits.h
	printf("| short int             | %d                | %-20d| %-20d|\n"
	            , sizeof(short int),SHRT_MIN,SHRT_MAX);//used defined value in limits.h
	printf("| int                   | %d                | %-20d| %-20d|\n"
	            , sizeof(int),INT_MIN,INT_MAX);//used defined value in limits.h
	printf("| long int              | %d                | %-20ld| %-20ld|\n"
	            , sizeof(long int),LONG_MIN,LONG_MAX);//used defined value in limits.h
	printf("| long long int         | %d                | %-20lld| %-20lld|\n"
	            , sizeof(long long int),LLONG_MIN,LLONG_MAX);//used defined value in limits.h
	printf("| unsigned char         | %d                | %-20u| %-20u|\n"
	            , sizeof(unsigned char),CHAR_MIN,UCHAR_MAX);//used defined value in limits.h
	printf("| unsigned short int    | %d                | %-20u| %-20u|\n"
	            , sizeof(unsigned short int),min,USHRT_MAX);//used defined value in limits.h
	printf("| unsigned int          | %d                | %-20u| %-20u|\n"
	            , sizeof(unsigned int),min,UINT_MAX);//used defined value in limits.h
	printf("| unsigned long int     | %d                | %-20d| %-20lu|\n"
	            , sizeof(unsigned long int),min,ULONG_MAX);//used defined value in limits.h
	printf("| unsigned long long int| %d                | %-20d| %-20llu|\n"
	                , sizeof(unsigned long long int),min,ULLONG_MAX);//used defined value in limits.h
	printf("| float                 | %d                | %-20e| %-20e|\n"
	            , sizeof(float),FLT_MIN,FLT_MAX);//used defined value in float.h
	printf("| double                | %d                | %-20e| %-20e|\n"
	            , sizeof(float),DBL_MIN,DBL_MAX);//used defined value in float.h
	printf("----------------------------------------------------------------------------------------\n");
	return 0;
}
