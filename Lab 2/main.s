;****************** main.s ***************
; Program written by: ***Your Names**update this***
; Date Created: 1/22/2016 
; Last Modified: 1/22/2016 
; Section ***Tuesday 1-2***update this***
; Instructor: ***Ramesh Yerraballi**update this***
; Lab number: 2
; Brief description of the program
; The overall objective of this system an interactive alarm
; Hardware connections
;  PF4 is switch input  (1 means SW1 is not pressed, 0 means SW1 is pressed)
;  PF3 is LED output (1 activates green LED) 
; The specific operation of this system 
;    1) Make PF3 an output and make PF4 an input (enable PUR for PF4). 
;    2) The system starts with the LED OFF (make PF3 =0). 
;    3) Delay for about 100 ms
;    4) If the switch is pressed (PF4 is 0), then toggle the LED once, else turn the LED OFF. 
;    5) Repeat steps 3 and 4 over and over

GPIO_PORTF_DATA_R       EQU   0x400253FC
GPIO_PORTF_DIR_R        EQU   0x40025400
GPIO_PORTF_AFSEL_R      EQU   0x40025420
GPIO_PORTF_PUR_R        EQU   0x40025510
GPIO_PORTF_DEN_R        EQU   0x4002551C
GPIO_PORTF_AMSEL_R      EQU   0x40025528
GPIO_PORTF_PCTL_R       EQU   0x4002552C
SYSCTL_RCGCGPIO_R       EQU   0x400FE608
PF4                     EQU   0x40025040	
PF3                     EQU   0x40025020

       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT  Start
Start  LDR R0,= SYSCTL_RCGCGPIO_R
       LDR R1,[R0]
	   ORR R1,#0x20
	   STR R1,[R0]
	   
	   NOP
	   NOP
	   
	   LDR R0,= GPIO_PORTF_DIR_R
	   LDR R1,[R0]
	   BIC R1,#0x10
	   ORR R1,#0x08
	   STR R1,[R0]
	   
	   LDR R0,= GPIO_PORTF_AFSEL_R
	   LDR R1,[R0]
	   BIC R1,#0x18
	   STR R1,[R0]
	   
	   LDR R0,= GPIO_PORTF_DEN_R
	   LDR R1,[R0]
	   ORR R1,#0x18
	   STR R1,[R0]
	   
	   LDR R0,= GPIO_PORTF_PUR_R
	   LDR R1,[R0]
	   ORR R1,#0x10
	   STR R1,[R0]
       
	   LDR R0,= PF3
	   LDR R1,[R0]
	   MOV R1,#0x0
	   STR R1,[R0]
	   
	   
loop   BL Delay 
       
	   
       LDR  R0,= PF4
	   LDR  R2,= PF3
	   
	   LDR  R1,[R0]       
	   CMP  R1,#0
	   BEQ  Toggle
	   BNE  Clear
	   
	   
Toggle LDR  R1,[R2]
       EOR  R1, R1, #0xFF
	   STR  R1,[R2]
	   B    loop
	   
Clear  LDR  R1,[R2]
       AND  R1, R1, #0x0
	   STR  R1,[R2]
	   

       B    loop
       
Delay  MOV  R8, #0x58800
wait   SUBS R8, R8, #0x01
       BNE  wait
	   BX   LR 

       ALIGN      ; make sure the end of this section is aligned
       END        ; end of file
       