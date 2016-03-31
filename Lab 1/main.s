;****************** main.s ***************
; Program written by: Megan Cooper
; Date Created: 1/22/2016 
; Last Modified: 1/22/2016 
; Section Wednesday 4-5
; Instructor: Ramesh Yerraballi
; Lab number: 1
; Brief description of the program
; The overall objective of this system is a digital lock
; Hardware connections
;  PE3 is switch input  (1 means switch is not pressed, 0 means switch is pressed)
;  PE4 is switch input  (1 means switch is not pressed, 0 means switch is pressed)
;  PE5 is switch input  (1 means switch is not pressed, 0 means switch is pressed)
;  PE2 is LED output (0 means door is locked, 1 means door is unlocked) 
; The specific operation of this system is to 
;   unlock if all three switches are pressed

GPIO_PORTE_DATA_R       EQU   0x400243FC
GPIO_PORTE_DIR_R        EQU   0x40024400
GPIO_PORTE_AFSEL_R      EQU   0x40024420
GPIO_PORTE_DEN_R        EQU   0x4002451C
GPIO_PORTE_AMSEL_R      EQU   0x40024528
GPIO_PORTE_PCTL_R       EQU   0x4002452C
SYSCTL_RCGCGPIO_R       EQU   0x400FE608

      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      EXPORT  Start
		  


Start LDR R0,= SYSCTL_RCGCGPIO_R
	  LDR R1,[R0]
	  ORR R1,#0x10
	  STR R1,[R0]
	
	  NOP
	  NOP
	
	  LDR R0,= GPIO_PORTE_DIR_R
	  LDR R1,[R0]
	  BIC R1,#0x38
	  ORR R1,#0x04
	  STR R1,[R0]
	
	  LDR R0,= GPIO_PORTE_AFSEL_R
	  LDR R1,[R0]
	  BIC R1,#0x3C
	  STR R1,[R0]
	
	  LDR R0,= GPIO_PORTE_DEN_R
	  LDR R1,[R0]
	  ORR R1,#0x3C
	  STR R1,[R0]
			  
	  LDR  R0,= GPIO_PORTE_DATA_R
loop  
      	  LDR  R1, [R0]
	  
	  
	  AND  R2, #0
      	  AND  R3, #0
	  AND  R4, #0
	  
	  EOR  R1, #0x38
	  AND  R2, R1, #0x20
	  AND  R3, R1, #0x10
	  AND  R4, R1, #0x08
	  
	  LSR  R2, #3
	  LSR  R3, #2
	  LSR  R4, #1
	  
	  AND  R2, R2, R3
	  AND  R1, R2, R4
	  	  	  
	  STR  R1,[R0]

	  B    loop
	  
      ALIGN        ; make sure the end of this section is aligned
      END          ; end of file
