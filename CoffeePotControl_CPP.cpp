#include "Assignment_2.h"

void WaitForCoreTimer(){
	while(!synced)
		idle();
	synced = false;
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

	return coffeePot_BaseAddress;
}

bool IsReady(COFFEEPOT_DEVICE *coffeePot_BaseAddress){
	return coffeePot_BaseAddress->controlRegister & DEVICE_READY_BIT_RO;
}

void ActivateCoffeePot(COFFEEPOT_DEVICE *coffeePot_BaseAddress){
	// enable led display, and turn on led1 to show coffee pot is finished turning on and led4 to show led display is enabled
	coffeePot_BaseAddress->controlRegister |= (USE_LED1_TO_SHOW_SYSTEM_POWEREDUP |
												LED_DISPLAY_ENABLE_BIT | USE_LED4_TO_SHOW_LED_DISPLAY_ENABLED |
												WATER_ENABLE_BIT | USE_LED3_TO_SHOW_WATER_ENABLED |
												HEATER_ENABLE_BIT | USE_LED2_TO_SHOW_HEATER_ENABLED);
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

#define LED_OFFSET 		12
#define GPIO_LED_MSK	0x0F

void LedOut(COFFEEPOT_DEVICE *coffeePot_BaseAddress){
	unsigned short int pot_status = coffeePot_BaseAddress->controlRegister;
	unsigned char gpio_leds = Read_LED_GPIOInterface() & ~GPIO_LED_MSK;
	if (pot_status & LED_DISPLAY_ENABLE_BIT){	// checks to see if led display is on, otherwise write 0 to gpio
		unsigned char led_status = coffeePot_BaseAddress->controlRegister >> LED_OFFSET; // note this automatically does the masking operation
		gpio_leds |= led_status;
	}
	Write_LED_GPIOInterface(gpio_leds);
}

#define SWITCH_MSK				0x0E
#define CONTROL_WRITE_MSK		0xE00E

void SwitchesIn(COFFEEPOT_DEVICE *coffeePot_BaseAddress){
	unsigned char switch_inputs = (Read_Input_GPIOInterface() >> 7) & SWITCH_MSK; 	// lines up the switch input with control register
	unsigned short int control_input = switch_inputs;
	if (switch_inputs & LED_DISPLAY_ENABLE_BIT)
		control_input |= USE_LED4_TO_SHOW_LED_DISPLAY_ENABLED;
	if (switch_inputs & WATER_ENABLE_BIT)
		control_input |= USE_LED3_TO_SHOW_WATER_ENABLED;
	if (switch_inputs & HEATER_ENABLE_BIT)
		control_input |= USE_LED2_TO_SHOW_HEATER_ENABLED;
	coffeePot_BaseAddress->controlRegister = control_input | (coffeePot_BaseAddress->controlRegister & ~CONTROL_WRITE_MSK);
}
