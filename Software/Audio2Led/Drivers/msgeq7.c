/** @file msgeq7.c
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 John Fritz
* All rights reserved
*/

#include "msgeq7.h"

/*!
* @brief Get seven audio band levels from MSGEQ7
* @param[in] *audio     Pointer to location to store data read from MSGEQ7
* @return void
*/
void msgeq7_get_audio(uint16_t *audio)
{
    // Setup bitbang interface to MSGEQ7
    GPIO_PORT &= ~((1<<MSGEQ7_STR) | (1<<MSGEQ7_RST));  
    GPIO_PORT |=   (1<<MSGEQ7_RST);
    _delay_us(100);
    GPIO_PORT |=   (1<<MSGEQ7_STR);
    _delay_us(50);
    GPIO_PORT &=  ~(1<<MSGEQ7_STR);
    _delay_us(50);
    GPIO_PORT &=  ~(1<<MSGEQ7_RST);
    GPIO_PORT |=   (1<<MSGEQ7_STR);
    _delay_us(50); 
    
    // Bitbang and read each band
    for (uint8_t i=0; i<MSGEQ7_AUD_BANDS; i++)
    {
        GPIO_PORT &= ~(1<<MSGEQ7_STR);\
        _delay_us(30);
        audio[i] = adc_get_avg();  // TBD
        _delay_us(30);  // TBD
        GPIO_PORT &= ~(1<<MSGEQ7_STR);
    }  
}

void msgeq7_init(void)
{
    GPIO_DDR  |=  ((1<<MSGEQ7_STR) | (1<<MSGEQ7_RST));  // Set rst and strobe to output
}
