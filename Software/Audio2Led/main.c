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

uint16_t audio_level[MSGEQ7_AUD_BANDS];
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
    //WDTCSR &= (1 << WDIE);
    system_init();

    while (1) 
    {
        // Get audio levels
        msgeq7_get_audio(audio_level);
        
        // Assign to bass/mid/treb
        bass = (uint8_t) ((audio_level[0] + audio_level[1])                  >> 3);         // bitshift 2 over (cast to 8 bit) and average
        mid  = (uint8_t)(((audio_level[2] + audio_level[3] + audio_level[4]) >> 2) / 3);    // bitshift 2 over and average
        treb = (uint8_t) ((audio_level[5] + audio_level[6])                  >> 3);         // bitshift 2 over and average
        
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
    timer_init();
    adc_init();
    msgeq7_init();  
}

