/** @file timer.h
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#ifndef _TIMER_H
#define _TIMER_H

#include "../board.h"

void timer_init(void);
void timer_update(uint8_t red, uint8_t blue, uint8_t green);


#endif /* _TIMER_H */