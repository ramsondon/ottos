/*
 * audio.h
 *
 *  Created on: Jan 12, 2012
 *      Author: stefan
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <ottos/const.h>

#define MAX 32767 // maybe already defined?
#define SUS ((unsigned int )(MAX * 0.8))
// sample rate 48kHz
#define SRATE 48000

// maybe fix this...use the already built in functions...
#define reg32r(b, r) (*(volatile uint32_t *)((b)+(r)))
#define reg32w(b, r, v) (*((volatile uint32_t *)((b)+(r))) = (v))

/* envelope details */
struct env {
	unsigned int target;
	unsigned int time;
};

EXTERN struct env adsr[];
EXTERN int ftable[];

EXTERN void audio_init();
EXTERN void smoke_on_the_water();

#endif /* AUDIO_H_ */
