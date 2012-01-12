/*
 * audio.c
 *
 *  Created on: Jan 12, 2012
 *      Author: stefan
 */

#include "../../hal/platform.h"
#include "audio.h"

#include <ottos/memory.h>

// A-H (incl. cis) is note (G is lower octave).
// 0-9 duration of *next* note in half-notes
char smoke[] =
	"GH2CGHI4CGH2CH4G";

/* adsr of our note */
struct env adsr[] = {
	{ MAX, ((unsigned int)(SRATE * 0.05)) },
	{ SUS, ((unsigned int)(SRATE * 0.05)) },
	{ SUS, ((unsigned int)(SRATE * 0.2)) },
	{ 0, ((unsigned int)(SRATE * 0.02)) }
};

// add here new notes: http://www.phy.mtu.edu/~suits/notefreqs.html
// wavelengths of various basic notes
int ftable[] = {
        (int)(SRATE / 220.000),  /* a3 */
        (int)(SRATE / 246.942),
        (int)(SRATE / 261.626),  /* c3 */
        (int)(SRATE / 293.665),
        (int)(SRATE / 329.628),
        (int)(SRATE / 349.228),
        (int)(SRATE / 195.998),  /* g2 */
        (int)(SRATE / 233.080),  /* h3 */
        (int)(SRATE / 277.180) 	 /* cis */
};

/* play one note with wavelength wvlen to channels lr (bits [1:0]) for sustain length len */
static playnote(int wvlen, int lr, int len) {
	int i;
	int ampstart = 0;
	int st = 0;

	for (i = 0; i < 4; i++) {
		int at = adsr[i].target;
		int dur = adsr[i].time;
		int j;

		// scale 'sustain' for note length
		if (i == 2) {
			dur = dur * len >> 8;
		}

		for (j = 0; j < dur; j++) {
				int amp = (at-ampstart) * j / dur + ampstart;
				int v = (st*2 - wvlen) * amp / wvlen;

				// send out 2 samples, left/right
				while ((reg32r(MCBSP2_BASE, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0)
						;
				reg32w(MCBSP2_BASE, MCBSPLP_DXR_REG, lr & 1 ? v : 0);
				while ((reg32r(MCBSP2_BASE, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0)
						;
				reg32w(MCBSP2_BASE, MCBSPLP_DXR_REG, lr & 2 ? v : 0);

				st += 1;
				if (st >= wvlen) {
						st = 0;
				}
		}
		ampstart = at;
	}
}

// helper function to write audio registers
static void audioRegW(uint32_t address, uint32_t value) {
	i2c_write8(I2C1_BASE, SCD_AUDIO_VOICE, address, value);
}

// Programming Model page 2049
static void init_mcbsp2(void) {
	int loop = 0;

	// Clocks
	// set the McBSP2 clock -> using functional clock -> page 1998, 708 (register overview)
	SET_BIT( (address) (CONTROL_GENERAL_BASE + CONTROL_DEVCONF0), MCBSP2_CLKS_BIT);

	// Power-Domain Clock Control Registers
	// CM_FCLKEN_PER (Functional Clock Enable Register - domain peripherals set)
	// CM_ICLKEN_PER (Interface Clock Enable Register - domain peripherals set)
	// Because a module may or may not be able to function without its IC of FC
	// it requires consistent programming of CM_FCLKEN and CM_ICLKEN registers!
	// Therefore we turn both clocks on -> Uhrenvergleich! :)
	// page 299 - 301
	SET_BIT( (address) (PER_CM_BASE + CM_FCLKEN_PER), EN_MCBSP2);
	SET_BIT( (address) (PER_CM_BASE + CM_ICLKEN_PER), EN_MCBSP2);

	// Hardware and Software Reset
	// McBSP2,3 and 4 belong to the PER Domain and their reset signal is the PER_RST from the RRCM module
	// set the transmitter in reset mode (needed to configure) page 2083
	CLEAR_BIT( (address) (MCBSP2_BASE + MCBSPLP_SPCR2_REG), MCBSP_FRST_BIT);
	CLEAR_BIT( (address) (MCBSP2_BASE + MCBSPLP_SPCR2_REG), MCBSP_XRST_BIT);
	// set the receiver in reset mode (needed to configure) page 2086
	CLEAR_BIT( (address) (MCBSP2_BASE + MCBSPLP_SPCR1_REG), MCBSP_RRST_BIT);

	// disable gpio-mode
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_PCR_REG), 0);

	// xmit format page 2088
	// XCR1
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_XCR1_REG), MCBSP_WDLEN_16_BIT); 	// 16-bit word length for phase 1
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_XCR1_REG), RFRLEN2_BIT);			// framelength = 1 word

	// XCR2
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_XCR2_REG), RPHASE_DUAL_BIT); 		// 2 phase
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_XCR2_REG), RDATDLY_ONE_BIT);		// delay of 1 ms for i2s page
	CLEAR_BIT( (address) (MCBSP2_BASE + MCBSPLP_XCR2_REG), RFRLEN2_BIT);			// framelength = 1 word
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_XCR2_REG), MCBSP_WDLEN_16_BIT);	// 16-bit word length for phase 2

	// receive format
	//	RCR1
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_RCR1_REG), MCBSP_WDLEN_16_BIT); 	// 16-bit word length for phase 1
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_RCR1_REG), RFRLEN2_BIT);			// framelength = 1 word

	// RCR2
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_RCR2_REG), RPHASE_DUAL_BIT); 		// 2 phase
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_RCR2_REG), RDATDLY_ONE_BIT);		// delay of 1 ms for i2s page
	CLEAR_BIT( (address) (MCBSP2_BASE + MCBSPLP_RCR2_REG), RFRLEN2_BIT);			// framelength = 1 word
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_RCR2_REG), MCBSP_WDLEN_16_BIT);	// 16-bit word length for phase 2

	// disable DMA
	//setBit( (address) (MCBSP1_BASE + MCBSPLP_XCCR_REG), XDISABLE_BIT);

	// clock polarity
	// receiver
	SET_BIT( (address) (MCBSP1_BASE + MCBSPLP_PCR_REG), MCBSP_CLKRP_BIT); // rising edge
	SET_BIT( (address) (MCBSP1_BASE + MCBSPLP_PCR_REG), MCBSP_CLKXP_BIT); // falling edge

	// Delay to allow settings to sync (hopefully 2 SRG clock cycles)
	for (loop=0; loop < 0XFF; loop++);

	// wake up!
	// wake up the transmitter of reset mode (needed to configure) page 2083
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_SPCR2_REG), MCBSP_FRST_BIT);
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_SPCR2_REG), MCBSP_XRST_BIT);
	// wake up the receiver of reset mode (needed to configure) page 2086
	SET_BIT( (address) (MCBSP2_BASE + MCBSPLP_SPCR1_REG), MCBSP_RRST_BIT);

	return;
}

