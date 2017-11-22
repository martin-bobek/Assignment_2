	.section program
	
// void MyWaterControlCode_ASM(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int waterLevelRequired, COFFEEPOT_ID potID)
// coffeePot_BaseAddress -> R0		waterLevelRequired -> R1		potID -> R2
	.global _MyWaterControlCode_ASM;
_MyWaterControlCode_ASM:
	[--SP] = R4;
	[--SP] = R5;
	[--SP] = R6;
#define baseAddress_R4		R4
#define levelRequired_R5	R5
#define potID_R6			R6
	baseAddress_R4 = R0;
	levelRequired_R5 = R1;
	potID_R6 = R2;
	LINK 28;
	// unsigned short int currentLevel = CurrentWaterLevel_CPP(coffeePot_BaseAddress);
#define currentLevel_R0		R0
	.extern __Z21CurrentWaterLevel_CPPP16COFFEEPOT_DEVICE;	// baseAddress still in R0
	CALL __Z21CurrentWaterLevel_CPPP16COFFEEPOT_DEVICE; // currentWaterLevel returned into R0
	// signed short int evaporatedWater = previousLevel[potID - 1] + previousAdded[potID - 1] - currentLevel;
#define evaporated_R1		R1
#define previousLevel_P0	P0
	.extern _previousLevel;
	R2.L = LO(_previousLevel);
	R2.H = HI(_previousLevel);
	R2 = R2 + potID_R6;
	R2 = R2 + potID_R6;
	previousLevel_P0 = R2;
	evaporated_R1 = W[previousLevel_P0 - 2] (Z);
#define previousAdded_P1	P1
	.extern _previousAdded;
	R2.L = LO(_previousAdded);
	R2.H = HI(_previousAdded);
	R2 = R2 + potID_R6;
	R2 = R2 + potID_R6;
	previousAdded_P1 = R2;
	R2 = W[previousAdded_P1 - 2] (Z);
	evaporated_R1 = evaporated_R1 + R2;
	evaporated_R1 = evaporated_R1 - currentLevel_R0;
	// signed short int toAdd = waterLevelRequired + evaporatedWater - currentLevel;
#define toAdd_R1			R1
	toAdd_R1 = evaporated_R1 + levelRequired_R5;
	toAdd_R1 = toAdd_R1 - currentLevel_R0;
	// if (toAdd < 0)
	R2 = 0;
	CC = toAdd_R1 < R2;
	IF !CC JUMP ELSEIF1;
	// toAdd = 0;
	toAdd_R1 = 0;
	JUMP ENDIF1;
ELSEIF1:
	// else if (toAdd > 100)
	R2 = 100;
	CC = R2 < toAdd_R1;
	IF !CC JUMP ENDIF1;
	// toAdd = 100;
	toAdd_R1 = 100;
ENDIF1:
#define baseAddress_P2		P2
	// coffeePot_BaseAddress->waterInFlowRegister = (unsigned char)toAdd;
	baseAddress_P2 = baseAddress_R4;
	B[baseAddress_P2 + 7] = toAdd_R1;
	// previousLevel[potID - 1] = currentLevel;
	W[previousLevel_P0 - 2] = currentLevel_R0;
	// previousAdded[potID - 1] = toAdd;
	W[previousAdded_P1 - 2] = toAdd_R1;
	SSYNC;
	UNLINK;
	R6 = [SP++];
	R5 = [SP++];
	R4 = [SP++];
_MyWaterControlCode_ASM.END:
	RTS;



// void MyHeatControlCode_ASM(COFFEEPOT_DEVICE *coffeePot_BaseAddress, unsigned short int temperatureRequired);
// coffeePot_BaseAddress -> R0		temperatureRequired -> R1
	.global _MyHeatControlCode_ASM
_MyHeatControlCode_ASM:
	[--SP] = R4;
	[--SP] = R5;
#define baseAddress_R4			R4
#define tempRequired_R5 		R5
	baseAddress_R4 = R0;
	tempRequired_R5 = R1;
	LINK 20;
	// unsigned short int currentTemp = CurrentTemperature_CPP(coffeePot_BaseAddress);
#define currentTemp_R0			R0
	.extern __Z22CurrentTemperature_CPPP16COFFEEPOT_DEVICE; // note baseAddress still in R0
	CALL __Z22CurrentTemperature_CPPP16COFFEEPOT_DEVICE;	// currentTemp returned to R0
	// unsigned char boost;
#define boost_R1				R1
	// if (currentTemp < temperatureRequired - 10)
	R2 = 10;
	R2 = tempRequired_R5 - R2;
	CC = currentTemp_R0 < R2 (IU);
	IF !CC JUMP ELSEIF2;
	// boost = 3;
	boost_R1 = 3;
	JUMP ENDIF2;
ELSEIF2:
	// else if(currentTemp <= temperatureRequired)
	CC = currentTemp_R0 <= tempRequired_R5 (IU);
	IF !CC JUMP ELSE2;
	// boost = 2;
	boost_R1 = 2;
	JUMP ENDIF2;
ELSE2:	// else boost = 0;
	boost_R1 = 0;
ENDIF2:
#define baseAddress_P0			P0
	// coffeePot_BaseAddress->heaterBoostRegister = boost;
	baseAddress_P0 = baseAddress_R4;
	B[baseAddress_P0 + 8] = boost_R1;
	// coffeePot_BaseAddress->heaterRegister = 255;
	R2 = 255;
	B[baseAddress_P0 + 6] = R2;
	SSYNC;
	UNLINK;
	R5 = [SP++];
	R4 = [SP++];
_MyHeatControlCode_ASM.END:
	RTS;