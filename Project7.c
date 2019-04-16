//------------------------------------------------------------------
// LED Blinky Program using the timer A function and Interrupts
// 		experimenter board RAM at 0x1100 - 0x30FF, FLASH
// 		0x3100 - 0xFBFF
//		Port 2 is used for the LEDs Port 2 bit 2 is the green LED
// 		Port 2 bit 1 is the yellow LED
//
//------------------------------------------------------------------
// must include the C header to get the predefined variable names
//
//------------------------------------------------------------------

#include "msp430fg4618.h"
#include "intrinsics.h"		// include if input and output routines are used in c
#include "stdio.h"

__interrupt void Time_ISR(void);

int main() {
	// tell the compiler not to optimize the variable i
	volatile unsigned int i;
	WDTCTL = WDTPW + WDTHOLD;
	P2DIR |= 0x04;
	P2OUT = 0x04;

	TACCR0 = 0x7000;
	TACTL = TASSEL_1 | ID_0 | MC_1 | TACLR;
	TACTL &= ~TAIFG;

	for(;;) {
		while((TACTL & TAIFG) == 0){}

		P2OUT ^= 0x04;
		TACTL &= ~TAIFG;
	}
}

//#pragma vector = TIMERA0_VECTOR;

/*
 __interrupt void Timer_ISR(void) {
	P2OUT ^= 0x04;			// blink the LED using ex-or
}
*/
