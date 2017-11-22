#include <sys/platform.h>
#include "adi_initialize.h"
#include "Assignment_2.h"

/** 
 * If you want to use command program arguments, then place them in the following string. 
 */
char __argv_string[] = "";

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

	char coffeePot1_Name[] = "WDS";
	char coffeePot2_Name[] = "Martin";
	char coffeePot3_Name[] = "Taylor";
	char coffeePot4_Name[] = "Clara";

	printf("\nInitializing coffee pots.\n");
	COFFEEPOT_DEVICE *coffeePot1_BaseAddress = InitCoffeePot(COFFEEPOT1, coffeePot1_Name);
	COFFEEPOT_DEVICE *coffeePot2_BaseAddress = InitCoffeePot(COFFEEPOT2, coffeePot2_Name);
	COFFEEPOT_DEVICE *coffeePot3_BaseAddress = InitCoffeePot(COFFEEPOT3, coffeePot3_Name);
	COFFEEPOT_DEVICE *coffeePot4_BaseAddress = InitCoffeePot(COFFEEPOT4, coffeePot4_Name);

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
		MyWaterControlCode_CPP(coffeePot1_BaseAddress, waterLevelRequired1, COFFEEPOT1);
		MyWaterControlCode_CPP(coffeePot2_BaseAddress, waterLevelRequired2, COFFEEPOT2);
		MyWaterControlCode_ASM(coffeePot3_BaseAddress, waterLevelRequired3, COFFEEPOT3);
		MyWaterControlCode_CPP(coffeePot4_BaseAddress, waterLevelRequired4, COFFEEPOT4);

		MyHeatControlCode_CPP(coffeePot1_BaseAddress, temperatureRequired1);
		MyHeatControlCode_CPP(coffeePot2_BaseAddress, temperatureRequired2);
		MyHeatControlCode_ASM(coffeePot3_BaseAddress, temperatureRequired3);
		MyHeatControlCode_CPP(coffeePot4_BaseAddress, temperatureRequired4);

		My_SimulatedOneSecondPassing(true);
	}
}
