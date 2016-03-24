// Piano.c
// This software configures the off-board piano keys
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
#include "tm4c123gh6pm.h"

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile uint32_t *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))
#define PB2     							  (*((volatile uint32_t *)0x40005010))
#define PB1      								(*((volatile uint32_t *)0x40005008))
#define PB0      								(*((volatile uint32_t *)0x40005004))
#define PB210      						  (*((volatile uint32_t *)0x4000501C))
void Piano_Init(void){
	//Initializes Port B PINS 2-0 as Inputs
	SYSCTL_RCGCGPIO_R |= 0x23;
	volatile unsigned long delay;
	delay = SYSCTL_RCGCGPIO_R;
	
	GPIO_PORTB_DIR_R 	 &= 0xF8;
	GPIO_PORTB_DIR_R 	 |= 0x08;
	GPIO_PORTB_AFSEL_R &= 0xF0;
	GPIO_PORTB_DEN_R   |= 0x0F;
	
}

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Piano_In(void){
	//Returns values of pressed switches
	uint32_t Input;
	Input = PB210;
  return Input;
}
