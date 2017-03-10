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
#include "Drivers/adc.h"
#include "Drivers/timer.h"
#include "Drivers/msgeq7.h"


void system_init(void);

uint8_t audio_level[MSGEQ7_AUD_BANDS];
uint8_t bass = 0;
uint8_t mid = 0;
uint8_t treb = 0;

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
    //sei();										//Enables global interrupts
        
    while (1) 
    {
        // Get audio levels
        msgeq7_get_audio(audio_level);
        
        // Assign to bass/mid/treb
        bass = (((audio_level[0] + audio_level[1]) / 2));
        mid  = (((audio_level[2] + audio_level[3] + audio_level[4]) / 3));
        treb = (((audio_level[5] + audio_level[6]) / 3));       
        
        // Assign to timer outputs
        timer_update(treb, bass, mid);
    }
}

/*!
* @brief @todo
* @return void
*/
void system_init(void)
{
    msgeq7_init(); 
    adc_init();
    timer_init(); 
}

