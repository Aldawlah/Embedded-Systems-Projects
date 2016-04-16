// Lab8.c
// Runs on LM4F120 or TM4C123
// Student names: Kaela Todd and Megan Cooper
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/5/2016 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats

#include <stdint.h>
#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"

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
void SysTick_Init(void);
void SystTick_Handler(void);

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
	
// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	volatile uint32_t delay;
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

void SysTick_Handler(void){
//Toggle Heartbeat Twice
	PF2 ^= 0xFF;          
  PF2 ^= 0xFF;
//Sample ADC
	for(int i = 0; i < N; i++){
		ADCMail += ADC_In();
	}
	ADCMail = (ADCMail/N);
//Set Flag
	ADCStatus = fresh;
	PF2 ^= 0xFF;
}


/*int main(void){      // single step this program and look at Data
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 1
  }
}
*/
/*
int main(void){
  TExaS_Init();       // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
  while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec    
		PF2 ^= 0xFF;       // Profile ADC
		Data = ADC_In();
		PF2 ^= 0xFF;       // end of ADC Profile
		ST7735_SetCursor(0,0);
		//PF1 ^= 0xFF;       // Profile LCD
		LCD_OutDec(Data);
		ST7735_OutString("    ");  // these spaces are used to coverup characters from last output
		//PF1 ^= 0xFF;          // end of LCD Profile
  }
}
*/
uint32_t Convert(uint32_t input){
  uint32_t pos = 0;
	pos = ((input*(439))/1000)+178;
	return pos;
}
/*
int main(void){ 
  TExaS_Init();         // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  ST7735_InitR(INITR_REDTAB);   
  PortF_Init();
  while(1){  
    PF2 ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 1
    PF3 = 0x08;       // Profile Convert
    Position = Convert(Data); 
    PF3 = 0;          // end of Convert Profile
    PF1 = 0x02;       // Profile LCD
		ST7735_SetCursor(0,0);
    LCD_OutDec(Data);
		ST7735_OutString("    "); 
		ST7735_SetCursor(6,0);
    LCD_OutFix(Position);
    PF1 = 0;          // end of LCD Profile
  }
} 
*/

int main(void){
// Bus clock is 80 MHz 
  TExaS_Init();         			
// turn on ADC, set channel to 1
  ADC_Init();         				
  ST7735_InitR(INITR_REDTAB); 
//Initialize PortF for LEDs
  PortF_Init();
//Initialize SysTick
  SysTick_Init();							
  while(1){
	  while(ADCStatus == 0){}		//Poll ADCStatus flag 
	  uint32_t pos = ADCMail;		//read ADCMail (input)
	  ADCStatus = 0;						//clear flag
	  pos = Convert(pos);				//convert the input to cm
	  ST7735_SetCursor(6,5);		
    LCD_OutFix(pos);					// print the fixed point value 
    ST7735_SetCursor(12,5);
    ST7735_OutString(" cm");	// print " cm"
		
	}
}
