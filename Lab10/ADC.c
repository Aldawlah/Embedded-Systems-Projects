// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: Kaela Todd and Megan Cooper
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
//Turn on clock for port e
	 SYSCTL_RCGCGPIO_R |= 0x10;
//Wait for clock to stabilize
	while((SYSCTL_RCGCGPIO_R&0x10) == 0) {};
//Setup PE4 & PE3
	GPIO_PORTE_DEN_R &= ~0x3C;
	GPIO_PORTE_AFSEL_R |= 0x3C;
	GPIO_PORTE_AMSEL_R |= 0x3C;
	GPIO_PORTE_DIR_R &= ~0x3C;
//Initialize ADC
	volatile unsigned long delay;
	SYSCTL_RCGCADC_R |= 0x01;         // 6) activate ADC0 
  delay = SYSCTL_RCGCADC_R;         // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;         // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;         // extra time to stabilize
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R = 0x1809;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x6000;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE3 END3
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
void ADC_In(uint32_t data[2]){ 
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[0] = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[1] = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
	data[2] = ADC0_SSFIFO2_R&0xFFF;  // 3C) read third result
	data[3] = ADC0_SSFIFO2_R&0xFFF;  // 3D) read fourth result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}
//data[0] is PE4, data[1] is PE3, data[2] is PE5, data[3] is PE2

