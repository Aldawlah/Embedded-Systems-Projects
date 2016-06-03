// Kaela Todd and Megan Cooper
// DAC.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123



#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "dac.h"


// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){ //PB5 - PB2
	SYSCTL_RCGCGPIO_R |= 0x02;
	volatile unsigned long delay;
	delay = SYSCTL_RCGCGPIO_R;
	
	GPIO_PORTB_DIR_R    |= 0x3C;
	GPIO_PORTB_AFSEL_R  &= ~0x3C;
	GPIO_PORTB_DEN_R    |= 0x3C;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
#define DAC (*((volatile uint32_t *)0x400050F0)) //PB5-PB2

void DAC_Out(unsigned long data){
	DAC = data;
}
