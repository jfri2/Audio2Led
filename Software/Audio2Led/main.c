/*
 * Audio2Led
 *
 * Created: 3/6/2017 7:22:31 PM
 * Author : John
 */ 

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Drivers/msgeq7.h"

void system_init(void);

ISR(INT0_vect)
{
    // @todo
}

ISR(INT1_vect)
{
    // @todo
}


int main(void)
{
    system_init();

    while (1) 
    {
    }
}

/*!
* @brief @todo
* @return void
*/
void system_init(void)
{
    msgeq7_init();
    pb_init();    
}

