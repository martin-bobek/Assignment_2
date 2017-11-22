/*
 * CoffeePotSimulatorFunctions2016.h
 *
 *  Created on: Sep 6, 2016
 *      Author: smithmr
 */

#ifndef COFFEEPOT_SIMULATORFUNCTIONS2016_H_
#define COFFEEPOT_SIMULATORFUNCTIONS2016_H_

#include "CoffeePot_SimulatorStructures.h"

enum WHICHDISPLAY {USE_TEXT_GUI = 1, USE_CCES_GUI = 2, USE_SPI_GUI = 4};  // Make all display modes available simultaneously
enum COFFEEPOT_ID {COFFEEPOT1 = 1, COFFEEPOT2, COFFEEPOT3, COFFEEPOT4};

void Init_CoffeePotSimulation(int numCoffeePots, WHICHDISPLAY whichDisplay);
void CoffeePot_Sync(void );
void UpdateSimulationDisplay(void);

COFFEEPOT_DEVICE *Add_CoffeePotToSystem_PlugAndPlay(COFFEEPOT_ID COFFEEPOT, char coffeePotName[]);

unsigned short int  ReadControlRegister_CPP(COFFEEPOT_DEVICE *baseAddress);
unsigned short int CurrentWaterLevel_CPP(COFFEEPOT_DEVICE *baseAddress);
unsigned short int CurrentTemperature_CPP(COFFEEPOT_DEVICE *baseAddress);
void Remove_CoffeePotFromSystem(COFFEEPOT_DEVICE *baseAddress);
void ShowDeviceRegisterValues(COFFEEPOT_DEVICE *baseAddress);

void NetworkTimingFudge_USE_CCES_GUI_Delay(void);

#endif /* COFFEEPOT_SIMULATORFUNCTIONS2016_H_ */
