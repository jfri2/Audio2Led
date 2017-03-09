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

/*!
* @brief @todo
*/
inline void timer_update(uint8_t red, uint8_t blue, uint8_t green)
{
    RED_CMP = red;
    BLU_CMP = blue;
    GRN_CMP = green;
}

#endif /* _TIMER_H */