static void init_tps(void) {
	// turn it off to configure
	audioRegW(AV_CODEC_MODE, 0);

	// page 725
	// Audio mode: Select sampling frequence (Fs) -> set to 8 kHz
	// Audio and voice option selection: Option 1: 2 RX and TX stereo audio paths
	audioRegW(AV_CODEC_MODE, APPL_RATE_8 | OPT_MODE);

	// Audio/voice digital filter power control
	audioRegW(AV_OPTION, ARXR2_EN | ARXL2_EN | ATXR1_EN | ATXL1_EN);

	// path selection
	audioRegW(AV_RX_PATH_SEL,  B_RXL1_SEL | B_RXR1_SEL);

	// set system clock speed and enable the local clock
	audioRegW(AV_APLL_CTL, APLL_EN | APLL_INFREQ_26);

	// left/right dacs enlabled
	audioRegW(AV_AVDAC_CTL, ADACL2_EN | ADACR2_EN);

	// turn on left and right output
	audioRegW(AV_ARXL2_APGA_CTL, ARX1_PDZ | ARX1_DA_EN | (0x06 << B_ARX1_GAIN_SET));
	audioRegW(AV_ARXR2_APGA_CTL, ARX1_PDZ | ARX1_DA_EN | (0x06 << B_ARX1_GAIN_SET));

	// headset output
	audioRegW(AV_HS_SEL, HSOL_AL2_EN | HSOR_AR2_EN);

	// set digital gain to 0db
	audioRegW(AV_ARXL2PGA, 0x30 | (3<<6));              /* 0dB */
	audioRegW(AV_ARXR2PGA, 0x30 | (3<<6));              /* 0dB */

	// set headset left/right gain (balance) to 0db each
	audioRegW(AV_HS_GAIN_SET, 0x0a);

	// audio interface.  master mode, i2s format, 16 bit data
	audioRegW(AV_AUDIO_IF, AIF_FORMAT_CODEC | DATA_WIDTH_16 | AIF_EN);

	// volume/effects
	// set base boost effect
	audioRegW(AV_BOOST_CTL, 0);
	// anti-pop paramters
	audioRegW(AV_HS_POPN_SET, VMID_EN | RAMP_DELAY_161ms | RAMP_EN);

	// tramsit settings (audio in)
	// left/right gain
	audioRegW(AV_ATXL1PGA, 0x0);
	audioRegW(AV_ATXR1PGA, 0x0);

	// left/right ADC, and choose as TXx1
	audioRegW(AV_AVADC_CTL, ADCL_EN | ADCR_EN);
	audioRegW(AV_ADCMICSEL, 0);

	// line in on beagle comes from AUX, also enable anti-pop and start
	audioRegW(AV_ANAMICL, 0x34 | AUXL_EN | OFFSET_CNCL_SEL_RXALL | CNCL_OFFSET_START);
	audioRegW(AV_ANAMICR, 0x14 | AUXR_EN);
	audioRegW(AV_ANAMIC_GAIN, (0<<3)|0);

	// now power it up, with sample rate and option 1 (2x stereo audio paths in and out)
	audioRegW(AV_CODEC_MODE, APPL_RATE_8 | 0 | OPT_MODE);
	audioRegW(AV_CODEC_MODE, APPL_RATE_48 | CODECPDZ | OPT_MODE);

	return;
}

void audio_init() {

	// init the mcbsp2 register on the OMAP
	init_mcbsp2();

	// init the tps chip with I2C
	init_tps();

	return;
}

void smoke_on_the_water() {
	int i = 0;
	while (i++ < 3) {
		//char *np = mary;
		char *np = smoke;
		char n;
		int len = 256;

		while ( (n = *np++) ) {
			if (n >= '0' && n <= '9') {
					len = (n - '0') * 128;
			} else {
					playnote(ftable[n - 'A'], 3, len);
					len = 256;
			}
		}
	}

	return;
}
