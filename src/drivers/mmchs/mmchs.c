/*
 * mmchs.c
 *
 *  Created on: 11.11.2011
 *      Author: Fox
 */
#include "mmchs.h"
#include "../../hal/omap35x.h"
#include "ottos/types.h"


typedef unsigned char       UINT8;	//
typedef unsigned int	    UINT32;
typedef UINT32				UINT;

#define MMC_REFERENCE_CLK (96000000)

UINT setClockSpeed() {
	UINT TransferRateValue = 100000;		//lowest value (for testing, can be up to 100.000.000 "maybe" :D)
	UINT TimeValue = 10;				//also lowest value -> highest possible value = 80
	UINT Frequency = 0;
	UINT ClockFrequencySelect;
	Frequency = TransferRateValue * TimeValue/10;
	return (MMC_REFERENCE_CLK/Frequency) + 1;
}

void mmchs_init() {
	//22.5.1.1 Enable Interface and Functional clock for MMC Controller
	//22.6.1.3.1.1 MMCHS Controller Interface and Functional Clocks Enabling
	*((mem_address_t) CM_ICLKEN1_CORE) |= 0x01000000;	//enables the interface clock for the MMCHS1 controller
	*((mem_address_t) CM_FCLKEN1_CORE) |= 0x01000000;	//enables the functional clock for the MMCHS1 module
	//MMCHS Controller Software Reset
	*(mem_address_t) MMCHS1_SYSCONFIG = 0x02;
	//TODO: Wait until MMCHS1.MMCHS_SYSSTATUS[0] RESETDONE turns 1.

	//MMCHS Controller Voltage Capabilities Initialization
	*(mem_address_t) MMCHS1_CAPA = 0x06000000;
	//*(mem_address_t) MMCHS1_CAPA = 0x06000000;

	//MMCHS Controller INIT Procedure Start
	*(mem_address_t) MMCHS1_CON |= 0x02;	//Sets MMCHS1.MMCHS_CON[1] INIT to 1
	*(mem_address_t) MMCHS1_CMD = 0x00;	//Sends dummy command
	*(mem_address_t) CONTROL_PADC = 0x100; //Set the INPUTENABLE bit to assure ONF_MMC1_CLK synchronous clock


	*(mem_address_t) MMCHS1_HCTL = 0x00000b00;		//Data bus width = 1, voltage =	1.8 V, MMC bus on
	*(mem_address_t) MMCHS1_SYSCONFIG = 0x00003C07;	//Card's clock enable and card's clock frequency divider.
	*(mem_address_t) MMCHS1_CON 	= 0x01;				//Set MMC bus mode to open drainSet MMC bus mode to open drain

}

//send CMD0	-> This command resets the MMC card
void sendCMD0() {
	*(mem_address_t) MMCHS1_CON = 0x01;				//MMC bus is still in open drain state for broadcast
	*(mem_address_t) MMCHS1_IE = 0x00040001;			//Enables CC and CEB events to occur
	*(mem_address_t) MMCHS1_ISE = 0x00040001;		//Enables CC and CEB interrupts to rise
	*(mem_address_t) MMCHS1_CMD = MMCHS_CMD_0;				//Sends CMD0 whose opcode is 0
}

//send CMD5	-> This command asks a SDIO card to send its operating conditions
void sendCMD5() {
	*(mem_address_t) MMCHS1_CON = 0x01;				//MMC bus is still in open drain state for broadcast.
	*(mem_address_t) MMCHS1_IE = 0x00050001;			//Enables CC, CTO and CEB events to occur.
	*(mem_address_t) MMCHS1_ISE = 0x00050001;		//Enables CC, CTO and CEB interrupts to rise
	*(mem_address_t) MMCHS1_CMD = MMCHS_CMD_5;		//Sends CMD5 whose opcode is 5 and response type is 48 bits
}

//send CMD8	-> This command...
void sendCMD8() {
	*(mem_address_t) MMCHS1_CON = 0x00000001;		//MMC bus is still in open drain state for broadcast.
	*(mem_address_t) MMCHS1_IE = 0x100f0001;		//Enables CERR, CIE, CCRC, CC, CTO and CEB events to occur
	*(mem_address_t) MMCHS1_ISE = 0x100f0001;		//1 Enables CERR, CIE, CCRC, CC, CTO and CEB interrupts to rise
	*(mem_address_t) MMCHS1_CMD = MMCHS_CMD_8;		//Sends CMD8 whose opcode is 8, response type is 48 bits with CICE and CCCE enabled.
}

//send CMD9	-> This command asks the card to send its csd register's content
void sendCMD9() {
	*(mem_address_t) MMCHS1_CON = 0x00;
	*(mem_address_t) MMCHS1_IE = 0x00070001;
	*(mem_address_t) MMCHS1_ISE = 0x00070001;
	*(mem_address_t) MMCHS1_CMD = MMCHS_CMD_9;		//Sends CMD9
	*(mem_address_t) MMCHS1_ARG = 0x00010000;		//MMCHS_ARG register carries MMC card's address. We	choose to assign address 1 any other 16 bit wide value is valid.
}

//send CMD55	-> This command asks the card to send its csd register's content
void sendCMD55() {
	*(mem_address_t) MMCHS1_CON = 0x00;
	*(mem_address_t) MMCHS1_IE = 0x100f0001;
	*(mem_address_t) MMCHS1_ISE = 0x100f0001;
	*(mem_address_t) MMCHS1_CMD = MMCHS_CMD_55;		//Sends CMD55
}

void cardIdent() {
	*(mem_address_t) MMCHS1_CON = 0x01;
	*(mem_address_t) MMCHS1_CMD = 0x00;		//wait 1ms;
	*(mem_address_t) MMCHS1_STAT = 0x01;
	*(mem_address_t) MMCHS1_CON = 0x00;
	*(mem_address_t) MMCHS1_STAT = 0xFFFFFFFF;
	//TODO: Change clock!!
	sendCMD0();
	sendCMD5();
	//TODO: Read Stat! -> 3164



}

