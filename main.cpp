#include <sys/platform.h>
#include "adi_initialize.h"
#include "Assignment_2.h"

#define POT1_SIZE			538
#define POT1_90_FULL		484
#define POT2_SIZE			519
#define POT2_90_FULL		467
#define POT3_SIZE			535
#define POT3_90_FULL		482
#define POT4_SIZE			583
#define POT4_90_FULL		525

int main(int argc, char *argv[])
{
	int numCoffeePots = 4;
	WHICHDISPLAY whichDisplay = (WHICHDISPLAY)(USE_TEXT_GUI | USE_CCES_GUI);

	printf("\nStarting coffee pot simulator.\n");
	Init_CoffeePotSimulation(numCoffeePots, whichDisplay);

	Init_LED_GPIOInterface();
	Write_LED_GPIOInterface(0);
	Init_Input_GPIOInterface();
	CoreTimer_EVT6_ASM();
	Init_CoreTimer();

	char coffeePot1_Name[] = "WDS";
	char coffeePot2_Name[] = "Martin";
	char coffeePot3_Name[] = "Taylor";
	char coffeePot4_Name[] = "Clara";

	printf("\nInitializing coffee pots.\n");
	COFFEEPOT_DEVICE *coffeePot1_BaseAddress = InitCoffeePot(COFFEEPOT1, coffeePot1_Name);
	COFFEEPOT_DEVICE *coffeePot2_BaseAddress = InitCoffeePot(COFFEEPOT2, coffeePot2_Name);
	COFFEEPOT_DEVICE *coffeePot3_BaseAddress = InitCoffeePot(COFFEEPOT3, coffeePot3_Name);
	COFFEEPOT_DEVICE *coffeePot4_BaseAddress = InitCoffeePot(COFFEEPOT4, coffeePot4_Name);

	// poll bit 4 until coffee pot finishes turning on.
	while(!IsReady(coffeePot1_BaseAddress) || !IsReady(coffeePot2_BaseAddress) || !IsReady(coffeePot3_BaseAddress) || !IsReady(coffeePot4_BaseAddress)){
		SwitchesIn(coffeePot4_BaseAddress);
		LedOut(coffeePot4_BaseAddress);
		WaitForCoreTimer();
	}

	ActivateCoffeePot(coffeePot1_BaseAddress);
	ActivateCoffeePot(coffeePot2_BaseAddress);
	ActivateCoffeePot(coffeePot3_BaseAddress);
	ActivateCoffeePot(coffeePot4_BaseAddress);

	printf("\nEntering coffee pot control loop.\n");
	unsigned short int waterLevelRequired1 = POT1_90_FULL;
	unsigned short int waterLevelRequired2 = POT2_90_FULL;
	unsigned short int waterLevelRequired3 = POT3_90_FULL;
	unsigned short int waterLevelRequired4 = POT4_90_FULL;
	unsigned short int temperatureRequired1 = 93;
	unsigned short int temperatureRequired2 = 92;
	unsigned short int temperatureRequired3 = 98;
	unsigned short int temperatureRequired4 = 100;

	while(true){
		SwitchesIn(coffeePot4_BaseAddress);
		LedOut(coffeePot4_BaseAddress);

		MyWaterControlCode_CPP(coffeePot1_BaseAddress, waterLevelRequired1, COFFEEPOT1);
		MyWaterControlCode_CPP(coffeePot2_BaseAddress, waterLevelRequired2, COFFEEPOT2);
		MyWaterControlCode_ASM(coffeePot3_BaseAddress, waterLevelRequired3, COFFEEPOT3);
#if MANUAL_CONTROL
		coffeePot4_BaseAddress->waterInFlowRegister = 100;
#else
		MyWaterControlCode_CPP(coffeePot4_BaseAddress, waterLevelRequired4, COFFEEPOT4);
#endif


		MyHeatControlCode_CPP(coffeePot1_BaseAddress, temperatureRequired1);
		MyHeatControlCode_CPP(coffeePot2_BaseAddress, temperatureRequired2);
		MyHeatControlCode_ASM(coffeePot3_BaseAddress, temperatureRequired3);
#if MANUAL_CONTROL
		coffeePot4_BaseAddress->heaterBoostRegister = 1;
		coffeePot4_BaseAddress->heaterRegister = 255;
#else
		MyHeatControlCode_CPP(coffeePot4_BaseAddress, temperatureRequired4);
#endif


		WaitForCoreTimer();
	}
}
