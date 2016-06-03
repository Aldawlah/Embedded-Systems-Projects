// UART.c
// Runs on LM4F120/TM4C123
// Provide functions that setup and interact with UART
// Last Modified: 4/12/2016 
// Student names: Kaela Todd and Megan Cooper 


#include <stdint.h>
#include "tm4c123gh6pm.h"

// UART initialization function 
// Input: none
// Output: none
void UART_Init(void){ 
	volatile uint32_t delay;
	SYSCTL_RCGCUART_R |= 0x0002;	//activate UART1
	delay = SYSCTL_RCGCUART_R;
	SYSCTL_RCGCGPIO_R |= 0x0004; 	//activate port A
	delay = SYSCTL_RCGCGPIO_R;
	UART1_CTL_R &= ~0x0001; 			//disable UART
	UART1_IBRD_R = 50; 						//IBRD = int(80000000/(16*100000))=int(50.000) *Baud rate = Baud16/16 = (Bus clock frequency)/(16*divider)*
	UART1_FBRD_R = 0; 						//
	UART1_LCRH_R = 0x0070;				//8-bit word length, enable FIFO
	UART1_CTL_R |= 0x0301;				//Enable UART		
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; //UART &xFF00FFFF + 0x00220000
	GPIO_PORTC_AMSEL_R &= ~0x30;	//disable analog function on PC4-5
	GPIO_PORTC_AFSEL_R |= 0x30;		//enable alternate function on PC4-5
	GPIO_PORTC_DEN_R |= 0x30;			//enable digital I/O on PC4-5
	UART1_IM_R |= 0x10;
  UART1_IFLS_R = (UART1_IFLS_R&(~0x38))+ 0x10;
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF0FFFFF)+0x00600000; // prio 3
	NVIC_EN0_R |= 0x40;
}
//--How to find the baud rate:--
//Baud rate =  (80 MHz)/(16* (m+n/64)) = (80 MHz)/(16* (260+27/64)) = 19199.616 bits/sec
//^^Just an example	

//------------UART_InChar------------
// Wait for new input,
// then return ASCII code
// Input: none
// Output: char read from UART
// *** Need not be busy-wait if you know when to call
char UART_InChar(void){  
	while((UART1_FR_R & 0x0010) != 0) {}; //Wait until RXFE = 0
	return ((unsigned char)(UART1_DR_R&0xFF));
}

//------------UART_OutChar------------
// Wait for new input,
// then return ASCII code
// Input: none
// Output: char read from UART
void UART_OutChar(char data){  
		while((UART1_FR_R & 0x0020) != 0){}; //Wait until TXFF = 0
		UART1_DR_R = data;
}
