;****************** main.s ***************
; Program written by: Megan Cooper and Kaela Todd
; Date Created: 1/22/2016 
; Last Modified: 1/22/2016 
; Section Wednesday 3-4
; Instructor: Ramesh Yerraballi
; Lab number: 4
; Brief description of the program
;   If the switch is presses, the LED toggles at 8 Hz
; Hardware connections
;   PE1 is switch input  (1 means pressed, 0 means not pressed)
;   PE0 is LED output  	 (1 activates external LED on protoboard) 
;Overall functionality of this system is the similar to Lab 3, with three changes:
;   1-  initialize SysTick with RELOAD 0x00FFFFFF 
;   2-  add a heartbeat to PF2 that toggles every time through loop 
;   3-  add debugging dump of input, output, and time
; Operation
;   1) Make PE0 an output and make PE1 an input. 
;   2) The system starts with the LED on (make PE0 =1). 
;   3) Wait about 62 ms
;   4) If the switch is pressed (PE1 is 1), then toggle the LED once, else turn the LED on. 
;   5) Steps 3 and 4 are repeated over and over


LED                     EQU 0x40024004   ;PE0
SWITCH                  EQU 0x40024008   ;PE1	
SYSCTL_RCGCGPIO_R       EQU 0x400FE608
SYSCTL_RCGC2_GPIOE      EQU 0x00000010   ; port E Clock Gating Control
SYSCTL_RCGC2_GPIOF      EQU 0x00000020   ; port F Clock Gating Control
GPIO_PORTE_DATA_R       EQU 0x400243FC
GPIO_PORTE_DIR_R        EQU 0x40024400
GPIO_PORTE_AFSEL_R      EQU 0x40024420
GPIO_PORTE_PUR_R        EQU 0x40024510
GPIO_PORTE_DEN_R        EQU 0x4002451C
GPIO_PORTF_DATA_R       EQU 0x400253FC
GPIO_PORTF_DIR_R        EQU 0x40025400
GPIO_PORTF_AFSEL_R      EQU 0x40025420
GPIO_PORTF_DEN_R        EQU 0x4002551C
NVIC_ST_CTRL_R          EQU 0xE000E010
NVIC_ST_RELOAD_R        EQU 0xE000E014
NVIC_ST_CURRENT_R       EQU 0xE000E018
			THUMB
			AREA    DATA, ALIGN=4
SIZE    		EQU    50
;You MUST use these two buffers and two variables
;You MUST not change their names
;These names MUST be exported
			EXPORT DataBuffer  
			EXPORT TimeBuffer  
			EXPORT DataPt [DATA,SIZE=4] 
			EXPORT TimePt [DATA,SIZE=4]
DataBuffer 	SPACE  SIZE*4
TimeBuffer 	SPACE  SIZE*4
DataPt     	SPACE  4
TimePt     	SPACE  4

    
			ALIGN          
			AREA    |.text|, CODE, READONLY, ALIGN=2
			THUMB
			EXPORT  Start
			IMPORT  TExaS_Init
			IMPORT	SysTick_Init


Start 		
; running at 80 MHz, scope voltmeter on PD3
            	        BL   	TExaS_Init  
; turn on clock for Ports E and F
			LDR 	R0,= SYSCTL_RCGCGPIO_R
			LDR 	R1,[R0]
			ORR 	R1,#0x30
			STR 	R1,[R0]
	   
			NOP
			NOP
; initialize Port E
			LDR 	R0,= GPIO_PORTE_DIR_R
			LDR 	R1,[R0]
			BIC 	R1,#0x02
			ORR 	R1,#0x01
			STR 	R1,[R0]
	   
			LDR 	R0,= GPIO_PORTE_AFSEL_R
			LDR 	R1,[R0]
			BIC 	R1,#0x03
			STR 	R1,[R0]
	   
			LDR 	R0,= GPIO_PORTE_DEN_R
			LDR 	R1,[R0]
			ORR 	R1,#0x03
			STR 	R1,[R0]
			
;			LDR     R0, =GPIO_PORTE_PUR_R
;			LDR     R1, [R0]
;			ORR     R1, #0x02
;			STR     R1,[R0]
			
; initialize Port F
			LDR 	R0,= GPIO_PORTF_DIR_R
			LDR 	R1,[R0]
			ORR 	R1,#0x04
			STR 	R1,[R0]	

			LDR 	R0,= GPIO_PORTF_AFSEL_R
			LDR 	R1,[R0]
			BIC 	R1,#0x04
			STR 	R1,[R0]
			
			LDR 	R0,= GPIO_PORTF_DEN_R
			LDR 	R1,[R0]
			ORR 	R1,#0x04
			STR 	R1,[R0]
			
			BL  	Debug_Init  ; initialize debugging dump, including SysTick
			


			CPSIE  	I           ; TExaS voltmeter, scope runs on interrupts

			LDR     R0,= LED
			LDR     R1,[R0]
			ORR     R1,#0xFF    ; turns the LED on
			STR     R1,[R0]
			
			MOV    	R5,#0xC8	; Counter for Debug_Capture
