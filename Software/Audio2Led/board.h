/** @file board.h
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#ifndef _BOARD_H
#define _BOARD_H

#include <avr/io.h>

// External Oscillator Frequency
#define F_CPU   16000000L

// Hardware Pin Definitions
#define GPIO_DDR	DDRD		// All GPIO Data Direction register
#define GPIO_PORT	PORTD		// All GPIO PORT register
#define MSGEQ7_RST	6			// MSGEQ7 Reset Pin in port gpio port 
#define MSGEQ7_STR	5			// MSGEQ7 Strobe Pin in port gpio port 
#define SW_0		2			// User pushbutton 0 position in gpio port
#define SW_1		3			// User pushbutton 1 position in gpio port
#define SW_2		4			// User pushbutton 2 position in gpio port
#define RED_CMP		OCR1B		// Green LED Output Compare Register
#define GRN_CMP		OCR1A		// Red LED Output Compare Register
#define BLU_CMP		OCR2A		// Blue LED Output Compare Register

#endif /* _BOARD_H */