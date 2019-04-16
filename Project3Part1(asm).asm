; Lab 3 — Part 2 — Question 1 — Assembly Code (final)
 .cdecls C,LIST,"msp430fg4618.h"
; cdecls tells assembler to allow the device header file
; Main Code

 .sect ".const" ; data Pom for initialized
 .bss label, 4 ; allocates 4 bytes of

; uninitialized memory with the
; name label

.word 0x1234 ; example of defining a 16 bit

; This is the constant area flash begins at address 0x3160 can be
; used for program code or constants

 .sect ".const" ; initialized data rom for

; constants. Use this .sect to
; put data in ROM
; in ROM
 
 .byte 0x0d, 0x0a ; add a CR and a LF
 .byte 0x00 ; null terminate the string with

;These are the four strings that will printed to the terminal whe

string1 .string "SW1 = 0, SNZ = 0"
 .byte 0x0d,0x0a
 .byte 0x00

 .text ; program start 
 .global _START ; define entry point

START mov.w #300h,SP ; Initialize 'x1121
; stackpointer

StopWDT mov.w #WDTPW+WDTHHOLD,&WDTCTL ; Stop WDT
 call #Init_UART ; initialize UART

start
 call #INCHAR_UART ; input
 call #OUTA_UART ; print out
 mov.b R4, R6 ; move R4 int R6
 call #INCHAR_UART ; input
 call #OUTA_UART ; print out
 mov.b R4, R7 ; move R4 into R7
 mov.b #0x09, R4 ; move 0x09 into R4
 call #OUTA_UART ; print out
 
good
 call #hexToChar ; convert hex to char
 jmp start ; jump to start
 
hexToChar
 mov.b R6, R8 ; move R6 into R8
 cmp #0x40, R8 ; compare
 jnc digitl ; jump to digitl if number
 jc letterl ; jump to letterl if letter
 
next
 mov.b R8, R6 ; move R8 into R6
 mov.b R7, R8 ; move R7 into R8
 cmp #0x40, R8 ; compare
 jnc digit2 ; jump to digit2 if number
 jc letter2 ; jump to letterZ if letter

final
 mov.b R8, R7 ; move R8 into R7
 rla.b R6 ; shift R6 left
 rla.b R6 ; shift R6 left
 rla.b R6 ; shift R6 left
 rla.b R6 ; shift R6 left
 add.b R7, R6 ; R6 = R7 & R6
 mov.b R6, R4 ; move R6 into R4
 call #OUTA_UART ; print out
 mov.b #0X0A, R4 ; R4 = new line
 call #OUTA_UART ; print new line
 mov.b #0X0D, R4 ; R4 = carriage return
 call #OUTA;UART ; print carriage return
 ret ; return

digitl
 sub.b #0x30, R8 ;0, R8 ; R8 = 0x30—R8
 jmp next ; jump to next

letterl
 sub.b #0x37, R8 ; R8 = 0x37—R8 
 jmp next ; jump to next
 
digit2
 sub.b #0x30, R8 ; R8 = 0x30—R8
 jmp final ; jump to final
 
letter2
 sub.b #0x37, R8 ; R8 = 6x37—R8
 jmp final ; jump to final
 
send
 mov.b @R;+, R4 ; Store a character in R4 and move 1
 call #OUTA_UART
 cmp #0, R4
 jnz send
 ret
