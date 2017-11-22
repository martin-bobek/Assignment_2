#include "Assignment_2.h"
#include <blackfin.h>

volatile bool synced = false;

void Init_CoreTimer(){
	*pTSCALE = 0;					// decrements counter once every clock cycle
	*pTPERIOD = 0x1000000;				// who the fuck knows how fast
	*pILAT = EVT_IVTMR;				// clears latch bit before enabling interrupt
	*pIMASK |= EVT_IVTMR;			// enables interrupt
	*pTCNTL = (TINT | TAUTORLD | TMPWR | TMREN);		// starts timer
}

#pragma interrupt
extern "C" void CoreTimer_ISR_CPP(void){
	static unsigned char counter = 0;

	if (counter == 0){
		counter = 10;
		printf("\n");
		UpdateSimulationDisplay();
	}
	else {
		CoffeePot_Sync();
	}

	counter--;
	synced = true;
}
