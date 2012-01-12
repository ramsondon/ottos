/*
 * prcm.h
 *
 *  Created on: Jan 12, 2012
 *      Author: stefan
 */

#ifndef PRCM_H_
#define PRCM_H_

// CM Module Registers
#define PER_CM_BASE		0x48005000

// Functional/Interface Clock Offsets
#define CM_FCLKEN_PER	0x00000000
#define CM_ICLKEN_PER	0x00000010

// McBSP2 functional/interface clock control
#define EN_MCBSP2		0x0

#endif /* PRCM_H_ */
