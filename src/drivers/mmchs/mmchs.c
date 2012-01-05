/* mmchs.c
 * 
 * Copyright (c) 2011 The ottos project.
 *
 * this work is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * this work is distributed in the hope that it will be useful, but without
 * any warranty; without even the implied warranty of merchantability or
 * fitness for a particular purpose. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 *  Created on: 24.11.2011
 *      Author: Franziskus Domig <fdomig@gmail.com>
 */

#include "mmchs.h"

#include <stdlib.h>
#include <ottos/io.h>
#include <ottos/memory.h>
#include <ottos/kernel.h>
#include <ottos/types.h>
#include <ottos/dev/device.h>
#include <ottos/dev/block.h>

#include "../../hal/mmchs.h"

static MMCHS_CARD_INFO mmchs_card_info;

static BLOCK_IO_MEDIA mmchs_media = {
  SIGNATURE_32('s', 'd', 'i', 'o'), // media ID
  TRUE,                             // removable media
  FALSE,                            // media present
  FALSE,                            // logical partition
  FALSE,                            // read only
  FALSE,                            // write caching
  512,                              // block size
  4,                                // IO align
  0                                 // last block
};

static BOOLEAN mmchs_media_change = TRUE;

// forward declaration
static MMCHS_STATUS mmchs_init();

static void mmchs_debug(uint32_t code, const char* message) {
  kernel_debug(code, message);
}

static void mmchs_error(uint32_t error_code, const char* message) {
  kernel_error(error_code, message);
}

static int mmchs_has_error(MMCHS_STATUS status) {
  return (status != 0);
}

static void mmchs_parse_card_cid_data(uint32_t r0, uint32_t r1, uint32_t r2,
                                      uint32_t r3) {

  mmchs_card_info.CID_data.MDT = ((r0 >> 8) & 0xFFF);
  mmchs_card_info.CID_data.PSN = (((r0 >> 24) & 0xFF) | ((r1 & 0xFFFFFF) << 8));
  mmchs_card_info.CID_data.PRV = ((r1 >> 24) & 0xFF);
  mmchs_card_info.CID_data.PNM[4] = ((r2) & 0xFF);
  mmchs_card_info.CID_data.PNM[3] = ((r2 >> 8) & 0xFF);
  mmchs_card_info.CID_data.PNM[2] = ((r2 >> 16) & 0xFF);
  mmchs_card_info.CID_data.PNM[1] = ((r2 >> 24) & 0xFF);
  mmchs_card_info.CID_data.PNM[0] = ((r3) & 0xFF);
  mmchs_card_info.CID_data.OID = ((r3 >> 8) & 0xFFFF);
  mmchs_card_info.CID_data.MID = ((r3 >> 24) & 0xFF);
}

static MMCHS_STATUS mmchs_send_cmd(uint32_t cmd, uint32_t cmd_int_en,
                                   uint32_t arg) {
  uint32_t mmc_status;
  uint32_t retry_count = 0;

  // check if command line is in use; poll until command line is available
  while ((MMIO_READ32(MMCHS_PSTATE) & DATI_MASK) == DATI_NOT_ALLOWED)
    ;

  // provide the block size
  MMIO_WRITE32(MMCHS_BLK, BLEN_512BYTES);

  // setting data timeout counter to max
  MMIO_AND_THEN_OR32(MMCHS_SYSCTL, ~DTO_MASK, DTO_VAL);

  // clear status register
  MMIO_WRITE32(MMCHS_STAT, 0xFFFFFFFF);

  // set command argument register
  MMIO_WRITE32(MMCHS_ARG, arg);

  // enable interrupt enable events to occur
  MMIO_WRITE32(MMCHS_IE, cmd_int_en);

  // enable interrupt signals to occur
  // TODO: do we need this?! MMCHS_IE should be enough for now
  // MMIO_WRITE32(MMCHS_ISE, cmd_int_en);

  // send command
  MMIO_WRITE32(MMCHS_CMD, cmd);

  // check for the command status
  while (retry_count < MMCHS_MAX_RETRY_COUNT) {
    do {
      mmc_status = MMIO_READ32(MMCHS_STAT);
    } while (mmc_status == 0);

    // read status of command response
    if ((mmc_status & ERRI) != 0) {

      // perform soft reset for mmci_cmd line
      MMIO_OR32(MMCHS_SYSCTL, SRC);
      while ((MMIO_READ32(MMCHS_SYSCTL) & SRC))
        ;

      // FIXME
      mmchs_error(MMCHS_ERROR_DEVICE, "MMCHS could not send command\n");
      return MMCHS_ERROR_DEVICE;
    }

    // check if command is completed
    if ((mmc_status & CC) == CC) {
      MMIO_WRITE32(MMCHS_STAT, CC);
      break;
    }

    retry_count++;
  }

  if (retry_count == MMCHS_MAX_RETRY_COUNT) {
    mmchs_error(MMCHS_STATUS_TIMEOUT, "MMCHS send command timeout\n");
    return MMCHS_STATUS_TIMEOUT;
  }

  return MMCHS_STATUS_SUCCESS;
}

