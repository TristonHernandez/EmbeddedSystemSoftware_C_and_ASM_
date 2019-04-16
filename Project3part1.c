#include "msp430fg4618.h"
#include "stdio.h"

void Init_UART(void);
void OUTA_UART(unsigned char A);
unsigned char INCHAR_UART(void);
unsigned char convert(volatile unsigned char);

int main(void)
{
	volatile unsigned char a; // volatile to prevent optimization
	volatile unsigned char b; // volatile to prevent optimization
	volatile unsigned int i; // volatile to prevent optimization
	WDTCTL = WDTPW + WDTHOLD; // Stop timer
	Init_UART() ;
	
	for(;;)
	{
		a = INCHAR_UART(); // takes in 2 hex ascii numbers
		OUTA_UART(a); // stored in variables a and b
		b = INCHAR_UART(); // and prints out both characters
		OUTA_UART(b) ;
		OUTA_UART(0x0A); // print new line
		OUTA;UART(0x0D); // print carriage return
		a = convert(a); // convert to 8 bit binary number
		b = convert(b); // convert to 8 bit binary number
		a = a*16;
		a = a + b;
		OUTA_UART(a) ;
		OUTA_UART(0x0A); // print new line
		OUTA_UART(0x0D); // print carriage return
}
unsigned char convert(unsigned char a)
{
	if(a < 0x40 && a > 29 ) // if between 0—9 inclusive
	{
		a = a — 0x30;
	}
	else if (a < 0x47 && a > 40) // if between A—F inclusive
	{
		a = a — 0x37;
	}
	return a;
}
void OUTA_UART(unsigned char A) // to print a single character
{
// IFG2 register {1) = 1 transmit buffer is empty,
// UCAOTXBUF 8 bit transmit buffer
// wait fOr the transmit buffer to be empty before sending the
// data out
	do
	{
	}
	while ((IFG2&0x02)==0);
// send the data to the transmit buffer
	UCA0TXBUF =A;
}
unsigned char INCHAR_UART(void) // takes in input Characters
{
	// IFGZ register (0) = 1 receive buffer is f?ll,
	// UCAORXBUF 8 bit receive buffer
	// wait fer the receive buffer is full befOre getting the data
	do
	{
	}
	while ((IFG2&0X0l)==0);
	// go get the char from the receive buffer
	return (UCA0RXBUF);
}
void Init_UART(void)
{ 	
	// Initialization code to set up the Hart on the experimenter
	// board to 8 data,
	// 1 stop, 30 parity, and 9600 baud, polling operation
	P28EL=OX30; // transmit and receive to port 2 b its 4 and ;
	// Bits p2.4 transmit and p2.; receive
	UCAOCTLO=O; // 8 data, no parity 1 stop, uart, async
	// {7)=l (parity), (6)=1 Even, (;): 0 lsb first,
	// (4): 0 8 data / 1 7 data,
	// {3) 0 1 stop 1 / 2 stop, (2—1) —— UART mode,
	// (0) 0 = async
	UCAOCTL1= 0X41; // select ALK 32768 and put in
	// software reset the UART
	// {7—6) 00 UCLK, 01 ACLK (32768 hz), 10 SMCLK,
	// 11 SMCLK
	// {0) = 1 reset
	UCAOBR1=O; // upper byte of divider clock word
	UCAOBRO=3; // clock divide from a clock to bit clock 32768/9600
	// = 3.413
	// UCAOBR1:UCAOBRO two 8 bit reg to from 16 bit
		// clock divider
	// for the baud rate
	UCAOMCTL= OX0 6 i
	// low frequency.mode module 3 modulation pater
	// used for the bit clock
	UCAOSTAT=O; // do not loop the transmitter back to the
		// receiver for echoing
	// (7) = 1 echo back trans to rec
	// (6) = l framing, {;) = 1 overrun, {4) =1 Parity,
	// {3) = 1 break
	// {0) = 2 transmitting or receiving data
	UCA0CTL1=0x40;
	
	IE2 = 0;
}
