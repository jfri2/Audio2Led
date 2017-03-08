/** @file adc.c
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#include "adc.h"
#include "../board.h"

/*!
* @brief @todo
* @return void
*/
void adc_init(void)
{
    // Set entire ADC DDR to inputs with pullups on
    ADC_DDR  = 0x00;
    ADC_PORT = 0x3F;
}

/*!
* @brief @todo
* @return void
*/
uint16_t adc_get(void)
{
    static uint16_t adc_val = 0;
    return (adc_val);
}

/*!
* @brief @todo
* @return void
*/
uint16_t adc_get_avg(uint8_t pow_2)
{
    static uint32_t adc_val = 0;
    
    for (uint8_t i=0; i<8; i++)
    {
        adc_val += adc_get();
    }
    return ((uint16_t)(adc_val>>pow_2));    
}
