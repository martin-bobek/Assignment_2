#include "Assignment_2.h"
#include <blackfin.h>

void Init_CoreTimer(){
	*pIMASK |= EVT_IVTMR;
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
}
