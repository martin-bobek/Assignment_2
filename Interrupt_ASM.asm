#include <blackfin.h>

	.extern _CoreTimer_ISR_CPP;

	.section program;
	.global _CoreTimer_EVT6_ASM;
_CoreTimer_EVT6_ASM:
	p0.L = LO(EVT6);
	p0.H = HI(EVT6);
	R0.L = LO(_CoreTimer_ISR_CPP);
	R0.H = HI(_CoreTimer_ISR_CPP);
	[p0] = R0;
	SSYNC;
_CoreTimer_EVT6_ASM.END:
	RTS;

	.section L1_DATA;
	.byte counter = 0;
	
	.extern _UpdateSimulationDisplay;
	.extern _CoffeePot_Sync;
	
	.section program;
	.global _CoreTimer_ISR_ASM;
_CoreTimer_ISR_ASM:
	[--SP] = ASTAT;
	[--SP] = P0;
	[--SP] = P1;
	[--SP] = P2;
	[--SP] = P3;
	[--SP] = R0;
	[--SP] = R1;
	[--SP] = R2;
	[--SP] = R3;
	[--SP] = R4;
	[--SP] = A0.w;
	[--SP] = A0.x;
	[--SP] = A1.w;
	[--SP] = A1.x;
	[--SP] = L0;
	[--SP] = L1;
	[--SP] = L2;
	[--SP] = L3;
	[--SP] = I0;
	[--SP] = I1;
	[--SP] = I2;
	[--SP] = I3;
	[--SP] = B0;
	[--SP] = B1;
	[--SP] = B2;
	[--SP] = B3;
	[--SP] = LC0;
	[--SP] = LC1;
	[--SP] = LT0;
	[--SP] = LT1;
	[--SP] = LB0;
	[--SP] = LB1;
	LINK 16;
	
	P3.L = LO(counter);
	P3.H = HI(counter);
	R4 = [P4];
	CC = R4 == 0;
	IF !CC JUMP ELSE;
	R4 = 10;
	CALL _UpdateSimulationDisplay;
	JUMP ENDIF;
ELSE:
	CALL _CoffeePot_Sync;
ENDIF:
	[P3] = R4;
	
	UNLINK;
	[--SP] = LB1;
	[--SP] = LB0;
	[--SP] = LT1;
	[--SP] = LT0;
	[--SP] = LC1;
	[--SP] = LC0;
	[--SP] = B3;
	[--SP] = B2;
	[--SP] = B1;
	[--SP] = B0;
	[--SP] = I3;
	[--SP] = I2;
	[--SP] = I1;
	[--SP] = I0;
	[--SP] = L3;
	[--SP] = L2;
	[--SP] = L1;
	[--SP] = L0;
	[--SP] = A1.x;
	[--SP] = A1.w;
	[--SP] = A0.x;
	[--SP] = A0.w;
	[--SP] = R4;
	[--SP] = R3;
	[--SP] = R2;
	[--SP] = R1;
	[--SP] = R0;
	[--SP] = P3;
	[--SP] = P2;
	[--SP] = P1;
	[--SP] = P0;
	[--SP] = ASTAT;
_CoreTimer_ISR_ASM.END:
	RTI;
	