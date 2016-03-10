// ***** 0. Documentation Section *****
// TableTrafficLight.c for (Lab 10 edX), Lab 5 EE319K
// Runs on LM4F120/TM4C123
// Program written by: Megan Cooper
// Date Created: 1/24/2015 
// Last Modified: 3/2/2016 
// Section Wed 4-5pm     
// Lab number: 5
// Hardware connections
// Hardware Specifications
// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include <stdint.h>
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h" 


#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control
#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control

#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C))
#define GPIO_PORTA_AMSEL_R      (*((volatile uint32_t *)0x40004528))

#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile uint32_t *)0x40025528))

#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define PA72                    (*((volatile uint32_t *)0x400043F0))
#define PF31                    (*((volatile uint32_t *)0x40025028))
#define PF1                     (*((volatile uint32_t *)0x40025008)) 	
#define PF3 										(*((volatile uint32_t *)0x40025020)) 

				

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void cDelay(unsigned short count); 
extern void Delay(void);

// ***** 3. Subroutines Section *****
void cDelay(unsigned short count){
	while(count){
		Delay();
		count--;
	}
} 

// Linked data structure
struct State {
  unsigned long Out; 
  unsigned long Time;  
  unsigned long Next[8];
}; 
typedef const struct State STyp;
#define goN    	0
#define waitN1 	1
#define waitN2 	2
#define goE    	3
#define waitE1 	4
#define waitE2	5
#define walk1 	6
#define walk2  	7
#define on1    	8
#define on2    	9
#define on3	10
#define	on4	11
#define on5	12
#define on6	13
#define off1	14
#define	off2	15
#define	off3	16
#define off4	17
#define off5	18
#define	off6	19
STyp FSM[20] = {
 {0x4C,	20, {goN,waitN2,waitN1,waitN2,goN,waitN2,waitN1,waitN2}},  
 {0x4A, 10, {goE,walk2,goE,walk2,goE,walk2,goE,walk2}},						 
 {0x4A,	10, {walk2,walk2,walk2,walk2,walk2,walk2,walk2,walk2}},    
 {0x61, 20, {goE,waitE2,goE,waitE2,waitE1,waitE2,waitE1,waitE2}},  
 {0x51, 10, {goN,walk1,goN,walk1,goN,walk1,goN,walk1}},						 
 {0x51, 10, {walk1,walk1,walk1,walk1,walk1,walk1,walk1,walk1}},	   
 {0x89, 20, {on1,walk1,on1,walk1,on1,walk1,on1,walk1}},						 
 {0x89, 20, {on4,walk2,on4,walk2,on4,walk2,on4,walk2}},						 
 {0x89,  2, {off1,walk1,off1,walk1,off1,walk1,off1,walk1}},
 {0x89,  2, {off2,walk1,off2,walk1,off2,walk1,off2,walk1}},
 {0x89,	 2, {off3,walk1,off3,walk1,off3,walk1,off3,walk1}},
 {0x89,	 2, {off4,walk2,off4,walk2,off4,walk2,off4,walk2}},
 {0x89,	 2, {off5,walk2,off5,walk2,off5,walk2,off5,walk2}},
 {0x89,	 2, {off6,walk2,off6,walk2,off6,walk2,off6,walk2}},
 {0x49,	 2, {on2,walk1,on2,walk1,on2,walk1,on2,walk1}},
 {0x49,  2, {on3,walk1,on3,walk1,on3,walk1,on3,walk1}},
 {0x49,  2, {goN,walk1,goN,walk1,goN,walk1,goN,walk1}},
 {0x49,  2, {on5,walk2,on5,walk2,on5,walk2,on5,walk2}},
 {0x49,  2, {on6,walk2,on6,walk2,on6,walk2,on6,walk2}},
 {0x49,  2, {goE,walk2,goE,walk2,goE,walk2,goE,walk2}}
 };


unsigned long S;      // index to the current state 
unsigned long Input;
uint16_t delay;

int main(void){ 
	
	SysTick_Init();   
	
  	TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz
  
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x31;      			// 1) A E F
  	delay = SYSCTL_RCGC2_R; 
  
	//Turn on clock for Ports A, E, and F
	SYSCTL_RCGCGPIO_R 		|= 0x31;
	delay = SYSCTL_RCGCGPIO_R;
	
	//Initialize PortE
	GPIO_PORTE_DIR_R		&= ~0x07; 
	GPIO_PORTE_AFSEL_R		&= 0x00;
	GPIO_PORTE_DEN_R		|= 0x07;
	//GPIO_PORTE_AMSEL_R  &= 0x00;
	//Initialize PortA
	GPIO_PORTA_DIR_R  		|= 0xFC;
	GPIO_PORTA_AFSEL_R 		&= ~0xFC;
	GPIO_PORTA_DEN_R 		|= 0xFC;
	//GPIO_PORTA_AMSEL_R  &= 0x00;
	//Initialize PortF
	GPIO_PORTF_DIR_R 		|= 0x0A;
	GPIO_PORTF_AFSEL_R		&= ~0x0A;
	GPIO_PORTF_DEN_R 		|= 0x0A;  
	//GPIO_PORTF_AMSEL_R  &= 0x00;
  
	S = goN;  
  
	EnableInterrupts();
  while(1){
        PA72 = ((FSM[S].Out & 0x3F)<<2);              // Set Traffic LEDs
	PF3  = ((FSM[S].Out & 0x80)>>4);              // Set Walk LEDs 
	PF1  = ((FSM[S].Out & 0x40)>>5);              // Set Don't Walk LEDs
    	cDelay(FSM[S].Time);															// Implement State Delay
    	Input = (GPIO_PORTE_DATA_R & 0x07);               // Read Sensors
    	S = FSM[S].Next[Input];
  }
}