static void mmchs_update_clk_freq(uint32_t new_clkd) {
  // set clock enable to 0x0 to not provide the clock to the card
  MMIO_AND32(MMCHS_SYSCTL, ~CEN);

  // set new clock frequency
  MMIO_AND_THEN_OR32(MMCHS_SYSCTL, ~CLKD_MASK, new_clkd << 6);

  // poll until internal clock is stable
  while ((MMIO_READ32(MMCHS_SYSCTL) & ICS_MASK) != ICS)
    ;

  // set clock enable to 0x1 to provide the clock to the card
  MMIO_OR32(MMCHS_SYSCTL, CEN);
}

static MMCHS_STATUS mmchs_perform_card_identification() {
  MMCHS_STATUS status;
  uint32_t cmd_argument = 0;
  uint32_t response = 0;
  uint32_t retry_count = 0;
  uint8_t SD_cmd8_supported = FALSE;

  //Enable interrupts.
  MMIO_WRITE32(MMCHS_IE, (BADA_EN | CERR_EN | DEB_EN | DCRC_EN | DTO_EN
      | CIE_EN | CEB_EN | CCRC_EN | CTO_EN | BRR_EN | BWR_EN | TC_EN | CC_EN));

  //Controller INIT procedure start.
  MMIO_OR32(MMCHS_CON, INIT);
  MMIO_WRITE32(MMCHS_CMD, 0x00000000);
  while (!(MMIO_READ32(MMCHS_STAT) & CC))
    ;

  //Wait for 1ms
  kernel_sleep(1);

  //Set CC bit to 0x1 to clear the flag
  MMIO_OR32(MMCHS_STAT, CC);
  //Retry INIT procedure.
  MMIO_WRITE32(MMCHS_CMD, 0x00000000);
  while (!(MMIO_READ32(MMCHS_STAT) & CC))
    ;

  //End initialization sequence
  MMIO_AND32(MMCHS_CON, ~INIT);
  MMIO_OR32(MMCHS_HCTL, (SDVS_3_0_V | DTW_1_BIT | SDBP_ON));

  //Change clock frequency to 400KHz to fit protocol
  mmchs_update_clk_freq(CLKD_400KHZ);
  MMIO_OR32(MMCHS_CON, OD);

  //Send CMD0 command.
  status = mmchs_send_cmd(CMD0, CMD0_INT_EN, cmd_argument);

  if (mmchs_has_error(status)) {
    mmchs_error(MMCHS_ERROR_DEVICE, "cmd0 fails.\n");
    return status;
  }

  {
    char buffer[1024];
    sprintf(buffer, "CMD0 response: %x\n", MMIO_READ32(MMCHS_RSP10));
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);
  }

  //Send CMD5 command.
  status = mmchs_send_cmd(CMD5, CMD5_INT_EN, cmd_argument);

  if (status == MMCHS_STATUS_SUCCESS) {
    char buffer[1024];
    sprintf(buffer, "CMD5 response: %x\n", MMIO_READ32(MMCHS_RSP10));

    mmchs_debug(MMCHS_DEBUG_INFO,
                "CMD5 Success. SDIO card. Follow SDIO card specification.\n");
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);

    //NOTE: Returning unsupported error for now. Need to implement SDIO specification.
    return MMCHS_ERROR_UNSUPPORTED;

  } else {
    mmchs_debug(MMCHS_DEBUG_INFO, "CMD5 fails. Not an SDIO card.\n");
  }

  MMIO_OR32(MMCHS_SYSCTL, SRC);

  kernel_sleep(1);

  while ((MMIO_READ32(MMCHS_SYSCTL) & SRC))
    ;

  //Send CMD8 command. (New v2.00 command for Voltage check)
  //Only 2.7V - 3.6V is supported for SD2.0, only SD 2.0 card can pass.
  //MMC & SD1.1 card will fail this command.
  cmd_argument = CMD8_ARG;
  status = mmchs_send_cmd(CMD8, CMD8_INT_EN, cmd_argument);

  if (status == MMCHS_STATUS_SUCCESS) {
    char buffer[1024];
    response = MMIO_READ32(MMCHS_RSP10);
    sprintf(buffer, "CMD8 success. CMD8 response: %x\n", response);
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);

    if (response != cmd_argument) {
      return MMCHS_ERROR_DEVICE;
    }
    mmchs_debug(MMCHS_DEBUG_INFO, "Card is SD2.0\n");
    SD_cmd8_supported = TRUE; //Supports high capacity.
  } else {
    mmchs_debug(MMCHS_DEBUG_INFO, "CMD8 fails. Not an SD2.0 card.\n");
  }

  MMIO_OR32(MMCHS_SYSCTL, SRC);
  kernel_sleep(1);

  while ((MMIO_READ32(MMCHS_SYSCTL) & SRC))
    ;

  //Poll till card is busy
  while (retry_count < MMCHS_MAX_RETRY_COUNT) {
    //Send CMD55 command.
    cmd_argument = 0;
    status = mmchs_send_cmd(CMD55, CMD55_INT_EN, cmd_argument);
    if (status == MMCHS_STATUS_SUCCESS) {
      char buffer[1024];
      sprintf(buffer, "CMD55 success. CMD55 response: %x\n",
              MMIO_READ32(MMCHS_RSP10));
      mmchs_debug(MMCHS_DEBUG_INFO, buffer);

      mmchs_card_info.card_type = SD_CARD;

    } else {
      mmchs_debug(MMCHS_DEBUG_INFO, "CMD55 fails.\n");
      mmchs_card_info.card_type = MMC_CARD;
    }

    //Send appropriate command for the card type which got detected.
    if (mmchs_card_info.card_type == SD_CARD) {
      cmd_argument = ((uint32_t *) &(mmchs_card_info.OCR_data))[0];

      //Set HCS bit.
      if (SD_cmd8_supported) {
        cmd_argument |= (uint32_t) MMCHS_HCS;
      }
      status = mmchs_send_cmd(ACMD41, ACMD41_INT_EN, cmd_argument);
      if (mmchs_has_error(status)) {
        mmchs_debug(MMCHS_DEBUG_INFO, "ACMD41 fails.\n");
        return status;
      }

      ((uint32_t *) &(mmchs_card_info.OCR_data))[0] = MMIO_READ32(MMCHS_RSP10);
      {
        char buffer[1024];
        sprintf(buffer, "SD card detected. ACMD41 OCR: %x\n",
                ((uint32_t *) &(mmchs_card_info.OCR_data))[0]);
        mmchs_debug(MMCHS_DEBUG_INFO, buffer);
      }

    } else if (mmchs_card_info.card_type == MMC_CARD) {
      cmd_argument = 0;
      status = mmchs_send_cmd(CMD1, CMD1_INT_EN, cmd_argument);
      if (mmchs_has_error(status)) {
        mmchs_debug(MMCHS_DEBUG_INFO, "CMD1 fails.\n");
        return status;
      }
      response = MMIO_READ32(MMCHS_RSP10);
      {
        char buffer[1024];
        sprintf(buffer, "MMC card detected. CMD1 response: %x\n", response);
        mmchs_debug(MMCHS_DEBUG_INFO, buffer);
      }
      //NOTE: For now, I am skipping this since I only have an SD card.
      //Compare card OCR and host OCR (Section 22.6.1.3.2.4)
      return MMCHS_ERROR_UNSUPPORTED; //For now, MMC is not supported.
    }

    //Poll the card until it is out of its power-up sequence.
    if (mmchs_card_info.OCR_data.Busy == 1) {
      if (SD_cmd8_supported) {
        mmchs_card_info.card_type = SD_CARD_2;
      }
      //Card is ready. Check CCS (Card capacity status) bit (bit#30).
      //SD 2.0 standard card will response with CCS 0, SD high capacity card will respond with CCS 1.
      if (mmchs_card_info.OCR_data.AccessMode & BIT1) {
        mmchs_card_info.card_type = SD_CARD_2_HIGH;
        mmchs_debug(MMCHS_DEBUG_INFO, "High capacity card.\n");
      } else {
        mmchs_debug(MMCHS_DEBUG_INFO, "Standard capacity card.\n");
      }
      break;
    }
    kernel_sleep(1);

    retry_count++;
  }
  if (retry_count == MMCHS_MAX_RETRY_COUNT) {
    char buffer[1024];
    sprintf(buffer, "Timeout error. retry_count: %d\n", retry_count);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return MMCHS_STATUS_TIMEOUT;
  }
  //Read CID data.
  cmd_argument = 0;
  status = mmchs_send_cmd(CMD2, CMD2_INT_EN, cmd_argument);
  if (mmchs_has_error(status)) {
    char buffer[1024];
    sprintf(buffer, "CMD2 fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return status;
  }

  {
    char buffer[1024];
    sprintf(buffer, "CMD2 response: %x %x %x %x\n", MMIO_READ32(MMCHS_RSP10),
            MMIO_READ32(MMCHS_RSP32), MMIO_READ32(MMCHS_RSP54),
            MMIO_READ32(MMCHS_RSP76));
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);
  }

  //Parse CID register data.
  mmchs_parse_card_cid_data(MMIO_READ32(MMCHS_RSP10), MMIO_READ32(MMCHS_RSP32),
                            MMIO_READ32(MMCHS_RSP54), MMIO_READ32(MMCHS_RSP76));
  //Read RCA
  cmd_argument = 0;
  status = mmchs_send_cmd(CMD3, CMD3_INT_EN, cmd_argument);
  if (mmchs_has_error(status)) {
    char buffer[1024];
    sprintf(buffer, "CMD3 fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return status;
  }
  //Set RCA for the detected card. RCA is CMD3 response.
  mmchs_card_info.RCA = (MMIO_READ32(MMCHS_RSP10) >> 16);
  {
    char buffer[1024];
    sprintf(buffer, "CMD3 response: RCA %x\n", mmchs_card_info.RCA);
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);
  }

  //MMC Bus setting change after card identification.
  MMIO_AND32(MMCHS_CON, ~OD);
  MMIO_OR32(MMCHS_HCTL, SDVS_3_0_V);
  mmchs_update_clk_freq(CLKD_400KHZ); //Set the clock frequency to 400KHz.

  return MMCHS_STATUS_SUCCESS;
}

