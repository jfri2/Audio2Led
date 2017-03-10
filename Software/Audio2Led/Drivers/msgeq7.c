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
void msgeq7_get_audio(uint8_t *audio)
{
    // Reset MSGEQ7
    MSGEQ7_STROBE_LOW();
    MSGEQ7_RESET_LOW();
    MSGEQ7_RESET_HIGH();
    _delay_us(100);
    MSGEQ7_STROBE_HIGH();   
    _delay_us(50);
    MSGEQ7_STROBE_LOW();
    _delay_us(50);
    MSGEQ7_RESET_LOW();
    MSGEQ7_STROBE_HIGH();
    _delay_us(50);

    // Strobe MSGEQ7 and read each band (4 samples per band)
    for (uint8_t i=0; i<MSGEQ7_AUD_BANDS; i++)
    {
        MSGEQ7_STROBE_LOW();
        _delay_us(30);  // Strobe low pulse width (40 us of 80 us (+ ADC read time) strobe period) 
        audio[i] = adc_get();  // TBD, must take less than 40 us. Must do 4 or fewer conversions
        _delay_us(5);
        MSGEQ7_STROBE_HIGH();
        _delay_us(50);  // @todo: subtract adc_get_avg time from this. It should be ok to leave in but if timing is an issue take it out
    }  
}

void msgeq7_init(void)
{
    GPIO_DDR  |=  ((1<<MSGEQ7_STR) | (1<<MSGEQ7_RST));  // Set rst and strobe to output
}
