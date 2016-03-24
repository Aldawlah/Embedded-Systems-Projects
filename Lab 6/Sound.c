// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "tm4c123gh6pm.h"

const uint8_t wave[32] = {8,9,11,12,13,13,14,14,15,15,15,14,14,13,13,12,11,9,8,6,4,4,3,2,1,2,3,4,5,6,8,8};

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input:    interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(uint32_t period){
	DAC_Init();
  NVIC_ST_CTRL_R = 0;         															 // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;															 // reload value
  NVIC_ST_CURRENT_R = 0;      															 // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // Sets to Priority one
  NVIC_ST_CTRL_R = 0xFFFF; 																	 // enable SysTick with core clock and interrupts
}

static uint32_t i = 0;
void SysTick_Handler(void){
	GPIO_PORTB_DATA_R |= 0x08;
  i = (i+1)%(32);
  DAC_Out(wave[i]);
	GPIO_PORTB_DATA_R &= ~0x08;
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none

//How Do we start sound output??
void Sound_Play(uint32_t period){
	if(period == 0){
	NVIC_ST_RELOAD_R =0;
	}
	else{
		NVIC_ST_RELOAD_R = period-1;
		
	}
}