static void mmchs_block_information(uint32_t *block_size, uint32_t *num_blocks) {
  MMCHS_CSD_SDV2 *csd_SDV2_data;
  uint32_t card_size;

  if (mmchs_card_info.card_type == SD_CARD_2_HIGH) {
    csd_SDV2_data = (MMCHS_CSD_SDV2 *) &mmchs_card_info.CSD_data;

    // Populate block_size.
    *block_size = (0x1UL << csd_SDV2_data->READ_BL_LEN);

    // Calculate Total number of blocks.
    card_size = csd_SDV2_data->C_SIZELow16 | (csd_SDV2_data->C_SIZEHigh6 << 2);
    *num_blocks = ((card_size + 1) * 1024);

  } else {
    // Populate block_size.
    *block_size = (0x1UL << mmchs_card_info.CSD_data.READ_BL_LEN);

    // Calculate Total number of blocks.
    card_size = mmchs_card_info.CSD_data.C_SIZELow2
        | (mmchs_card_info.CSD_data.C_SIZEHigh10 << 2);
    *num_blocks = (card_size + 1) * (1 << (mmchs_card_info.CSD_data.C_SIZE_MULT
        + 2));
  }

  //For >=2G card, block_size may be 1K, but the transfer size is 512 bytes.
  if (*block_size > 512) {
    // TODO: we do not support cards > 2gb yet
    // *num_blocks = MULT_U64x32(*num_blocks, *block_size/2);
    *block_size = 512;
  }

  {
    char buffer[1024];
    sprintf(buffer, "Card type: %x, block_size: %x, num_blocks: %x\n",
            mmchs_card_info.card_type, *block_size, *num_blocks);
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);
  }
}

