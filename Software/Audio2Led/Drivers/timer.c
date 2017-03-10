/** @file timer.c
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#include "timer.h"

/*!
* @brief @todo
*/
void timer_init(void)
{
    DDRB = 0X3F;
    
    //Sets timer 1 clear on compare match (output to low level) for output compares a and b
    TCCR1A |= (1 << COM1A1)|(1 << COM1B1)|(1 << WGM10);
    //Set prescalar to clk/256 (
    TCCR1B |= (1 << CS12);
    //Enables interrupts for OCR1A and OCR1B
    TIMSK1 |= (1 << OCIE1B)|(1 << OCIE1A);
    
    TCCR2A |= (1 << COM2A1)|(1 << COM2B1)|(1 << WGM20);
    TCCR2B |= (1 << CS22)|(1 << CS21);
    //Enables interrupts for OCR2B
    TIMSK2 |= (1 << OCIE2A);
}

void timer_update(uint8_t red, uint8_t green, uint8_t blue)
{
    RED_CMP = red;
    BLU_CMP = blue;
    GRN_CMP = green;
}
