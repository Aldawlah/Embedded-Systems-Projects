;****************** main.s ***************
; Program written by: Megan Cooper
; Date Created: 1/22/2016 
; Last Modified: 1/22/2016 
; Section Wednesday 4-5
; Instructor: Ramesh Yerraballi
; Lab number: 3
; Brief description of the program
;   If the switch is presses, the LED toggles at 8 Hz
; Hardware connections
;  PE1 is switch input  (1 means pressed, 0 means not pressed)
;  PE0 is LED output (1 activates external LED on protoboard) 
;Overall functionality of this system is the similar to Lab 2, with six changes:
;1-  the pin to which we connect the switch is moved to PE1, 
;2-  you will have to remove the PUR initialization because pull up is no longer needed. 
;3-  the pin to which we connect the LED is moved to PE0, 
;4-  the switch is changed from negative to positive logic, and 
;5-  you should increase the delay so it flashes about 8 Hz.
;6-  the LED should be on when the switch is not pressed
; Operation
;   1) Make PE0 an output and make PE1 an input. 
;   2) The system starts with the LED on (make PE0 =1). 
;   3) Wait about 62 ms
;   4) If the switch is pressed (PE1 is 1), then toggle the LED once, else turn the LED on. 
;   5) Steps 3 and 4 are repeated over and over


GPIO_PORTE_DATA_R       EQU   0x400243FC
GPIO_PORTE_DIR_R        EQU   0x40024400
GPIO_PORTE_AFSEL_R      EQU   0x40024420
GPIO_PORTE_DEN_R        EQU   0x4002451C
GPIO_PORTE_AMSEL_R      EQU   0x40024528
GPIO_PORTE_PCTL_R       EQU   0x4002452C
SYSCTL_RCGCGPIO_R       EQU   0x400FE608
PE1                     EQU   0x40024008
PE0                     EQU   0x40024004
	
       IMPORT  TExaS_Init
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT  Start
Start
 ; TExaS_Init sets bus clock at 80 MHz
	BL  TExaS_Init ; voltmeter, scope on PD3
; you initialize PE1 PE0
    LDR R0,= SYSCTL_RCGCGPIO_R
    LDR R1,[R0]
	ORR R1,#0x10
	STR R1,[R0]
	   
	NOP
	NOP
	   
	LDR R0,= GPIO_PORTE_DIR_R
	LDR R1,[R0]
	BIC R1,#0x02
	ORR R1,#0x01
	STR R1,[R0]
	   
	LDR R0,= GPIO_PORTE_AFSEL_R
	LDR R1,[R0]
	BIC R1,#0x03
	STR R1,[R0]
	   
	LDR R0,= GPIO_PORTE_DEN_R
	LDR R1,[R0]
	ORR R1,#0x03
	STR R1,[R0]


    CPSIE  I    ; TExaS voltmeter, scope runs on interrupts
	  
	LDR R0,= PE0
	LDR R1,[R0]
	ORR R1,#0xFF ; turns the LED on
	STR R1,[R0]

loop  
; you input output delay
	BL Delay 
       
	   
    LDR  R0,= PE1
	LDR  R2,= PE0
	   
	LDR  R1,[R0]       
	CMP  R1,#0
	BNE  Toggle ; Goes to Toggle if PE1 = 1
	BEQ  StayOn ; Goes to StayOn if PE1 = 0
	   
	   
Toggle 
; Flips PE0 if the switch is pressed
	LDR  R1,[R2]
	EOR  R1, R1, #0xFF
	STR  R1,[R2]
	B    loop
	   
StayOn 
; Clears PE1 and returns to loop
    LDR  R1,[R2]
    ORR  R1,#0xFF
	STR  R1,[R2]
	B    loop
	   
Delay  
; Implements a 62ms long delay
    MOV  R7, #20
Subt   
    MOV  R8, #62000
wait   
    SUBS R8, #1
    BNE  wait 
    SUBS R7, #1
	BNE  Subt
	BX   LR

    ALIGN      ; make sure the end of this section is aligned
    END        ; end of file
       
