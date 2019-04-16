#include "msp436fg4618.h"
#include "stdio.h"

void Init_LCD(void);

// setup a pointer to the area of memory of the TMS439 that points to
// the segments
// of the softbaugh LCD LCDM3 = the starting address
// each of the seven segments for each display is store in memory
// starting at address LCDM3
// which is the right most seven segment of the LCD4-7
// The bit order in each byte is
// dp, E, G, F, D, C, B, A or
// :, E, G, F, D, c, B, A
// after the seven segments these memory locations are used to turn on
// the special characters
// such as battery status, antenna, f1-f4, etc.
// there are 7 seven segment displays

unsigned char *LCDSeg = (unsigned char *) &LCDM3;

// there are 11 locations that are needed for the softbaugh LCD
// ony 7 used for the seven segment displays

int LCD_SIZE=11;
int loopArray[16];

int main(void)
{
	volatile unsigned char a;
	volatile unsigned int i;
	WDTCTL = WDTPW + WDTHOLD;
	
	P2DIR |= 0x02;
	Init_LCD();
	
	loopArray[0] = 0x5F; //0
	loopArray[1] = 0x06; //1
	loopArray[2] = 0x6B; //2
	loopArray[3] = 0x2F; //3
	loopArray[4] = 0x36; //4
	loopArray[5] = 0x3D; //5
	loopArray[6] = 0x7D; //6
	loopArray[7] = 0x07; //7
	loopArray[8] = 0x7F; //8
	loopArray[9] = 0x37; //9
	loopArray[10] = 0x77; //A
	loopArray[11] = 0x7C; //B
	loopArray[12] = 0x59; //C
	loopArray[13] = 0x6E; //D
	loopArray[14] = 0x79; //E
	loopArray[15] = 0x71; //F
	
	double j;
   start:
	for(i=0; i<16;i++)
	{
		for(j = 999; j>0;)
		{
			j=j-1.0;
		}
		LCDSeg[0] = loopArray[i];
	}
	goto start; // basically an infinite loop
	
}
// -------------------------------------
// Initialize the LCD system

void Init_LCD(void)
{
// Using the LCD A controller for the MSP439fg4618
// the pins of the LCD are memory mapped onto the mp430F4xxx
// memory bus and
// are accessed via LCDSeg[i] array
// See page 269 of Davie’s text
// LCD_SIZE-4 only gives the 7 segment displays plus DP, and
// (colons are the same bit setting)
// LCD_SIZE-4 only gives the 7 segment displays plus DP, and
// colons (colons / dp)
// Right most seven segment display is at LCDSeg[e];
// Display format
// AAA
// F B
// X F B
// GGG
//XEC
// E C
// DP DDD 4-8
// bit order
// dp, E, G, F, D, C, B, A or
// 1, E, G, F; D, C; B, A
	int n;
	for(n=0; n<LCD_SIZE;n++){
		// initialize the segment memory to zero to clear the LCD
		// writing a zero in the LCD memory location clears turns
		// off the LCD segment 
		// Including all of the special characters
		// This way or
		*(LCDSeg+n) = 0;
	//LCDSeg[n]	= 0;	
	}
	// come is fixed and already assigned
	// Need to assign coml - com3 to portS
	P5SEL = 0X1C; // BIT4 l BIT3 |BIT2 = 1 P5.4, P.3, P5.2 = 1
	// Used the internal voltage for the LCD bit 4 = 6 (VLCDEXT=6)
	// internal bias voltage set to 1/3 of Vcc, charge pump disabled,
	// page 26-25 of MSP436X4XX user manual
	LCDAVCTL0 = 0x00;
	// LCDS28-LCDSB pins LCDSQ = lsb and LCDSZS = MSB need
	// LCDS4 through LCDS24
	// from the experimenter board schematic the LCD uses 54-524,
	// 56-53 are not used here
	// Only use up to 524 on the LCD 28-31 not needed.
	// Also LCDACTLl not required since not using S32 - S39
	// Davie's book page 266
	// page 26-23 of MSP436X4XX user manual
	LCDAPCTL0 = 0x7E;
	// The LCD uses the ACLK as the master clock as the scan rate for
	// the display segments
	// The ACLK has been set to 32768 Hz with the external
	// 327768 Hz crystal
	// Let's use scan frequency of 256 Hz (This is fast enough not
	// to see the display flicker)
	// OP a divisor of 128
	// LCDFREQ division(3 bits), LCDMUX (2 bits), LCDSON segments on,
	// Not used, LCDON LCD module on
	// 611 = Freq /128, 11 = 4 mux's needed since the display uses for
	// common inputs come-com3
	// need to turn the LCD on LCDON = 1
	// LCDSON allows the segments to be blanked good for blinking but
	// needs to be on to
	// display the LCD segments LCDSON = 1
	// Bit pattern required = 6111 1161 = 6X7d
	// page 26-22 of MSP436X4XX user manual
	LCDACTL = 0x7d;
}

