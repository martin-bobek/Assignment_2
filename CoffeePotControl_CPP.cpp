#include "Assignment_2.h"

void My_SimulatedOneSecondPassing(bool showDetail){
	static unsigned char counter = 5;
	if (showDetail){
		counter--;
		if (counter == 0){
			counter = 5;
			printf("\n");
			UpdateSimulationDisplay();
		}
		else
			CoffeePot_Sync();
	}
	else
		CoffeePot_Sync();
}


COFFEEPOT_DEVICE *InitCoffeePot(COFFEEPOT_ID potID, char coffeePotName[]){
	COFFEEPOT_DEVICE *coffeePot_BaseAddress = 0;
	coffeePot_BaseAddress = Add_CoffeePotToSystem_PlugAndPlay(potID, coffeePotName);

	// all control registers are initialized to a known state
	coffeePot_BaseAddress->controlRegister = 0;
	coffeePot_BaseAddress->waterInFlowRegister = 0;
	coffeePot_BaseAddress->heaterRegister = 0;
	coffeePot_BaseAddress->heaterBoostRegister = 0;

	// Turn on coffee pot
	coffeePot_BaseAddress->controlRegister |= INITandSTAYPOWEREDON_BIT;

	// poll bit 4 until coffee pot finishes turning on.
	while(!(coffeePot_BaseAddress->controlRegister & DEVICE_READY_BIT_RO)){
		My_SimulatedOneSecondPassing(true);
	}

	// enable led display, and turn on led1 to show coffee pot is finished turning on and led4 to show led display is enabled
	coffeePot_BaseAddress->controlRegister |= (USE_LED1_TO_SHOW_SYSTEM_POWEREDUP |
												LED_DISPLAY_ENABLE_BIT | USE_LED4_TO_SHOW_LED_DISPLAY_ENABLED |
												WATER_ENABLE_BIT | USE_LED3_TO_SHOW_WATER_ENABLED |
												HEATER_ENABLE_BIT | USE_LED2_TO_SHOW_HEATER_ENABLED);

	return coffeePot_BaseAddress;
}

unsigned short int previousLevel[4], previousAdded[4]; // automatically zero initialized

void MyWaterControlCode_CPP(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int waterLevelRequired, COFFEEPOT_ID potID){
	unsigned short int currentLevel = CurrentWaterLevel_CPP(coffeePot_BaseAddress);

	// calculates the amount of water evaporated in the previous step
	signed short int evaporatedWater = previousLevel[potID - 1] + previousAdded[potID - 1] - currentLevel;
	// adds the amount of water evaporated to the amount of water needed to reach the required level
	signed short int toAdd = waterLevelRequired + evaporatedWater - currentLevel;

	if (toAdd < 0) // ensures toAdd is not negative
		toAdd = 0;
	else if (toAdd > 100) // ensures toAdd is not bigger than the largest flowrate possible
		toAdd = 100;
	coffeePot_BaseAddress->waterInFlowRegister = (unsigned char)toAdd;

	previousLevel[potID - 1] = currentLevel; // records the current level and amount added, used for next iteration
	previousAdded[potID - 1] = toAdd;
}

void MyHeatControlCode_CPP(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int temperatureRequired){
	unsigned char boost;
	unsigned short int currentTemp = CurrentTemperature_CPP(coffeePot_BaseAddress);

	if (currentTemp < temperatureRequired - 10) // applies full heat
		boost = 3;
	else if(currentTemp <= temperatureRequired) // turns down heater
		boost = 2;
	else					// turns off heater if too high
		boost = 0;

	coffeePot_BaseAddress->heaterBoostRegister = boost;
	coffeePot_BaseAddress->heaterRegister = 255;			// maximum value
}
