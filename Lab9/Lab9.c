// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: Kaela Todd and Megan Cooper
// Last modification date: 4/19/2016
// Last Modified: 4/12/2016 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
// (we suggest implementing and testing this first)

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"

#define fresh 1
#define stale 0
#define N 	  300

uint32_t Avg;
uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
uint32_t ADCMail;
uint32_t ADCStatus;
//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm
int32_t TxCounter = 0;
int32_t RxCounter = 0;

void PortF_Init(void){unsigned long volatile delay;  
//Turn on clock for port f
	SYSCTL_RCGCGPIO_R |= 0x20;
//Wait for clock to stabilize
	delay = SYSCTL_RCGCGPIO_R;
//Setup PE2
	GPIO_PORTF_DIR_R   |= 0x0E;
	GPIO_PORTF_DEN_R 	 |= 0x0E;
	GPIO_PORTF_AFSEL_R &= ~0x0E;
	GPIO_PORTF_PUR_R 	 |= 0x0E;
}

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;    					// disable SysTick during setup
  NVIC_ST_CTRL_R = 0x07;
  NVIC_ST_RELOAD_R = 2000000;			// reload value --> 40 Hz
  NVIC_ST_CURRENT_R = 0;      		// any write to current clears it
}
// Get fit from excel and code the convert routine with the constants
// from the curve-fit
uint32_t Convert(uint32_t input){
  uint32_t pos = 0;
	pos = ((input*(439))/1000)+178;
	return pos;
}



void SysTick_Handler(void){ // every 25 ms
	
	uint8_t ADC_Temp[8];
	uint8_t i;
//Toggle Heartbeat Twice
	PF3 ^= 0xFF;          
  PF3 ^= 0xFF;
//Sample the ADC
	ADCMail = ADC_In();
//Set Flag
	ADCStatus = fresh;
	PF3 ^= 0xFF;
//Send Measurement to UART_OutChar
	ADCMail = Convert(ADCMail);
	ADC_Temp[0] = 0x02;
	ADC_Temp[1] = (ADCMail/1000);
	ADCMail -= 1000*ADC_Temp[1];
	ADC_Temp[1] += 0x30;
	ADC_Temp[2] = 0x2E;
	ADC_Temp[3] = ADCMail/100;
	ADCMail -= 100*ADC_Temp[3];
	ADC_Temp[3] += 0x30;
	ADC_Temp[4] = ADCMail/10;
	ADCMail -= 10*ADC_Temp[4];
	ADC_Temp[4] += 0x30;
	ADC_Temp[5] = ADCMail;
	ADC_Temp[5] += 0x30;
	ADC_Temp[6] = 0x0D;
	ADC_Temp[7] = 0x03;
	for(i=0; i<8; i++){
		UART_OutChar(ADC_Temp[i]);
	}
//Increment Counter
	TxCounter++;
//Toggle LED
	PF3	 ^= 0xFF;


}

//Lets us know when the buffer is full
//Set to 1 so that on first implementation
//while the FIFO is empty --> runs through code

void UART1_Handler(void){
//Toggle Heart Beat 
  PF2 ^= 0xFF;
	PF2 ^= 0xFF;
	while((UART1_FR_R & 0x0010) == 0){
		FiFo_Put(UART_InChar());
	}
	RxCounter++;
	UART1_ICR_R = 0x10;   // this clears bit 4 (RXRIS) in the RIS register
	PF2 ^= 0xFF;
}

//Status allows us to see when FIFO is empty
	char data;
  uint32_t FiFo_Get(char* datapt);
int main(void){
//Initialize all below:
	TExaS_Init();       // Bus clock is 80 MHz 
	ADC_Init();    			// initialize to sample ADC1
	ST7735_InitR(INITR_REDTAB);
	SysTick_Init();		
	UART_Init();
	FiFo_Init();
  PortF_Init();
	EnableInterrupts();
	
	while(1){
		while(ADCStatus == 0){};	//Poll ADCStatus flag 
		ADCStatus = 0;						//clear flag
		//Prints from a full FIFO --> BUT must know when 
		//we get to the end 			--> uses while loop to check this condition,
		//and a for loop to print 5 times
		ST7735_SetCursor(6,5);
		//Infinite loop if the fifo is empty / if returns fail
		while (FiFo_Get(&data) == 0) {};
		FiFo_Get(&data);
		for(int i = 1; i <= 5; i++){
			ST7735_OutChar(data);
			FiFo_Get(&data);
		}
		FiFo_Get(&data);
		FiFo_Get(&data);		
		ST7735_SetCursor(12,5);
		ST7735_OutString(" cm");	// print " cm"
	}

}
/*

uint32_t Status[20];             // entries 0,7,12,19 should be false, others true
char GetData[10];  // entries 1 2 3 4 5 6 7 8 should be 1 2 3 4 5 6 7 8
int main3(void){ // Make this main to test FiFo
  FiFo_Init();
  for(;;){
    Status[0]  = FiFo_Get(&GetData[0]);  // should fail,    empty
    Status[1]  = FiFo_Put(1);            // should succeed, 1 
    Status[2]  = FiFo_Put(2);            // should succeed, 1 2
    Status[3]  = FiFo_Put(3);            // should succeed, 1 2 3
    Status[4]  = FiFo_Put(4);            // should succeed, 1 2 3 4
    Status[5]  = FiFo_Put(5);            // should succeed, 1 2 3 4 5
    Status[6]  = FiFo_Put(6);            // should succeed, 1 2 3 4 5 6
    Status[7]  = FiFo_Put(7);            // should fail,    1 2 3 4 5 6 
    Status[8]  = FiFo_Get(&GetData[1]);  // should succeed, 2 3 4 5 6
    Status[9]  = FiFo_Get(&GetData[2]);  // should succeed, 3 4 5 6
    Status[10] = FiFo_Put(7);            // should succeed, 3 4 5 6 7
    Status[11] = FiFo_Put(8);            // should succeed, 3 4 5 6 7 8
    Status[12] = FiFo_Put(9);            // should fail,    3 4 5 6 7 8 
    Status[13] = FiFo_Get(&GetData[3]);  // should succeed, 4 5 6 7 8
    Status[14] = FiFo_Get(&GetData[4]);  // should succeed, 5 6 7 8
    Status[15] = FiFo_Get(&GetData[5]);  // should succeed, 6 7 8
    Status[16] = FiFo_Get(&GetData[6]);  // should succeed, 7 8
    Status[17] = FiFo_Get(&GetData[7]);  // should succeed, 8
    Status[18] = FiFo_Get(&GetData[8]);  // should succeed, empty
    Status[19] = FiFo_Get(&GetData[9]);  // should fail,    empty
  }
}
*/