static void mmchs_calculate_card_CLKD(uint32_t *clock_frequency_select) {
  uint8_t max_data_transfer_rate;
  uint32_t transfer_rate_value = 0;
  uint32_t time_value = 0;
  uint32_t frequency = 0;

  max_data_transfer_rate = mmchs_card_info.CSD_data.TRAN_SPEED;

  // For SD Cards  we would need to send CMD6 to set
  // speeds abouve 25MHz. High Speed mode 50 MHz and up

  //Calculate Transfer rate unit (Bits 2:0 of TRAN_SPEED)
  switch (max_data_transfer_rate & 0x7) {
    case 0:
      transfer_rate_value = 100 * 1000;
      break;

    case 1:
      transfer_rate_value = 1 * 1000 * 1000;
      break;

    case 2:
      transfer_rate_value = 10 * 1000 * 1000;
      break;

    case 3:
      transfer_rate_value = 100 * 1000 * 1000;
      break;

    default:
      mmchs_error(MMCHS_ERROR_INVALID_PARAMETER,
                  "Invalid rate unit parameter.\n");
  }

  //Calculate Time value (Bits 6:3 of TRAN_SPEED)
  switch ((max_data_transfer_rate >> 3) & 0xF) {
    case 1:
      time_value = 10;
      break;

    case 2:
      time_value = 12;
      break;

    case 3:
      time_value = 13;
      break;

    case 4:
      time_value = 15;
      break;

    case 5:
      time_value = 20;
      break;

    case 6:
      time_value = 25;
      break;

    case 7:
      time_value = 30;
      break;

    case 8:
      time_value = 35;
      break;

    case 9:
      time_value = 40;
      break;

    case 10:
      time_value = 45;
      break;

    case 11:
      time_value = 50;
      break;

    case 12:
      time_value = 55;
      break;

    case 13:
      time_value = 60;
      break;

    case 14:
      time_value = 70;
      break;

    case 15:
      time_value = 80;
      break;

    default:
      mmchs_error(MMCHS_ERROR_INVALID_PARAMETER,
                  "Invalid transfer speed parameter.\n");
  }

  frequency = transfer_rate_value * time_value / 10;

  // Calculate Clock divider value to program in MMCHS_SYSCTL[CLKD] field.
  *clock_frequency_select = ((MMC_REFERENCE_CLK / frequency) + 1);

  {
    char buffer[1024];
    sprintf(
            buffer,
            "max data transfer rate: 0x%x, frequency: %d KHz, clock frequency select: %x\n",
            max_data_transfer_rate, frequency / 1000, *clock_frequency_select);
    mmchs_debug(MMCHS_DEBUG_INFO, buffer);
  }
}

