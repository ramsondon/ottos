/*
 * tps65950SCD.h
 *
 *	TPS65950 System Companion Device
 *
 * 	TPS65950 OMAP™ Power Management and System Companion Device ES 1.2 TRM (Rev. G)
 * 	http://www.ti.com/product/tps65950
 *
 *  Created on: Jan 12, 2012
 *      Author: stefan
 */

#ifndef TPS65950SCD_H_
#define TPS65950SCD_H_

// I2C addresses of audio-voice
#define SCD_AUDIO_VOICE		0x49

// AUDIO_VOICE registers
#define AV_CODEC_MODE 		0x00000001
#define AV_OPTION 			0x00000002
#define AV_RX_PATH_SEL 		0x00000043
#define AV_APLL_CTL 		0x0000003A
#define AV_AVDAC_CTL 		0x00000017
#define AV_ARXL2_APGA_CTL 	0x0000001B
#define AV_ARXR2_APGA_CTL 	0x0000001C
#define AV_HS_SEL 			0x00000022
#define AV_ARXR2PGA			0x00000012
#define AV_ARXL2PGA 		0x00000013
#define AV_HS_GAIN_SET 		0x00000023
#define AV_AUDIO_IF 		0x0000000E
#define AV_BOOST_CTL 		0x0000002E
#define AV_HS_POPN_SET		0x00000024
#define AV_ATXL1PGA 		0x0000000A
#define AV_ATXR1PGA 		0x0000000B
#define AV_AVADC_CTL 		0x00000007
#define AV_ADCMICSEL 		0x00000008
#define AV_ANAMICL 			0x00000005
#define AV_ANAMICR 			0x00000006
#define AV_ANAMIC_GAIN 		0x00000048

// CODEC_MODE settings
#define APPL_RATE_8			(0<<4)
#define APPL_RATE_48 		(0xa<<4)
#define CODECPDZ 			2
#define OPT_MODE			1

// OPTION settings
#define ARXR2_EN 			(1<<7)
#define ARXL2_EN 			(1<<6)
#define ARXR1_EN 			(1<<5)
#define ARXL1_VRX_EN 		(1<<4)
#define ATXR2_VTXR_EN 		(1<<3)
#define ATXL2_VTXL_EN 		(1<<2)
#define ATXR1_EN 			(1<<1)
#define ATXL1_EN 			(1<<0)

// RX_PATH_SEL settings
#define B_RXL1_SEL 			(0<<2)
#define B_RXR1_SEL 			(0<<0)

// APLL_CTL settings
#define APLL_EN 			(1<<4)
#define APLL_INFREQ_26 		6

// ACDAT_CTL settings
#define ADACL2_EN 			(1<<3)
#define ADACR2_EN 			(1<<2)

// ARX[LR]1_AGPA_CTL
#define B_ARX1_GAIN_SET 	3
#define ARX1_GAIN_SET_MAX 	0xf
#define ARX1_FM_EN 			(1<<2)
#define ARX1_DA_EN 			(1<<1)
#define ARX1_PDZ 			(1<<0)

// HS_SEL settings
#define HSOR_AR2_EN 		(1<<5)
#define HSOL_AL2_EN 		(1<<2)

// AUDIO_IF settings
#define DATA_WIDTH_16 		(0<<5)
#define AIF_FORMAT_CODEC 	(0<<3)
#define AIF_EN 				(1<<0)

// HS_POPN_SET settings
#define VMID_EN 			(1<<6)

// for 26Mhz sysclock
#define RAMP_DELAY_20ms 	(0<<2)
#define RAMP_DELAY_40ms 	(1<<2)
#define RAMP_DELAY_81ms 	(2<<2)
#define RAMP_DELAY_161ms 	(3<<2)
#define RAMP_DELAY_323ms 	(4<<2)
#define RAMP_DELAY_645ms 	(5<<2)
#define RAMP_DELAY_1291ms 	(6<<2)
#define RAMP_DELAY_2581ms 	(7<<2)
#define RAMP_EN 			1

// AVADC_CTL settings
#define ADCL_EN 			(1<<3)
#define ADCR_EN 			(1<<1)

// ANAMICL settings
#define CNCL_OFFSET_START 	(1<<7)
#define OFFSET_CNCL_SEL_RXALL (3<<5)
#define AUXL_EN 			(1<<2)

// ANAMICR settings
#define AUXR_EN				(1<<2)

#endif /* TPS65950SCD_H_ */
