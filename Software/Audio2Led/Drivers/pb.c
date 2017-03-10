/** @file pb.c
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#include "pb.h"

/*!
* @brief @todo
* @return void
*/
void pb_init(void)
{
	DDRD	&= ~(1 << SW_0) | ~(1 << SW_1) | ~(1 << SW_2);		//Data direction for SWs set to input
	PORTD	|= (1 << SW_0) | (1 << SW_1) | (1 << SW_2);		//Internal pullup resistors turned on
    EICRA   |= (1<<ISC11) | (1<<ISC01);     //Falling edge interrupt triggering
    EIMSK   |= (1<<INT1)  | (1<<INT0);      //Enables external interrupts on INT0 and INT1 (SW0 and SW1)
}