static void mmchs_card_configuration_data() {
  uint32_t block_size;
  uint32_t num_blocks;
  uint32_t clock_frequency_select;

  //Calculate block_size and Total number of blocks in the detected card.
  mmchs_block_information(&block_size, &num_blocks);

  mmchs_card_info.block_size = block_size;
  mmchs_card_info.num_blocks = num_blocks;

  //Calculate Card clock divider value.
  mmchs_calculate_card_CLKD(&clock_frequency_select);
  mmchs_card_info.clock_frequency_select = clock_frequency_select;
}

static MMCHS_STATUS mmchs_card_specific_data() {
  MMCHS_STATUS status;
  uint32_t cmd_argument;

  //Send CMD9 to retrieve CSD.
  cmd_argument = mmchs_card_info.RCA << 16;
  status = mmchs_send_cmd(CMD9, CMD9_INT_EN, cmd_argument);

  if (mmchs_has_error(status)) {
    char buffer[1024];
    sprintf(buffer, "CMD9 fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_INVALID, buffer);
    return status;
  }
  //Populate 128-bit CSD register data.
  ((uint32_t *) &(mmchs_card_info.CSD_data))[0] = MMIO_READ32(MMCHS_RSP10);
  ((uint32_t *) &(mmchs_card_info.CSD_data))[1] = MMIO_READ32(MMCHS_RSP32);
  ((uint32_t *) &(mmchs_card_info.CSD_data))[2] = MMIO_READ32(MMCHS_RSP54);
  ((uint32_t *) &(mmchs_card_info.CSD_data))[3] = MMIO_READ32(MMCHS_RSP76);
  /*DEBUG(
   (MMCHS_DEBUG_INFO, "CMD9 response: %x %x %x %x\n", MMIO_READ32(MMCHS_RSP10), MMIO_READ32(
   MMCHS_RSP32), MMIO_READ32(MMCHS_RSP54), MMIO_READ32(MMCHS_RSP76)));*/

  //Calculate total number of blocks and max. data transfer rate supported by the detected card.
  mmchs_card_configuration_data();

  return status;
}

MMCHS_STATUS mmchs_perform_card_configuration() {
  uint32_t cmd_argument = 0;
  MMCHS_STATUS status;

  //Send CMD7
  cmd_argument = mmchs_card_info.RCA << 16;
  status = mmchs_send_cmd(CMD7, CMD7_INT_EN, cmd_argument);
  if (mmchs_has_error(status)) {
    char buffer[1024];
    sprintf(buffer, "CMD7 fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return status;
  }

  if ((mmchs_card_info.card_type != UNKNOWN_CARD) && (mmchs_card_info.card_type
      != MMC_CARD)) {
    // We could read SCR register, but SD Card Phys spec stats any SD Card shall
    // set SCR.SD_BUS_WIDTHS to support 4-bit mode, so why bother?

    // Send ACMD6 (application specific commands must be prefixed with CMD55)
    status = mmchs_send_cmd(CMD55, CMD55_INT_EN, cmd_argument);
    mmchs_debug(MMCHS_DEBUG_INFO, "CMD55 success\n");
    if (!mmchs_has_error(status)) {
      // set device into 4-bit data bus mode
      status = mmchs_send_cmd(ACMD6, ACMD6_INT_EN, 0x2);
      mmchs_debug(MMCHS_DEBUG_INFO, "ACMD6 success\n");
      if (!mmchs_has_error(status)) {
        // Set host controler into 4-bit mode
        MMIO_OR32(MMCHS_HCTL, DTW_4_BIT);
        mmchs_debug(MMCHS_DEBUG_INFO, "SD Memory Card set to 4-bit mode\n");
      }
    }
  }

  //Send CMD16 to set the block length
  cmd_argument = mmchs_card_info.block_size;
  status = mmchs_send_cmd(CMD16, CMD16_INT_EN, cmd_argument);
  if (mmchs_has_error(status)) {
    char buffer[1024];
    sprintf(buffer, "CMD16 fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return status;
  }

  //Change MMCHS clock frequency to what detected card can support.
  mmchs_update_clk_freq(mmchs_card_info.clock_frequency_select);

  return MMCHS_STATUS_SUCCESS;
}

MMCHS_STATUS mmchs_read_block_data(BLOCK_IO_PROTOCOL *this, void *buffer) {
  uint32_t mmc_status;
  uint32_t *data_buffer = buffer;
  uint32_t data_size = this->media->block_size / 4;
  uint32_t count;
  uint32_t retry_count = 0;

  //Check controller status to make sure there is no error.
  while (retry_count < MMCHS_MAX_RETRY_COUNT) {
    do {
      //Read Status.
      mmc_status = MMIO_READ32(MMCHS_STAT);
    } while (mmc_status == 0);

    //Check if Buffer read ready (BRR) bit is set?
    if (mmc_status & BRR) {

      //Clear BRR bit
      MMIO_OR32(MMCHS_STAT, BRR);

      //Read block worth of data.
      for (count = 0; count < data_size; count++) {
        *data_buffer++ = MMIO_READ32(MMCHS_DATA);
      }
      break;
    }
    retry_count++;
  }

  if (retry_count == MMCHS_MAX_RETRY_COUNT) {
    return MMCHS_STATUS_TIMEOUT;
  }

  return MMCHS_STATUS_SUCCESS;
}

MMCHS_STATUS mmchs_write_block_data(BLOCK_IO_PROTOCOL *this, void *buffer) {
  uint32_t mmc_status;
  uint32_t *data_buffer = buffer;
  uint32_t data_size = this->media->block_size / 4;
  uint32_t count;
  uint32_t retry_count = 0;

  //Check controller status to make sure there is no error.
  while (retry_count < MMCHS_MAX_RETRY_COUNT) {
    do {
      //Read Status.
      mmc_status = MMIO_READ32(MMCHS_STAT);
    } while (mmc_status == 0);

    //Check if Buffer write ready (BWR) bit is set?
    if (mmc_status & BWR) {

      //Clear BWR bit
      MMIO_OR32(MMCHS_STAT, BWR);

      //Write block worth of data.
      for (count = 0; count < data_size; count++) {
        MMIO_WRITE32(MMCHS_DATA, *data_buffer++);
      }

      break;
    }
    retry_count++;
  }

  if (retry_count == MMCHS_MAX_RETRY_COUNT) {
    return MMCHS_STATUS_TIMEOUT;
  }

  return MMCHS_STATUS_SUCCESS;
}

MMCHS_STATUS mmchs_transfer_block(BLOCK_IO_PROTOCOL *this, uint32_t lba,
                                  void* buffer,
                                  MMCHS_OPERATION_TYPE operation_type) {
  MMCHS_STATUS status;
  uint32_t mmc_status;
  uint32_t retry_count = 0;
  uint32_t cmd = 0;
  uint32_t cmd_interrupt_enable = 0;
  uint32_t cmd_argument = 0;

  //Populate the command information based on the operation type.
  if (operation_type == READ) {
    cmd = CMD17; //Single block read
    cmd_interrupt_enable = CMD18_INT_EN;
  } else if (operation_type == WRITE) {
    cmd = CMD24; //Single block write
    cmd_interrupt_enable = CMD24_INT_EN;
  }

  //Set command argument based on the card access mode (Byte mode or Block mode)
  if (mmchs_card_info.OCR_data.AccessMode & BIT1) {
    cmd_argument = lba;
  } else {
    cmd_argument = lba * this->media->block_size;
  }

  //Send Command.
  status = mmchs_send_cmd(cmd, cmd_interrupt_enable, cmd_argument);
  if (mmchs_has_error(status)) {
    char buffer[1024];
    sprintf(buffer, "CMD fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return status;
  }

  //Read or Write data.
  if (operation_type == READ) {
    status = mmchs_read_block_data(this, buffer);
    if (mmchs_has_error(status)) {
      mmchs_debug(MMCHS_ERROR_DEVICE, "ReadBlockData fails.\n");
      return status;
    }
  } else if (operation_type == WRITE) {
    status = mmchs_write_block_data(this, buffer);
    if (mmchs_has_error(status)) {
      mmchs_debug(MMCHS_ERROR_DEVICE, "WriteBlockData fails.\n");
      return status;
    }
  }

  //Check for the Transfer completion.
  while (retry_count < MMCHS_MAX_RETRY_COUNT) {
    //Read status
    do {
      mmc_status = MMIO_READ32(MMCHS_STAT);
    } while (mmc_status == 0);

    //Check if Transfer complete (TC) bit is set?
    if (mmc_status & TC) {
      break;

    } else {
      char buffer[1024];
      sprintf(buffer, "mmc_status for TC: %x\n", mmc_status);
      mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
      //Check if DEB, DCRC or DTO interrupt occured.
      if ((mmc_status & DEB) | (mmc_status & DCRC) | (mmc_status & DTO)) {
        //There was an error during the data transfer.

        //Set SRD bit to 1 and wait until it return to 0x0.
        MMIO_OR32(MMCHS_SYSCTL, SRD);
        while ((MMIO_READ32(MMCHS_SYSCTL) & SRD) != 0x0)
          ;

        return MMCHS_ERROR_DEVICE;
      }
    }
    retry_count++;
  }

  if (retry_count == MMCHS_MAX_RETRY_COUNT) {
    mmchs_debug(MMCHS_ERROR_DEVICE, "TransferBlockData timed out.\n");
    return MMCHS_STATUS_TIMEOUT;
  }

  return MMCHS_STATUS_SUCCESS;
}

BOOLEAN mmchs_card_present() {
  // TODO: well, this is pretty optimistic, right?
  return TRUE;
}

MMCHS_STATUS mmchs_detect_card() {
  MMCHS_STATUS status;

  if (!mmchs_card_present()) {
    return MMCHS_ERROR_NO_MEDIA;
  }

  // Initialize MMC host controller clocks.
  status = mmchs_init();
  if (mmchs_has_error(status)) {
    char buffer[128];
    sprintf(buffer, "Initialize MMC host controller fails. status: %x\n", status);
    mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
    return status;
  }

  // Software reset of the MMCHS host controller.
  MMIO_WRITE32(MMCHS_SYSCONFIG, SOFTRESET);
  kernel_sleep(1);

  while ((MMIO_READ32(MMCHS_SYSSTATUS) & RESETDONE_MASK) != RESETDONE)
    ;

  // Soft reset for all.
  // TODO: do we need this?!
  // MMIO_WRITE32(MMCHS_SYSCTL, SRA);
  // kernel_sleep(1);
  // while ((MMIO_READ32(MMCHS_SYSCTL) & SRA) != 0x0)
  // ;

  // Voltage capabilities initialization. Activate VS18 and VS30.
  MMIO_OR32(MMCHS_CAPA, (VS30 | VS18));

  // Wake up configuration
  MMIO_OR32(MMCHS_SYSCONFIG, ENAWAKEUP);
  MMIO_OR32(MMCHS_HCTL, IWE);

  // MMCHS Controller default initialization
  MMIO_OR32(MMCHS_CON, (OD | DW8_1_4_BIT | CEATA_OFF));

  MMIO_WRITE32(MMCHS_HCTL, (SDVS_3_0_V | DTW_1_BIT | SDBP_OFF));

  // Enable internal clock
  MMIO_OR32(MMCHS_SYSCTL, ICE);

  // Set the clock frequency to 80KHz.
  mmchs_update_clk_freq(CLKD_80KHZ);

  // Enable SD bus power.
  MMIO_OR32(MMCHS_HCTL, (SDBP_ON));

  // Poll till SD bus power bit is set.
  while ((MMIO_READ32(MMCHS_HCTL) & SDBP_MASK) != SDBP_ON)
    ;

  // Card identification
  status = mmchs_perform_card_identification();
  if (mmchs_has_error(status)) {
    mmchs_debug(MMCHS_ERROR_DEVICE, "No MMC/SD card detected.\n");
    return status;
  }

  // Get CSD (Card specific data) for the detected card.
  status = mmchs_card_specific_data();
  if (mmchs_has_error(status)) {
    return status;
  }

  // Configure the card in data transfer mode.
  status = mmchs_perform_card_configuration();
  if (mmchs_has_error(status)) {
    return status;
  }

  // Patch the Media structure.
  mmchs_media.last_block = (mmchs_card_info.num_blocks - 1);
  mmchs_media.block_size = mmchs_card_info.block_size;
  mmchs_media.read_only = FALSE; // set this always to FALSE in this ENV
  mmchs_media.media_present = TRUE;
  mmchs_media.media_id++;

  mmchs_debug(MMCHS_DEBUG_INFO, "SD Card Media Change SUCCESS\n");
  return status;
}

MMCHS_STATUS mmchs_read_write(BLOCK_IO_PROTOCOL *this, uint32_t lba,
                              void* buffer, size_t buffer_size,
                              MMCHS_OPERATION_TYPE operation_type) {
  MMCHS_STATUS status = MMCHS_STATUS_SUCCESS;
  uint32_t retry_count = 0;
  uint32_t block_count;
  uint32_t bytes_to_be_tranfered_this_pass = 0;
  uint32_t bytes_remaining_to_be_transfered;
  char debug[64];
  BOOLEAN update;

  sprintf(debug, "Read Block: %d\n\r", lba);
  mmchs_debug(MMCHS_DEBUG_INFO, debug);

  update = FALSE;

  if (mmchs_media_change) {
    update = TRUE;
    status = mmchs_detect_card();
    if (mmchs_has_error(status)) {
      // We detected a removal
      mmchs_media.media_present = FALSE;
      mmchs_media.last_block = 0;
      mmchs_media.block_size = 512; // Should be zero but there is a bug in DiskIo
      mmchs_media.read_only = FALSE;
    }
    mmchs_media_change = FALSE;
  } else if (!mmchs_media.media_present) {
    status = MMCHS_ERROR_NO_MEDIA;
    goto DONE;
  }

  if (update) {
    // FIXME: this is wrong?!
    mmchs_debug(MMCHS_ERROR_MEDIA_CHANGED, "SD card MEDIA has changed\n");
    // return MMCHS_ERROR_MEDIA_CHANGED;
  }

  if (mmchs_has_error(status)) {
    goto DONE;
  }

  if (buffer == NULL) {
    status = MMCHS_ERROR_INVALID_PARAMETER;
    goto DONE;
  }

  if (lba > this->media->last_block) {
    status = MMCHS_ERROR_INVALID_PARAMETER;
    goto DONE;
  }

  if ((buffer_size % this->media->block_size) != 0) {
    status = MMCHS_ERROR_BAD_BUFFER_SIZE;
    goto DONE;
  }

  // check if the data lines are not in use.
  while ((retry_count++ < MMCHS_MAX_RETRY_COUNT) && ((MMIO_READ32(MMCHS_PSTATE)
      & DATI_MASK) != DATI_ALLOWED))
    ;

  if (retry_count == MMCHS_MAX_RETRY_COUNT) {
    status = MMCHS_STATUS_TIMEOUT;
    goto DONE;
  }

  bytes_remaining_to_be_transfered = buffer_size;
  while (bytes_remaining_to_be_transfered > 0) {

    if (mmchs_media_change) {
      status = MMCHS_ERROR_NO_MEDIA;
      mmchs_debug(MMCHS_DEBUG_INFO,
                  "mmchs_read_write() MMCHS_ERROR_NO_MEDIA due to mmchs_media_change\n");
      goto DONE;
    }

    // Turn OFF DMA path until it is implemented
    // bytes_to_be_tranfered_this_pass = (bytes_to_be_tranfered_this_pass >= MAX_MMCHS_TRANSFER_SIZE) ? MAX_MMCHS_TRANSFER_SIZE : bytes_remaining_to_be_transfered;
    bytes_to_be_tranfered_this_pass = this->media->block_size;

    block_count = bytes_to_be_tranfered_this_pass / this->media->block_size;

    /* Do not use DMA so far ... use single block transfer.
     if (block_count > 1) {
     status = mmchs_dma_blocks(this, lba, buffer, block_count, operation_type); */

    //Transfer a block worth of data.
    status = mmchs_transfer_block(this, lba, buffer, operation_type);

    if (mmchs_has_error(status)) {
      char buffer[1024];
      sprintf(buffer, "TransferBlockData fails. 0x%8x\n", status);
      mmchs_debug(MMCHS_ERROR_DEVICE, buffer);
      goto DONE;
    }

    bytes_remaining_to_be_transfered -= bytes_to_be_tranfered_this_pass;
    lba += block_count;
    buffer = (uint8_t *) buffer + this->media->block_size;
  }

  DONE: return status;

}

BLOCK_STATUS mmchs_read_blocks(BLOCK_IO_PROTOCOL *this, uint32_t media_id,
                               BLOCK_LBA lba, size_t buffer_size, void *buffer) {
  BLOCK_STATUS status;

  // perform read operation.
  status = mmchs_read_write(this, (uint32_t) lba, buffer, buffer_size, READ);

  return status;
}

BLOCK_STATUS mmchs_write_blocks(BLOCK_IO_PROTOCOL *this, uint32_t media_id,
                                BLOCK_LBA lba, size_t buffer_size, void *buffer) {
  BLOCK_STATUS status;

  // perform write operation.
  status = mmchs_read_write(this, (uint32_t) lba, buffer, buffer_size, WRITE);

  return status;
}

BLOCK_IO_PROTOCOL mmchs_block_io = { &mmchs_media, mmchs_read_blocks,
                                     mmchs_write_blocks, };

DEV_STATUS mmchs_device_read(EXTERNAL_DEVICE *this, uint32_t reg,
                             uint32_t length, void* buffer) {

  return mmchs_read_write(&mmchs_block_io, reg, buffer, length, READ);
}

DEV_STATUS mmchs_device_write(EXTERNAL_DEVICE *this, uint32_t reg,
                              uint32_t length, void* buffer) {

  return mmchs_read_write(&mmchs_block_io, reg, buffer, length, WRITE);
}

EXTERNAL_DEVICE* mmchs_io_device;

static MMCHS_STATUS mmchs_init() {

  memset(&mmchs_card_info, 0, sizeof(mmchs_card_info));
  mmchs_io_device = malloc(sizeof(EXTERNAL_DEVICE)); // TODO: we create a leak here!

  mmchs_io_device->read = mmchs_device_read;
  mmchs_io_device->write = mmchs_device_write;

  return MMCHS_STATUS_SUCCESS;
}

