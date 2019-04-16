//---------------------------------------------------------------------
// C-language program to satisfy Laboratory Experiment #5
// for Embedded Systems - Spring 2018 - Section 0013 
//---------------------------------------------------------------------


// Includes
#include "msp430fg4618.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// function prototypes
void Init_UART(void);
void OUTA_UART(unsigned char A); 
unsigned char INCHAR_UART(void); 
void Init_LCD(void);

// written function prototypes
char char2num(char a); 			// converts ASCII chars to int
char num2char(int a)			// converts int to ASCII char for output

// global variables
unsigned char *LCDSeg = (unsigned char *) &LCDM3; 
int LCD_SIZE = 11;

int numbers[16] = { 			// programmed numericals in hex for the LCD
	0x5F, 	// 0
	0x06, 	// 1
	0x6B, 	// 2
	0x2F,	// 3
	0x36,	// 4
	0x3D,	// 5
	0x7D, 	// 6
	0x07,	// 7
	0x7F,	// 8
	0x3F,	// 9
	0x77,	// 10 - A
	0x7C,	// 11 - B
	0x59,	// 12 - C
	0x6E,	// 13 - D
	0x79,	// 14 - E
	0x71	// 15 - F
}; 

// main function
int main(void) { 
	WDTCTL = WDTPW + WDTHOLD; 	// withhold watchdog timer
	Init_UART();				// initialize UART
	Init_LCD();					// initialize LCD Display
	
	for(;;) { 
		// read inputs from HyperTerminal
		char digit1, digit2, digit3, digit4; 
		
		// first numerical
		OUTA_UART(0x30); // '0'
		OUTA_UART(0x78); // 'x'
		digit1 = INCHAR_UART(); 
		OUTA_UART(digit1); 
		digit2 = INCHAR_UART(); 
		OUTA_UART(digit2);
		
		// second numerical
		OUTA_UART(0x20); // space
		OUTA_UART(0x30); // '0'
		OUTA_UART(0x78); // 'x'
		digit3 = INCHAR_UART(); 
		OUTA_UART(digit3);
		digit4 = INCHAR_UART(); 
		OUTA_UART(digit4); 
		
		// newline and carraige return
		OUTA_UART(0x0A); 
		OUTA_UART(0x0D); 
		
		// converting the digits
		digit1 = char2num(digit1); 
		digit2 = char2num(digit2); 
		digit3 = char2num(digit3); 
		digit4 = char2num(digit4); 
		
		// displaying the digits on the LCD Display
		LCDSeg[4] = numbers[digit1]; 
		LCDSeg[3] = numbers[digit2]; 
		LCDSeg[2] = 0x00; 				// blank number, space
		LCDSeg[1] = numbers[digit3]; 
		LCDSeg[0] = numbers[digit4];
	}
}

char char2num(char a) { // converts ASCII Character to Int Char
	char b = ' '; 		// buffer character
	if (a > 0x30 && a <= 0x39) b = a - 0x30; 
	if (a > 0x41 && a <= 0x46) b = a - 0x37; 
	return b; 
} 

char num2char(int a) { 	// converts int num to ASCII Character 
	char b = ' '; 		// buffer character
	if (a >= 0 && a < 10) b = a + 0x30; 
	if (a > 10 && a < 16) b = a + 0x37; 
	return b; 
} 

//---------------------------------------------------------------------
// Initialize the LCD system
//---------------------------------------------------------------------
void Init_LCD(void){						// initialize the LCD Dislay on the MSP430 Experimenter Board
	int n;
	for (n=0;n<LCD_SIZE;n++){
		*(LCDSeg+n) = 0;
	}
	P5SEL = 0x1C;
	LCDAVCTL0 = 0x00;
	LCDAPCTL0 = 0x7E;
	LCDACTL = 0x7d;
}
//---------------------------------------------------------------------
// UART FUNCTIONS AND INIALIZATION
//---------------------------------------------------------------------
void OUTA_UART(unsigned char A){			// send character out via UART from character
	do{
	 }while ((IFG2&0x02)==0);
	UCA0TXBUF =A;
} unsigned char INCHAR_UART(void){			// intake character from UART into character
	do{
	}while ((IFG2&0x01)==0);
	return (UCA0RXBUF);
} void Init_UART(void){						// initalize the MSP430-FG4618 UART communication
	P2SEL=0x30;
	UCA0CTL0=0;
	UCA0CTL1= 0x41;
	UCA0BR1=0;
	UCA0BR0=3;
	UCA0MCTL=0x06;
	UCA0STAT=0;
	UCA0CTL1=0x40;
	IE2=0; 
}
