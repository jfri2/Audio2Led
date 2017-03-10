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
#include <stdint.h>

#define MSGEQ7_STROBE_HIGH()   (GPIO_PORT |=  (1<<MSGEQ7_STR))
#define MSGEQ7_STROBE_LOW()    (GPIO_PORT &= ~(1<<MSGEQ7_STR))
#define MSGEQ7_RESET_HIGH()    (GPIO_PORT |=  (1<<MSGEQ7_RST))
#define MSGEQ7_RESET_LOW()     (GPIO_PORT &= ~(1<<MSGEQ7_RST))

#define MSGEQ7_AUD_BANDS   7       // Audio bands in MSGEQ7

void msgeq7_get_audio(uint8_t *audio);
void msgeq7_init(void);

#endif /* _MSGEQ7_H */