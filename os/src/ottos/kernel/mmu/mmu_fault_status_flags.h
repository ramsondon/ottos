/* fault_status_flags.h
 * 
 * Copyright (c) 2011 The ottos project.
 *
 * This work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This work is distributed in the hope that it will be useful, but without
 * any warranty; without even the implied warranty of merchantability or
 * fitness for a particular purpose. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 *  Created on: 5 Jan 2012
 *      Author: Thomas Bargetz <thomas.bargetz@gmail.com>
 */

#ifndef MMU_FAULT_STATUS_FLAGS_H_
#define MMU_FAULT_STATUS_FLAGS_H_

#define MMU_ALIGNMENT_FAULT 0x01 //0b000001
#define MMU_INSTRUCTION_CACHE_MAINTENANCE_FAULT 0x04 //0b000100
#define MMU_L1_TRANSLATION_PRECISE_EXTERNAL_ABORT_AXI_DECODE 0x0C //0b001100
#define MMU_L1_TRANSLATION_PRECISE_EXTERNAL_ABORT_AXI_SLAVE 0x2C //0b101100
#define MMU_L2_TRANSLATION_PRECISE_EXTERNAL_ABORT_AXI_DECODE 0x0E //0b001110
#define MMU_L2_TRANSLATION_PRECISE_EXTERNAL_ABORT_AXI_SLAVE 0x2E //0b101110
#define MMU_L1_TRANSLATION_PRECISE_PARITY_ERROR 0x1C //0b011100
#define MMU_L2_TRANSLATION_PRECISE_PARITY_ERROR 0x1E //0b011110
#define MMU_TRANSLATION_FAULT_SECTION 0x05 //0b000101
#define MMU_TRANSLATION_FAULT_PAGE 0x07 //0b000111
#define MMU_ACCESS_FLAG_FAULT_SECTION 0x03 //0b000011
#define MMU_ACCESS_FLAG_FAULT_PAGE 0x06 //0b000110
#define MMU_DOMAIN_FAULT_SECTION 0x09 //0b001001
#define MMU_DOMAIN_FAULT_PAGE 0x0B //0b001011
#define MMU_PERMISSION_FAULT_SECTION 0x0D //0b001101
#define MMU_PERMISSION_FAULT_PAGE 0x0F //0b001111
#define MMU_PRECISE_EXTERNAL_ABORT_NONTRANSLATION_AXI_DECODE 0x08 //0b001000
#define MMU_PRECISE_EXTERNAL_ABORT_NONTRANSLATION_AXI_SLAVE 0x28 //0b101000
#define MMU_IMPRECISE_EXTERNAL_ABORT_AXI_DECODE 0x16 //0b010110
#define MMU_IMPRECISE_EXTERNAL_ABORT_AXI_SLAVE 0x36 //0b110110
#define MMU_IMPRECISE_ERROR_PARITY_OR_ECC 0x18 //0b011000
#define MMU_DEBUG_EVENT 0x02 //0b000010
#endif /* MMU_FAULT_STATUS_FLAGS_H_ */
