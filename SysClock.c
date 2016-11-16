/*
 * SysClock.c
 *
 * Modified Sys clock, where interrupt is triggered approximately every
 * 0.020 seconds.  Upon trigger, interrupts are turned off for up to 2ms, to 
 * accomplish generating the PWM signal.   
 *
 * Time is a
 *
 
 * Provides a system clock that is accurate to plus or minus 1ms. 
 * Uses hardware Timer0 and interrupts to accomplish this.  Interrupt is
 * triggered every 0.0128 seconds, or about 78 Hz.  I.e., this clock uses
 * very little overhead.
 *
 * Also provides a hook into the actual timer interrupt routine.  A call back
 * can be configured to trigger at a given time.  Either way, to avoid a
 * linker error, the routine SlowInterrupt() must be defined in external code.
 *
 * Note that before calling SlowInterrupt, the global interrupts are enabled.
 * Therefore, SlowInterrupt should be fairly quick to avoid deeply nested interrupt
 * processing.
 *
 * NOTE: this code was taken from the KK board stuff that operated with a 20MHz
 * crystal.  This version works with any F_CPU greater than 80KHz.  However, 
 * accuracy is only maintained when F_CPU is evenly divisible by 80K.
 *
 * Created: 4/18/2013 11:07:59 AM
 * Author: Dal
 */ 

#include "MainDef.h"
#include "SysClock.h"

volatile uint32_t gTime = 0;    // System Time Counter.  LSB = 10ms. 

volatile bool8  gbEnableSlowInterrupt = False;  // Indicates that the slow interrupt is enabled.
volatile uint32_t gNextSlowInterrupt        = 0;  // Time that the slow interrupt will trigger.

// --------------------------------------------------------
// SetupSystemTimeCounter()
// Sets up Timer0 to trigger once every 0.020 seconds.
void SetupSystemTimeCounter()
{
    // On overflow, the Timer will trigger an interrupt.
    // Input to this 8-bit timer will be system clock (F_CPU) / 1024.  To
    // calculate the overflow count, OCR0A, to get close to 0.020 seconds,
	// we have:  0.020secs = OCR0A * (1024 / F_CPU).
	// Or OCR0A = (F_CPU / 1024) * 0.020
	//          = (F_CPU / 51200)
	// Timer counts at a rate of F_CPU/1024 = 9.765625 KHz.  Period is 0.1ms.

    OCR0A = (uint8_t) (F_CPU / 51200UL);   //=195 for 10MHz, which is under 8 bits.
	OCR0B = 0;             // Not used, but set for readability.
	TCCR0A = 0b00000010;   // Selects Clear Timer on Compare mode
	TCCR0B = 0b00000101;   // This divides the System Clock by 1024, as input to the timer.
	// Now, set up the overflow Interrupt for Timer 1
	TIMSK0 |= _BV(OCIE0A); // Enable a Match to OCR0A interrupt.
}

// --------------------------------------------------------
// ISR()
// Interrupt on Timer0 compare.
// Come here to track system time.  Resolution is 0.020 seconds -- see setup.
ISR(TIMER0_COMPA_vect)
{
	gTime++;			// This adds 10ms to the time.
	gTime++;            // And another 10ms to the time. 
	if(gbEnableSlowInterrupt)
	{
		if(gTime >= gNextSlowInterrupt)
		{
			gbEnableSlowInterrupt = False;
			sei();                           // Interrupts are enabled during processing of SlowInterrupt()!
			SlowInterrupt();
		}
	}
}

// --------------------------------------------------------
// GetSystemTime()
// Returns the system clock, accurate to 1ms.  LSB = 1ms.
// Starts at zero on powerup, and then rolls over about once
// every 50 days.
uint32_t GetSystemTime()
{
	// Take a snapshot of all the relevant registers.
    
    uint32_t v;
    uint16_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
	    v = gTime;
	    t = TCNT0;
    }
	uint32_t vout = v*10; // + (975*(uint32_t)t)/100L;
	return vout;
}	


// --------------------------------------------------------
// SetSlowInterrupt()
// Sets the slow interrupt to trigger when the given time
// occurs.  After trigger, the slow interrupt is disabled.
// Note, NextTime is specified in milliseconds, but that the
// actual trigger will be accurate only to tens of milliseconds.
void SetSlowInterrupt(uint32_t NextTime)
{
	gbEnableSlowInterrupt = True;
	gNextSlowInterrupt = NextTime / 10;
}

// --------------------------------------------------------
// ClearSlowInterrupt()
// Turns off any pending slow interrupt.  Note, that when the 
// executing thread enters this routine, a slow interrupt might
// be on the verge of being triggered, and if so, the interrupt
// routine might be allowed to run before this routine returns.
// HOWEVER, after this routine has returned, the caller can be
// assured that no further slow interrupt will be triggered
// until the slow interrupt is re-enabled.
void ClearSlowInterrupt()
{
	gbEnableSlowInterrupt = False;
	gNextSlowInterrupt = 0;
}

// --------------------------------------------------------
// TimeTillSlowInterrupt()
// Gets the time remaining, in milliseconds, until the next
// slow interrupt will be fired.  If no interrupt is set to
// go off then SLOW_INTR_OFF is returned.  If the interrupt
// is in the process of being triggered, then SLOW_INTR_PROCESSING
// is returned.  Otherwise a number greater than 5 is returned.
uint32_t TimeTillSlowInterrupt()
{
	if(!gbEnableSlowInterrupt) return SLOW_INTR_OFF;
	uint32_t tTime = GetSystemTime();
	uint32_t tTrigger = gNextSlowInterrupt * 10;
	if(tTime > tTrigger) return SLOW_INTR_PROCESSING;
	uint32_t d = tTrigger - tTime;
	if(d <= 5) return SLOW_INTR_PROCESSING;
	return d;
}	
