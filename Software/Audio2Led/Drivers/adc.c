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
    
    // Set ADC for external AVcc reference, ADC channel 0
    ADMUX |= (1<<REFS0);
    
    // Disable digital input on ADC channel 0
    DIDR0 |= (1<<ADC0D);
    
    // Enable ADC, prescale clock to divide by 2 (13 cycles / 8 MHz clk = 1.625 usec sample period). 
    ADCSRA |= (1<<ADEN) ;
    
    // Do one ADC conversion to clear registers
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));    
}

/*!
* @brief @todo
* @return void
*/
uint16_t adc_get(void)
{
    volatile uint8_t adc_val = 0;
    ADCSRA |= (1<<ADSC);            // Start conversion
    while (ADCSRA & (1<<ADSC));     // Wait for conversion to complete   
    adc_val  = ADCH;                // Read upper byte of ADC result
    adc_val |= ADCL;                // Read lower byte of ADC result
    return (adc_val);
}

/*!
* @brief @todo
* @return void
*/
uint16_t adc_get_avg_4(void)
{
    volatile uint16_t adc_val = 0;
    
    for (uint8_t i=0; i<4; i++)
    {
        adc_val += adc_get();
    }
    return (adc_val>>2);    
}
