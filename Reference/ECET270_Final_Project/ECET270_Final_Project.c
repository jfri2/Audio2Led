/*
 * MSGEQ7_Test_0.c
 *
 * Created: 4/21/2015 10:45:26 AM
 *  Author: John
 
 This code is for driving an ATmega328p on the ECET270 Led Driver Board Version 0.1
 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ADC.c"
#include "serial.c"					//only needs to be included if using stdio or serial functions

//Preprocessor Macros
#define EQ_RESET 6					//msgeq-7 reset on portd bit 6
#define STROBE 5					//strobe pin on portd bit 5
#define SW0 2						//SW0 on PORTD bit 2 (INT0)
#define SW1 3						//SW1 on PORTD bit 3 (INT1)
#define SW2 4						//SW2 on PORTD bit 4 (PCINT20)

#define GREEN OCR1A					//GREEN LED is PWM Output Compare A for timer1
#define RED OCR1B					//RED LED is PWM Output Compare B for timer1
#define BLUE OCR2A					//BLUE LED is PWM Output Compare A for timer2

#define DISP_0 0
#define DISP_1 1
#define DISP_2 2


//Global audioband array
uint8_t AudioLevel[7] = {0};		//init 7 position AudioLevels array to 0

//Globals for storing average output values	
uint8_t out_low;
uint8_t out_mid;
uint8_t out_high;

//Fade delay function declaration
const int led_delay = 200;

//Selector switch global initialized at 0xFF
volatile char display_select = 0xFF;
volatile char second_disp_sel = 0xFF;

//Timer0 overflow value
volatile int timer0_overflow_cnt = 0;

//MCUSR stuff
uint8_t status_reg_read = 0xFF;			

//Function Prototypes
void GetAudioBandLevel(void);							//Gets audio levels, stores in global AudioLevels array
void MusicOnLed(void);									//Music display function on LEDs
void FadeOnLed(void);									//Fade display function on LEDs
void StrobeOnLed(uint8_t, uint8_t, uint8_t);			//Strobe display function on LEDs
void IncrementUp(uint8_t, uint8_t);						//Increments specific LED up by specified delay cycle time (const int led_delay)
void IncrementDown(uint8_t, uint8_t);					//Increments specific LED down by specified delay cycle time (const int led_delay)
void Init_Timer1_PWM(void);								//Timer1 for PWM on BASS and MID
void Init_Timer2_PWM(void);								//Timer2 for PWM on TREBLE
void Init_Timer0_delay(void);							//Timer0 delay initialization
void delay_ms(int);										//New delay function for delay ms
void delay_us(int);										//New delay function for delay us
void Init_IO(void);										//See actual function for IO Port Description
void SolidColorOnLed(void);								//Changes between colors on LED
void ReadMCUSR(void);

int main(void)
{
	ReadMCUSR();							//gets info about possible resets
	init_ADC();								//init ADC for channel 0, 8 bit resolution, ADC clock = clk/128
	Init_Timer1_PWM();						//timer1 and 2 as 8 bit output compares
	Init_Timer2_PWM();
	Init_IO();								//enables things and interrupts
	EIFR |= (1 << INTF1)|(1 << INTF0);		//clears things
	WDTCSR &= (1 << WDIE);
	sei();									//enables global interrupts	
				
    while(1)
    {
		FadeOnLed();

		/*
		switch(display_select)
		{
			case 0x00:
					while (1)
					{
						MusicOnLed();
					}
			break;
			case 0x01:
				while (1)
				{
					FadeOnLed();
				}
			break;
			case 0xFF:			//standby glow faint purple
				switch(status_reg_read)
				{
					case 0b11110000:		//if no flags were set, output GREEN
						RED = 0;
						GREEN = 1;
						BLUE = 0;
					break;
					case 0b11110001:		//power on reset flag is active, output BLUE
						RED = 0;
						GREEN = 0;
						BLUE = 1;
					break;
					case 0b11110010:		//external reset flag is active, output PURPLE
						RED = 1;
						GREEN = 0;
						BLUE = 1;
					break;
					case 0b11110100:		//brown out reset flag is active, output RED
						RED = 1;
						GREEN = 0;
						BLUE = 0;
					break;
					case 0b11111000:		//watchdog reset flag is active, output YELLOW
						RED = 1;
						GREEN = 1;
						BLUE = 0;
					break;
					default:				//otherwise clear values
						RED = 0;
						GREEN = 0;
						BLUE = 0;
					break;
				}
			break;
			
		}	
		*/
	}
}

