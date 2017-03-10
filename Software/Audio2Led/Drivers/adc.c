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
#include <stdint.h>

/*!
* @brief @todo
* @return void
*/
void adc_init(void)
{

    // setup the Analog to Digital Converter
    ADMUX = 0x40;		        // start by selecting the voltage reference - Avcc
    ADMUX = ADMUX | 0x00;       // Select the ADC channel - channel 0
    ADMUX = ADMUX | 0x20;	    // set for Left Justified - Only using 8 bit of resolution
    ADMUX |= (1 << REFS0);	    //Sets reference to AVcc                                 
    ADCSRA = 0x04;	            // select the ADC clock frequency - Clock / 128
    ADCSRA = ADCSRA | 0x80;	    // enable the ADC
        
    // Do one ADC conversion to clear registers
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));   
    DDRD |= (1<<PORTD7); 
}

/*!
* @brief @todo
* @return void
*/
uint8_t adc_get(void)
{    
PORTD |= (1<<PORTD7);       // DEBUG
    ADCSRA |= (1<<ADSC);            // Start conversion
    while (ADCSRA & (1<<ADSC));     // Wait for conversion to complete   
PORTD &= ~(1<<PORTD7);      // DEBUG
    return (ADCH);
}

/*!
* @brief @todo
* @return void
*/
uint8_t adc_get_avg_4(void)
{
    volatile uint16_t adc_val = 0;
    
    for (uint8_t i=0; i<4; i++)
    {
        adc_val += (uint16_t)adc_get();
    }
    return ((uint8_t)(adc_val>>2));    
}
