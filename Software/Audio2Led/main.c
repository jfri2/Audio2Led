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
#include "Drivers/pb.h"
#include "Drivers/adc.h"
#include "Drivers/timer.h"
#include "Drivers/msgeq7.h"

#define MAX_BUTTON_VAL  7

void system_init(void);

uint8_t audio_level[MSGEQ7_AUD_BANDS];
uint8_t bass;
uint8_t mid;
uint8_t treb;
uint8_t red;
uint8_t green;
uint8_t blue;

volatile uint8_t button_counter = 3;

ISR(INT0_vect)
{
    button_counter++;
    if (button_counter > MAX_BUTTON_VAL) button_counter = 0;
}

ISR(INT1_vect)
{
    // @todo
}

int main(void)
{
    system_init();
    sei();          // Enable global interrupts
        
    while (1) 
    {
        // Get audio
        msgeq7_get_audio(audio_level);
        
        // Assign to bass/mid/treb
        bass = audio_level[0];
        mid  = (((audio_level[1] + audio_level[2] + audio_level[3]) / 3));
        treb = (((audio_level[4] + audio_level[5] + audio_level[6]) / 3));
        
        // Normal Playback
	    if (bass < 30) bass = 0;
	    if (mid < 30) mid = 0;
	    if (treb < 30) treb = 0;
        
        switch (button_counter)
        {
            case 0:
                red = 50;
                green = 0;
                blue = 50;
            break;
            
            case 1:
                red = bass;
                green = mid;
                blue = treb;
            break;
            
            case 2:
                red = mid;
                green = treb;
                blue = bass;
            break;
            
            case 3:
                red = treb;
                green = bass;
                blue = mid;
            break;
            
            case 4:
                red = bass;
                green = treb;
                blue = mid;
            break;
            
            case 5:
                red = mid;
                green = bass;
                blue = treb;
            break;
            
            case 6:
                red = treb;
                green = mid;
                blue = bass;
            break;
            
            case 7:
                red = 0;
                green = 0;
                blue = 0;
            break;
            
            default:
                red = 0;
                green = 0;
                blue = 0;
            break;
        }

        // Update outputs
        timer_update(red, green, blue);
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

