; LCD Driver for the for MSP 4;6X4XXX experimenter board using
; Softbaugh LCD
; Davies book pg 259, 266
; setup a pointer to the area of memory of the TMS4;6 that points to
; the segments
; of the softbaugh LCD LCDM; = the starting address
	.cdecls C,LIST,"msp430fg4618.h" ; cdecls tells assembler
; to allow
; the device header file
; #LCDM; is the start of the area of memory of the TMS4;6 that points
; to the segments
; of the softbaugh LCD LCDM; = the starting address
; each of the seven segments For each display is store in memory
; starting at address LCDM;
; which is the right most seven segment of the LCD
; The bit order in each byte is
; dp, E, G, F, D, C, B, A OP
;:,E,G,F,D,C,B,A
; after the seven segments these memory locations are used to turn on
; the special characters
; such as battery status, antenna, f1-f4, etc.
; there are 7 seven segment displays
; data area Pam starts axllee
; the .sect directives are defined in lnk_msp4;@f4618.cmd
; .sect ".stack" ; data Pam for the stack
; .sect ".const" ; data rom for initialized data
; constants
; .sect ".text" ; program Pom for code
; .sect ".cinit" ; program Pom for global inits
; .sect ".Peset" ; MSP436 RESET Vector
	.sect ".sysmem"
	
	LCD_SIZE .byte 11
	
	.text
	.global _START
	
START mov.w #300h, sp

StopWDT #WDTPW+WDTHOLD,&WDTCTL
SetupP1 bis.b #0x04,&P2DIR

	call #Init_LCD
	
	mov.b #0x00, R6
	
	mov.w #LCDM3, R5
	mov.b #0x6B, R7
	mov.b R7, 0(R5)
	
MainLoop
	xor.b #0x04,&P2OUT
Wait
	mov.w #0x0A000,R15
L1
	dec.w R15
	jmp MainLoop
	
;---------------------------------------------------
; Initialize the LCD system
;---------------------------------------------------
Init_LCD
; Using the LCD A controller for the MSP439fg4618
; the pins of the LCD are memory mapped onto the mp430F4xxx
; memory bus and
; are accessed via LCDSeg[i] array
; See page 269 of Davie’s text
; LCD_SIZE-4 only gives the 7 segment displays plus DP, and
; (colons are the same bit setting)
; LCD_SIZE-4 only gives the 7 segment displays plus DP, and
; colons (colons / dp)
; Right most seven segment display is at LCDSeg[e];
; Display format
; AAA
; F B
; X F B
; GGG
;XEC
; E C
; DP DDD 4-8
; bit order
; dp, E, G, F, D, C, B, A or
; 1, E, G, F; D, C; B, A
	mov.b #0x00, R6
	mov.w #LCDM3, R5
	mov.b #0x00, R7
	
lbt mov.b R7, 0(R5)

	inc.w R5
	inc.b R6
	
	cmp.b LCD_SIZE, R6
	jnz lpt
	
	mov.b #0x1C, &P5SEL
	mov.b #0x00, &LCDAVCTL0
	mov.b #0x7E, &LCDAPCTL0
	mov.b #0x7d, &LCDACTL
	ret
;-----------------------------------------------------
; bit pattern required = 0111 1101 = 0x7d
; page 26-22 of MSP430x4xx user manual
	mov.b #0x7d, &LCDACTL
	ret
;-----------------------------------------------------
; Interrupt Vectors
;-----------------------------------------------------
	.sect ".reset"
	.short START
	.end
