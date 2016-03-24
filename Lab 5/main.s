;****************** main.s ***************
; Program written by: put your names here
; Date Created: 8/25/2013 
; Last Modified: 2/24/2015 
; Section 1-2pm     TA: Youngchun Kim
; Lab number: 5
; Brief description of the program
;   A traffic light controller with 3 inputs and 8 output
; Hardware connections
;The “don’t walk” and “walk” lights must be PF1 and PF3 respectively, but where to attach the others have some flexibility. 
;Obviously, you will not connect both inputs and outputs to the same pin.

;Red south       PA7    PB5    PE5
;Yellow south    PA6    PB4    PE4
;Green south     PA5    PB3    PE3
;Red west        PA4    PB2    PE2
;Yellow west     PA3    PB1    PE1
;Green west      PA2    PB0    PE0
;Table 5.1. Possible ports to interface the traffic lights (PF1=red don’t walk, PF3=green walk).

;Walk sensor     PA4    PD2    PE2
;South sensor    PA3    PD1    PE1
;West sensor     PA2    PD0    PE0
;Table 5.2. Possible ports to interface the sensors.

SYSCTL_RCGCGPIO_R       EQU 0x400FE608
GPIO_PORTA_DATA_R       EQU 0x400043FC
GPIO_PORTA_DIR_R        EQU 0x40004400
GPIO_PORTA_AFSEL_R      EQU 0x40004420
GPIO_PORTA_DEN_R        EQU 0x4000451C
GPIO_PORTB_DATA_R       EQU 0x400053FC
GPIO_PORTB_DIR_R        EQU 0x40005400
GPIO_PORTB_AFSEL_R      EQU 0x40005420
GPIO_PORTB_DEN_R        EQU 0x4000551C
GPIO_PORTD_DATA_R       EQU 0x400073FC
GPIO_PORTD_DIR_R        EQU 0x40007400
GPIO_PORTD_AFSEL_R      EQU 0x40007420
GPIO_PORTD_DEN_R        EQU 0x4000751C
GPIO_PORTE_DATA_R       EQU 0x400243FC
GPIO_PORTE_DIR_R        EQU 0x40024400
GPIO_PORTE_AFSEL_R      EQU 0x40024420
GPIO_PORTE_DEN_R        EQU 0x4002451C
GPIO_PORTF_DATA_R       EQU 0x400253FC
GPIO_PORTF_DIR_R        EQU 0x40025400
GPIO_PORTF_AFSEL_R      EQU 0x40025420
GPIO_PORTF_DEN_R        EQU 0x4002551C
NVIC_ST_CURRENT_R       EQU 0xE000E018
NVIC_ST_CTRL_R          EQU 0xE000E010
NVIC_ST_RELOAD_R        EQU 0xE000E014
PA72					EQU 0x400043F0
PF1						EQU 0x40025008
PF3						EQU 0x40025020
	


      AREA   DATA, ALIGN=2
	  
		  

      ALIGN          
      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      EXPORT  Start
      IMPORT  PLL_Init
	  IMPORT  SysTick_Init
	  IMPORT  SysTick_Wait10ms
Start 
; the DCM.DLL -pCM4 -dLaunchPadDLL debugger setting does NOT grade or simulate traffic
; the DCM.DLL -pCM4 -dEE319KLab5 debugger setting DOES not test all specifications for EE319K
      BL   PLL_Init    ; running at 80 MHz
	  BL   SysTick_Init; Initialize Systick
;Initialize Inputs and Outputs
	  LDR R0,= SYSCTL_RCGCGPIO_R
      LDR R1,[R0]
	  ORR R1,#0x31
	  STR R1,[R0]
	   
	  NOP
	  NOP
	   
	  LDR R0,= GPIO_PORTE_DIR_R
	  LDR R1,[R0]
	  BIC R1,#0x07
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTE_AFSEL_R
	  LDR R1,[R0]
	  BIC R1,#0x07
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTE_DEN_R
	  LDR R1,[R0]
	  ORR R1,#0x07
	  STR R1,[R0]
	   
	  LDR R0,= GPIO_PORTF_DIR_R
	  LDR R1,[R0]
	  ORR R1,#0x0A
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTF_AFSEL_R
	  LDR R1,[R0]
	  BIC R1,#0x0A
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTF_DEN_R
	  LDR R1,[R0]
	  ORR R1,#0x0A
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTA_DIR_R
	  LDR R1,[R0]
	  ORR R1,#0xFC
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTA_AFSEL_R
	  LDR R1,[R0]
	  BIC R1,#0xFC
	  STR R1,[R0]
	  
	  LDR R0,= GPIO_PORTA_DEN_R
	  LDR R1,[R0]
	  ORR R1,#0xFC
	  STR R1,[R0]
