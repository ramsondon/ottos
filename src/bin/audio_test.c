/*
 * audio_test.c
 *
 *  Created on: Jan 12, 2012
 *      Author: stefan
 */

#include "../drivers/audio/audio.h"
#include "audio_test.h"


void audio_test() {

	// init audio
	audio_init();

	// play sample - 3 times in a row!
	smoke_on_the_water();

	return;
}