void ReadMCUSR(void)
{
	//stores info about what recently reset the micro
	status_reg_read = MCUSR;
	//cleara all flags in MCUSR
	MCUSR = 0x0;	
}

void Init_Timer0_delay(void)
{
	//clk / 8 prescalar means 2 overflows per 1 usec
	TCCR0B |= (1 << CS01);			//set Timer0 for clk/8 prescalar 
	TIMSK0 |= (1 << TOIE0);			//set Timer0 overflow interrupt enable	
	TCNT0 = 0;						//init timer to 0
}

void delay_us(int delay_value)
{
	TCNT0 = 0;					//initialize timer0 to 0
	timer0_overflow_cnt = 0;	//initialize overflow counter to 0
	
	while(timer0_overflow_cnt < (2 * delay_value))
	{
		//do nothing until delay time is achieved 
	}
	
	TCNT0 = 0;					//once done, clear timer register
	timer0_overflow_cnt = 0;	//reset counter variable
}

void delay_ms(int delay_value)
{
	delay_us(1000 * delay_value);		//delay
}

void IncrementUp(uint8_t led, uint8_t max_val)
{
	uint8_t i = 0;		//LCV

	while((i < max_val) & (display_select != 0x00))
	{
		switch(led)
		{
			case 1:
			RED		= i;
			break;
			case 2:
			GREEN	= i;
			break;
			case 3:
			BLUE	= i;
			break;
		}
		i++;
		_delay_ms(led_delay);
	}
}
void IncrementDown(uint8_t led, uint8_t max_val)
{
	uint8_t i = 0;		//LCV

	while((i < max_val) & (display_select != 0x00))
	{
		switch(led)
		{
			case 1:
			RED		= max_val - i;
			break;
			case 2:
			GREEN	= max_val - i;
			break;
			case 3:
			BLUE	= max_val - i;
			break;
		}
		i++;
		_delay_ms(led_delay);
	}
}

void GetAudioBandLevel(void)
{
	uint8_t audio_band = 0;
	DDRD	|=	(1 << STROBE)|(1 << EQ_RESET);				//PORTD bit strobe and reset pins output
	PORTD	&=	~((1 << STROBE)|(1 << EQ_RESET));			//sets strobe and reset low
	PORTD	|=	(1 << EQ_RESET);							//reset goes high
	_delay_us(100);											//delay 100usec for setup time req
	PORTD	|=	(1 << STROBE);								//strobe goes high
	_delay_us(50);											//strobe delays
	PORTD	&=	~(1 << STROBE);								//strobe goes low
	_delay_us(50);											//strobe delays
	PORTD	&=	~(1 << EQ_RESET);							//reset reset
	PORTD	|=	(1 << STROBE);								//strobe goes high
	_delay_us(50);				
		
	for(audio_band = 0; audio_band < 7; audio_band++)
	{
		//Commented out for debugging only
		
		PORTD	&=	~(1 << STROBE);				//resets (set strobe pin low (active))
		_delay_us(30);							//setup time for capture
		AudioLevel[audio_band] = read_ADC();	//reads 8 bit resolution audio level from audio bandpass filter
		PORTD	|=	(1 << STROBE);					//sets (set strobe pin high again)
		_delay_us(50);							//not sure if needed - check datasheet
	}
}

void MusicOnLed(void)
{
	
	GetAudioBandLevel();
	out_low		= (AudioLevel[0] + AudioLevel[1]) / 2;						// Average of two Lowest Bands
	out_mid		= (AudioLevel[2] + AudioLevel [3] + AudioLevel[4]) / 3;		// Average of three Middle Bands
	out_high	= (AudioLevel[5] + AudioLevel[6]) / 2;						// Average of two Highest Bands
	
	float low_multiplier	= 1.5;
	float mid_multiplier	= 1;
	float high_multiplier	= 1;
		
	uint8_t led_out_low		=	out_low * low_multiplier;
	uint8_t led_out_mid		=	out_mid * mid_multiplier;
	uint8_t led_out_high	=	out_high * high_multiplier;
	
	if(out_low >= (255 / low_multiplier))
	{
		led_out_low = 255;		//set to max brightness if overflow condition would otherwise occur
	}
	if(out_mid >= (255 / mid_multiplier))
	{
		led_out_mid = 255;		//set to max brightness if overflow condition would otherwise occur
	}
	if(out_high >= (212 / high_multiplier))
	{
		led_out_high = 255;		//set to max brightness if overflow condition would otherwise occur
	}
	
	//assign to outputs
	GREEN	= led_out_low;
	RED		= led_out_mid;
	BLUE	= led_out_high;
	//_delay_ms(10);			//old delay before menu implementation
	
	//_delay_ms(5);
	
}