;Finite State Machine 
OUT   EQU 0		;Offset for output                     
WAIT  EQU 4		;Offset for time (10ms)
NEXT  EQU 8 	;Offset for next		  
goE   DCD 0x4C	;North Red, East Green
	  DCD 200	;Wait 2 sec
	  DCD goE,goE,wtE1,wtE1,wtE2,wtE2,wtE2,wtE2
wtE1  DCD 0x4A	;North Red, East Yellow
	  DCD 100	;Wait 1 sec
	  DCD goN,goE,goN,goN,wk1,wk1,wk1,wk1
wtE2  DCD 0x4A	;North Red, East Yellow
	  DCD 100	;Wait 1 sec
	  DCD wk1,wk1,wk1,wk1,wk1,wk1,wk1,wk1
goN	  DCD 0x61	;North Green, East Red
	  DCD 200	;Wait 2 sec
	  DCD goN,wtN1,goN,wtN1,wtN2,wtN2,wtN2,wtN2
wtN1  DCD 0x51	;North Yellow, East Red
	  DCD 100	;Wait 1 sec
	  DCD goE,goE,goN,goE,wk2,wk2,wk2,wk2
wtN2  DCD 0x51	;North Yellow, East Red
	  DCD 100	;Wait 1 sec
	  DCD wk2,wk2,wk2,wk2,wk2,wk2,wk2,wk2
wk1   DCD 0x89	;Walk LED on, North Red, East Red
	  DCD 200	;Wait 2 sec
	  DCD on1,wk1,on1,wk1,on1,wk1,on1,wk1
wk2   DCD 0x89	;Walk LED on, North Red, East Red
	  DCD 200	;Wait 2 sec
	  DCD on4,wk2,on4,wk2,on4,wk2,on4,wk2
on1	  DCD 0x89	
	  DCD 25		;Wait 1/4 sec
	  DCD off1,wk1,off1,wk1,off1,wk1,off1,wk1
off1  DCD 0x49
	  DCD 25		;Wait 1/4 sec
	  DCD on2,wk1,on2,wk1,on2,wk1,on2,wk1
on2	  DCD 0x89
	  DCD 25		;Wait 1/4 sec
	  DCD off2,wk1,off2,wk1,off2,wk1,off2,wk1
off2  DCD 0x49		
	  DCD 25		;Wait 1/4 sec
	  DCD on3,wk1,on3,wk1,on3,wk1,on3,wk1
on3	  DCD 0x89	
	  DCD 25		;Wait 1/4 sec
	  DCD off3,wk1,off3,wk1,off3,wk1,off3,wk1
off3  DCD 0x49
	  DCD 25		;Wait 1/4 sec
	  DCD goN,wk1,goN,wk1,goN,wk1,goN,wk1
on4	  DCD 0x89
      DCD 25		;Wait 1/4 sec
	  DCD off4,wk2,off4,wk2,off4,wk2,off4,wk2
off4  DCD 0x49
	  DCD 25		;Wait 1/4 sec
	  DCD on5,wk2,on5,wk2,on5,wk2,on5,wk2
on5	  DCD 0x89
	  DCD 25		;Wait 1/4 sec
	  DCD off5,wk2,off5,wk2,off5,wk2,off5,wk2
off5  DCD 0x49	
	  DCD 25		;Wait 1/4 sec
	  DCD on6,wk2,on6,wk2,on6,wk2,on6,wk2
on6	  DCD 0x89
	  DCD 25		;Wait 1/4 sec
	  DCD off6,wk2,off6,wk2,off6,wk2,off6,wk2
off6  DCD 0x49
	  DCD 25		;Wait 1/4 sec
      DCD goE,wk2,goE,wk2,goE,wk2,goE,wk2
;Port and Timer Initialization
	  LDR R4,= goN						;State Pointer
	  LDR R5,= GPIO_PORTE_DATA_R        ;Port E Switches
	  LDR R6,= PA72
	  LDR R7,= PF1
	  LDR R8,= PF3
FSM   
	  
	  LDR R0, [R4,#OUT] 	;Output Value
	  LSR R0,#2
	  
	  STR R0, [R6]			;Set Lights
	  LDR R0, [R4,#OUT]
	  AND R0,#0x80
	  LSR R0,#6
	  STR R0, [R7]          ;Set Walk Light
	  LDR R0, [R4,#OUT]
	  AND R0,#0x40
	  LSR R0,#3
	  STR R0, [R8]			;Set Don't Walk Light
	  
	  LDR R0, [R4,#WAIT]	;Time Delay
	  BL  SysTick_Wait10ms
	  LDR R0, [R5]			;Read Input
	  AND R0,#0x07
	  LSL R0,R0,#2 			;Offset(index):
							;4 Bytes/Address
	  ADD R0,R0,#NEXT		;8,12,16,20
	  LDR R4, [R4,R0]
	  
	  B    FSM



      ALIGN          ; make sure the end of this section is aligned
      END            ; end of file
      