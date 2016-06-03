// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/12/2016 
// Student namges: Kaela Todd and Megan Cooper
// Last modification date: 4/19/2016

#include <stdint.h>
// Declare state variables for FiFo
// size, buffer, put and get indexes
#define SIZE 6
uint8_t PutI;
uint8_t GetI;
char FIFO[SIZE];

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init() {
	PutI = GetI = 0;
}


// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {
	//Complete this routine
	if(((PutI + 1)%SIZE) == GetI){
		return 0;
	}
	FIFO[PutI] = data;
	PutI = (PutI + 1)%SIZE;
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(char *datapt)
{
	if(GetI == PutI){
		return 0;
	}
	*datapt = FIFO[GetI];
	GetI = (GetI + 1)%SIZE;
	return 1;

}