loop
			
DC_loop			CMP     R5, #0x0
			BEQ     full
			BL  	Debug_Capture
			SUB   	R5, #0x04
			
full			BL	Delay       ; 2480062 instructions
; Heartbeat
			LDR     R3,= GPIO_PORTF_DATA_R
            		LDR     R4, [R3]
            		EOR     R4, #0xFF
            		STR     R4, [R3]    ; 4 instructions
         				   
			LDR  	R0,= SWITCH
			LDR  	R2,= LED    ; 2 instructions
	   
			LDR  	R1,[R0]       
			CMP  	R1,#0       ; 2 instructions
			BNE  	Toggle      ; Goes to Toggle if PE1 = 1
			BEQ  	StayOn      ; Goes to StayOn if PE1 = 0
	   ; 2480070 instructions
	   ; 2480070*2*12.5ns= 62001750ns
	   ; 725ns/62001750ns * 100% = 0.00117%
	   
Toggle 
; Flips PE0 if the switch is pressed
	LDR  R1,[R2]
	EOR  R1, R1, #0xFF
	STR  R1,[R2]
	B    loop; 4 instructions
	   
StayOn 
; Clears PE1 and returns to loop
	LDR  R1,[R2]
	ORR  R1,#0xFF
	STR  R1,[R2]
	B    loop; 4 instructions
	
; Delay	   
Delay  
; Implements a 62ms long delay
    	MOV  R7, #20
Subt   
    	MOV  R8, #62000
wait   
    	SUBS R8, #1
	BNE  wait        ; 2*62000=124000 instructions
    	SUBS R7, #1
	BNE  Subt        ; (124000+3)*20 = 2480060 instructions
	BX   LR          ; 2480060 +2=2480062 instructions

	B    loop


;------------Debug_Init------------
; Initializes the debugging instrument
; Input: none
; Output: none
; Modifies: none
; Note: push/pop an even number of registers so C compiler is happy
Debug_Init
			PUSH	{R1-R3, LR}     ; Store registers that will be used
			LDR		R1, =DataBuffer
			MOV     R3, R1
			ADD     R3, #0xC8
			MOV	R2, #0xFFFFFFFF			
notDone1	STR		R2, [R1]        ; Store 0xFFFFFFFF as the first element of DataBuffer
			ADD     R1, #0x04
			CMP     R1, R3
			BNE     notDone1

            		LDR     R1, =DataBuffer
			LDR	R2, =DataPt
			STR	R1, [R2]        ; Make DataPt point to the start of DataBuffer
			
			LDR	R1, =TimeBuffer
			MOV     R3, R1
			ADD     R3, #0xC8
			MOV	R2, #0xFFFFFFFF
notDone2       		STR	R2, [R1]        ; Store 0xFFFFFFFF as the first element of TimeBuffer
			ADD     R1, #0x04
			CMP     R1, R3
			BNE     notDone2
			
			LDR     R1, =TimeBuffer
			LDR	R2, =TimePt
			STR	R1, [R2]        ; Make TimePt point to the start of TimeBuffer
			
			BL	SysTick_Init    ; Init SysTick
			POP	{R1-R3, LR}     ; Pop stored values back into registers
			BX      LR

;------------Debug_Capture------------
; Dump Port E and time into buffers
; Input: none
; Output: none
; Modifies: none
; Note: push/pop an even number of registers so C compiler is happy
Debug_Capture
			PUSH	{R0-R8,LR}   ; Save used registers to the Stack
			
			LDR	R0, =DataPt
			LDR	R2, [R0]     ; R2 = pointer to DataBuffer

			
			LDR	R5, =TimePt
			LDR	R7, [R5]     ; R7 = pointer to TimeBuffer

			
			LDR	R3, =SWITCH 
			LDR	R3, [R3]     ; R3 = SWITCH value
			LSL	R3, #3
			LDR	R4, =LED
			LDR	R4, [R4]     ; R4 = LED value
			ADD	R4, R3, R4   ; combine LED and SWITCH into one word
			STR	R4, [R2]     ; Store in DataBuffer

            		ADD	R2, #0x04
			STR	R2, [R0]     ; Increment DataPt
			LDR	R8, =NVIC_ST_CURRENT_R
			LDR	R8, [R8]
			STR	R8, [R7]     ; Store time in TimeBuffer
			ADD	R7, #0x04
			STR	R7, [R5]     ; Increment TimePt
			
done    		POP	{R0-R8, LR}
            		BX      LR           ; 29 cycles
			
        
			

			                      ;29*2*12.5ns = 725ns

    			ALIGN                         ; make sure the end of this section is aligned
			END                           ; end of file
        
