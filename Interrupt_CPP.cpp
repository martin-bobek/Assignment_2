#include "Assignment_2.h"
#include <blackfin.h>

volatile unsigned char synced = false;

void Init_CoreTimer(){
	*pTSCALE = 0;					// decrements counter once every clock cycle
	*pTPERIOD = 0x1000000;				//
	*pILAT = EVT_IVTMR;				// clears latch bit before enabling interrupt
	*pIMASK |= EVT_IVTMR;			// enables interrupt
	*pTCNTL = (TINT | TAUTORLD | TMPWR | TMREN);		// starts timer
}

#if MANUAL_CONTROL
#define COUNTER_RESET		1
#else
#define COUNTER_RESET		10
#endif

#pragma interrupt
extern "C" void CoreTimer_ISR_CPP(void){
	static unsigned char counter = 0;

	if (counter == 0){
		counter = COUNTER_RESET;
		UpdateSimulationDisplay();
	}
	else {
		CoffeePot_Sync();
	}

	counter--;
	synced = true;
}
