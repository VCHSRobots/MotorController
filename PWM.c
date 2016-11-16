/*
 * PWM.c
 *
 * Created: 12/24/2015 9:54:07 PM
 * Updated: 10/24/2016
 *
 *  Author: Dal
 */ 

// Note, this module configures and controls pin OC1B, which is otherwise
// known as PD4.  On this pin, PWM is output.  If PWM is "off", then this
// pin is left floating, otherwise it is an output and controlled by this module.

#include "MainDef.h"
#include "PWM.h"
#define T1CLK (F_CPU/8)               // Input clock into counter (should be about 1.25MHz)
#define T1CLK_SCALED (T1CLK/50000)    // Scaled version of T1CLK
#define TSCALE 20                     // Other part of scale (x=1,000,000)
static bool8 s_bRunning = False;   // If not running, then the output pin is a simple GPIO, and driven low.
static uint16_t s_iFreq = 50;      // Frequency of PWM in HZ.
static uint16_t s_iWidth = 1500;   // Width of pwm pulse in usecs. Can run from about 500us to 2500us...

// Init the PWM output pin.  Leaves it in a floating state until the
// PWM is turned on with PWM_On().
void PWM_Init()
{
	//BitOff(PORTD, PWMPin);
	BitOff(DDRD, PWMPin);  
	s_bRunning = False;
}

// Turn on the PWM and set its initial frequency in Hz, and pulse width
// in usec.  The pulse width should be between 500 and 2500 usec.
void PWM_On(uint16_t freq, uint16_t width)
{
	s_iFreq = freq;
	s_iWidth = width;
	
	uint16_t topcnt = T1CLK / s_iFreq;            // Calculate top count
	uint16_t pulsecnt = s_iWidth * T1CLK_SCALED / TSCALE;  // Calculate pulse count 

    // Set up the control registers to output on the OC1B pin, and not
	// use the OC1A pin, running fast PWM (Mode=15).  In this mode, OCR1A
	// is used to set the count TOP, and OCR1B is used to set the pulse width.
	
	// Start with everything off.
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TIMSK1 = 0;  // Not using interrupts
	
	// Start at zero count
	TCNT1 = 0;
	
	// Set the top count and the width count
	OCR1A = topcnt;
	OCR1B = pulsecnt;

	// Set up the control registers
	TCCR1A|=(0<<COM1A1)|(0<<COM1A0);  // Don't use OC1A pin
	TCCR1A|=(1<<COM1B1)|(0<<COM1B0);  // Use OC1BW with compare match in non-inverted mode
	TCCR1A|=(1<<WGM11)|(1<<WGM10);    // First Part of mode bits for mode 15
	TCCR1B|=(1<<WGM13)|(1<<WGM12);    // Second part of mode bits for mode 15
	TCCR1B|=(0<<CS12)|(1<<CS11)|(0<<CS10);  // Clock select, divide by 8     
	
	// At this point, PWM should be running
	BitOn(DDRD, PWMPin);
	s_bRunning = True;
}

// Sets the PWM width, given in usecs.
void PWM_SetWidth(uint16_t width)
{
	s_iWidth = width;	
	uint16_t pulsecnt = s_iWidth * T1CLK_SCALED / TSCALE;  // Calculate pulse count
	OCR1B = pulsecnt;
}

// PWM_Off() -- Turn off the PWM timmer, and set the output pin floating.
void PWM_Off()
{
	   //Configure TIMER1 to be Normal and Off
	   TCCR1A = 0;
	   TCCR1B = 0;
	   TCCR1C = 0;

		//Set output to be floating.
	   BitOff(DDRD, PWMPin);
	   s_bRunning = False;
}
