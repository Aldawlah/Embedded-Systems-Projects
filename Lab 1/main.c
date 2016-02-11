// Program written by: ***Your Names**update this***
// Date Created: 1/22/2016 
// Last Modified: 1/22/2016 
// Section ***Tuesday 1-2***update this***
// Instructor: ***Ramesh Yerraballi**update this***
// Lab number: 1 (due as HW3)
// Brief description of the program
// The overall objective of this system is a digital lock
// Hardware connections
//  PE3 is switch input  (1 means switch is not pressed, 0 means switch is pressed)
//  PE4 is switch input  (1 means switch is not pressed, 0 means switch is pressed)
//  PE5 is switch input  (1 means switch is not pressed, 0 means switch is pressed)
//  PE2 is LED output (0 means door is locked, 1 means door is unlocked) 
// The specific operation of this system is to 
//   unlock if all three switches are pressed

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

void Init(void);
uint32_t delay;
#define mask1  0x10
#define mask2  0x38
#define mask3  0x04
#define mask4  0x3C
#define mask5  0x00
#define PE2    (*((volatile uint32_t *)0x40024010))
#define PE5    (*((volatile uint32_t *)0x40024080))
#define PE4    (*((volatile uint32_t *)0x40024040))	
#define PE3    (*((volatile uint32_t *)0x40024020))

int main(void){ 
	
	Init();
	GPIO_PORTE_DATA_R &= mask5;
	
  while(1){
		
		uint32_t Result = ((~PE5>>3) & (~PE4>>2) & (~PE3>>1));
		PE2 = Result;
		
  }
	
}

void Init(void){
	
	SYSCTL_RCGCGPIO_R  |= mask1;
	delay = 100;
	
	GPIO_PORTE_DIR_R   &= mask2;
	GPIO_PORTE_DIR_R   |= mask3;
	
	GPIO_PORTE_AFSEL_R &= mask4;
	
	GPIO_PORTE_DEN_R   |= mask4;
	
}
