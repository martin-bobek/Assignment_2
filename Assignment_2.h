#ifndef MYCOFFEEPOTCONTROLFUNCTIONS_H_
#define MYCOFFEEPOTCONTROLFUNCTIONS_H_

#include <stdio.h>
#include "CoffeePot_SimulatorFunctions.h"

extern volatile bool synced;

void Init_CoreTimer();
extern "C" void CoreTimer_EVT6_ASM(void);
void WaitForCoreTimer();

void MyWaterControlCode_CPP(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int waterLevelRequired, COFFEEPOT_ID potID);
void MyHeatControlCode_CPP(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int temperatureRequired);

extern "C" void MyWaterControlCode_ASM(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int waterLevelRequired, COFFEEPOT_ID potID);
extern "C" void MyHeatControlCode_ASM(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int temperatureRequired);

COFFEEPOT_DEVICE *InitCoffeePot(COFFEEPOT_ID potID, char coffeePotName[]);

#endif /* MYCOFFEEPOTCONTROLFUNCTIONS_H_ */
