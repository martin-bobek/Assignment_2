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
	