/** @file module.h
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>

void adc_init(void);
uint16_t adc_get(void);
uint16_t adc_get_avg(uint8_t pow_2);

#endif /* _ADC_H */