// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: put your names here
// Date Created: 1/24/2015 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// Port A PINS 5, 4, 3 and 2 --> DAC 				(Output)
// Port B PINS 2, 1, 0 			 --> Piano Keys (Input)


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "dac.h"
#include "TExaS.h"
//#include "Timer0A.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void SysTick_Init(void);
void Heartbeat_Init(void);

#define F80HZ (50000000/80)
#define A  5680
#define C  4777
#define Eb 4017

const uint32_t song[6] = {A,C,Eb,A,C,Eb};
int main(void){      
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
	//Timer0A_Init(&songTask, F80HZ);
  Piano_Init();
  Sound_Init(0);
	DAC_Init();
	Heartbeat_Init();
  // other initialization
  EnableInterrupts();
	uint32_t input;
  while(1){  
			GPIO_PORTF_DATA_R ^= 0x04;
			input = Piano_In();
			if (input == 0x04){
				Sound_Play(A);
			}
			else if (input == 0x02){
				Sound_Play(C);
			}
			else if (input == 0x01){
				Sound_Play(Eb);
			}
			else if (input == 0x07){
				Sound_Play(A);
			}
			else {
				Sound_Play(0);
			}
				
  }         
} 
//Testing the DAC
/*int main(void){
	TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);
	DAC_Init();
	uint32_t Data= 0x00; //0,1,2,3,4...14,15
	while(1){
		for(int x = 0; x <= 0x15; x++){
		DAC_Out(Data);
		Data = 0x3C&(Data+0x04);
		}
	}
}*/

void Heartbeat_Init(void){
	GPIO_PORTF_DIR_R   |= 0x0E;
	GPIO_PORTF_AFSEL_R &= ~0x0E; 
	GPIO_PORTF_DEN_R   |= 0x0E;
}