void Init_IO(void)
{
	DDRB = 0x3F;											//portb as output
	
	//SMCR |= (1 << SM0);									//Sets sleep mode 'ADC Noise Reduction'
	
	DDRD	&= ~(1 << SW0) | ~(1 << SW1) | ~(1 << SW2);		//portd bits 2, 3, and 4 as inputs (pushbuttons)
	//DDRD	|= (1 << SW0) | (1 << SW1) | (1 << SW2);		//ddrd set high
	PORTD	|= (1 << SW0) | (1 << SW1) | (1 << SW2);		//turn pullups on only on pushbutton input pins
	//PORTD	&= (1 << SW0) | (1 << SW1) | (1 << SW2);		//sink mode
	
	//EICRA	= 0x0;											//Enables active LOW triggering on ext. interrupts on INT0 and INT1 pins (SW0 and SW1)
	EICRA	|= (1 << ISC11) | (1 << ISC01);					//Falling edge interrupt triggering
	EIMSK	|= (1 << INT1) | (1 << INT0);					//Enables external interrupts on INT0 and INT1 (SW0 and SW1)	
		
	//PCMSK2	= 0x10;										//Enable interrupts on PCINT20 (SW2)
	//PCICR	|= (1 << 2);									//Enables interrupts on pin change request 2 reg	
		
	DDRC	= 0x00;											//port c as input
	PORTC	= 0x3F;											//pullups on
}

void Init_Timer1_PWM(void)
{
	//Sets timer 1 clear on compare match (output to low level) for output compares and b
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1)|(1 << WGM10);
	//Set prescalar to clk/256
	TCCR1B |= (1 << CS12);
	//Enables interrupts for OCR1A and OCR1B
	TIMSK1 |= (1 << OCIE1B)|(1 << OCIE1A);	
	
}

void Init_Timer2_PWM(void)
{
	TCCR2A |= (1 << COM2A1)|(1 << COM2B1)|(1 << WGM20);
	TCCR2B |= (1 << CS22)|(1 << CS21);
	//Enables interrupts for OCR2B
	TIMSK2 |= (1 << OCIE2A);
}

void FadeOnLed(void)
{
	uint8_t increment_amount = 120;
	GREEN	= 0;
	BLUE	= 0;
	RED		= increment_amount;
	//see led_delay for led delay time
	//RED is defined as 1, GREEN as 2, BLUE as 3 when passing values to function
	//while (display_select == 0x01)
	while(1)
	{
		IncrementUp(2, increment_amount);
		IncrementDown(1, increment_amount);
		IncrementUp(3, increment_amount);
		IncrementUp(1, increment_amount);
		IncrementDown(2, increment_amount);
		IncrementDown(1, increment_amount);
		IncrementDown(3, increment_amount);
		IncrementUp(1, increment_amount);
		IncrementUp(2, increment_amount);
		IncrementDown(2, increment_amount);
	}
}

void StrobeOnLed(uint8_t red, uint8_t green, uint8_t blue)
{
	RED		= red;
	GREEN	= green;
	BLUE	= blue;
	_delay_ms(100);
	RED		= 0;
	GREEN	= 0;
	BLUE	= 0;
}

void SolidColorOnLed(void)
{
	RED = 0;
	GREEN = 100;
	BLUE = 0;
}

ISR(INT0_vect)
{
	//display_select = 0x00;
	//second_disp_sel = 0x00;
	while(1)
	{
		MusicOnLed();
	}
}

ISR(INT1_vect)
{
	//display_select = 0x01;
	//second_disp_sel = 0x01;
	FadeOnLed();
}

/*
ISR(TIMER0_OVF_vect)
{
	timer0_overflow_cnt++;
}
*/
/*
ISR(PCINT2_vect)		//PCINT20
{
	display_select++;
}
*/

/*
ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << 1);
}

ISR(TIMER1_COMPB_vect)
{
	PORTB ^= (1 << 2);
}

ISR(TIMER2_COMPA_vect)
{
	PORTB ^= (1 << 3);
}
*/
