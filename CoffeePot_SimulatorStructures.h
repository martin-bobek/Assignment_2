/*
 * CoffeePot_SimulatorStructures.h
 *
 *  Created on: Sep 6, 2016
 *      Author: smithmr
 */

#ifndef COFFEEPOT_SIMULATORSTRUCTURES_H_
#define COFFEEPOT_SIMULATORSTRUCTURES_H_

//  GENERAL CONTROL   (Bits 4 to 0)
#define INITandSTAYPOWEREDON_BIT   (1 << 0)      // BIT 0 -> 0x1 – Cause device to initialize (takes 10 seconds) and stay powered up
#define LED_DISPLAY_ENABLE_BIT   (1 << 1)      // BIT 1 -> 0x2 – LED Power on
#define WATER_ENABLE_BIT         (1 << 2)      // BIT 2 -> 0x4 – Water control power on
#define HEATER_ENABLE_BIT        (1 << 3)	   // BIT 3 -> 0x8 – Heater power on

#define DEVICE_READY_BIT_RO      (1 << 4)  	   // 0x10 – Device is ready after initialization  -- RO (READ ONLY)

// LED CONTROL FIELD  (Bits 15 to 12)  -- W1S
#define LED_CONTROL_FIELD_OFFSET 12
#define LED1_BIT       (1 << (LED_CONTROL_FIELD_OFFSET + 0))    // 0x1 – LED 1  -- BIT 12
#define LED2_BIT       (1 << (LED_CONTROL_FIELD_OFFSET + 1)) 	// 0x2 – LED 2  -- BIT 13
#define LED3_BIT       (1 << (LED_CONTROL_FIELD_OFFSET + 2)) 	// 0x4 – LED 3  -- BIT 14
#define LED4_BIT       (1 << (LED_CONTROL_FIELD_OFFSET + 3)) 	// 0x8 – LED 4  -- BIT 15

// DEVELOPER CHOICE OF HOW TO USE LEDS AS INDICTORS
#define USE_LED1_TO_SHOW_SYSTEM_POWEREDUP    		LED1_BIT
#define USE_LED2_TO_SHOW_HEATER_ENABLED      		LED2_BIT
#define USE_LED3_TO_SHOW_WATER_ENABLED       		LED3_BIT
#define USE_LED4_TO_SHOW_LED_DISPLAY_ENABLED	    LED4_BIT

#define COFFEEPOT_INSERTED   (1 << 10)		// BIT 10 -> 0x0800

typedef struct COFFEEPOT_DEVICE {  // Base address is different for each coffeepot -- set automatically when plugged in
       unsigned short int controlRegister;	    // A BIT pattern controlling switches 	-- OFFSET 0 bytes from plug-and-play base address
       unsigned short int TIMER_upper;		// Value from (0x0000 to 0xFFFF)			-- OFFSET 2 bytes from plug-and-play base address
       unsigned short int TIMER_lower;		// Value from (0x0000 to 0xFFFF)			-- OFFSET 4 bytes from plug-and-play base address
       // Combine to get unsigned int TIMER =  (TIMER_upper << 16) + TIMER_lower;
       unsigned char heaterRegister;	   // A VALUE (0 to 255, 0x00 --> 0xFF)			-- OFFSET 6 bytes from plug-and-play base address
       unsigned char waterInFlowRegister;	// A VALUE (0 to 255, 0x00 --> 0xFF)		-- OFFSET 7 bytes from plug-and-play base address
       unsigned char heaterBoostRegister;	// A VALUE (0 to 15, 0x0 --> 0x3)			-- OFFSET 8 bytes from plug-and-play base address
       // MUST HAVE heaterRegister and heaterBoostRegister set to at least 1 to get any heat
       // Heat applied is proportional to (heaterRegister * heaterBoostRegister)
}  COFFEEPOT_DEVICE_REGISTER_STRUCTURE;

#endif /* COFFEEPOT_SIMULATORSTRUCTURES_H_ */
