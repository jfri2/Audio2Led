/** @file msgeq7.h
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#ifndef _MSGEQ7_H
#define _MSGEQ7_H

#include "../board.h"
#include "adc.h"
#include <util/delay.h>

#define MSGEQ7_AUD_BANDS   7       // Audio bands in MSGEQ7

uint16_t msgeq7_audio_level[MSGEQ7_AUD_BANDS];

void msgeq7_get_audio(uint16_t *audio);
void msgeq7_init(void);

#endif /* _MSGEQ7_H */