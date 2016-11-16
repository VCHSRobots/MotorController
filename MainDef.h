/*
 * MainDef.h
 *
 * Master definitions that apply to the entire application, including 
 * hardware.
 *
 * This version for Epic Robot's Motor Controller, Version 1. Nov 2015.
 *
 * Created: 12/12/15 DLB
 */ 

#ifndef MAINDEF_H_
#define MAINDEF_H_

// The CPU Clock frequency of the V0 board.
#define F_CPU 10000000UL            // Used everywhere!

// Standard include files for AVR chips.
#include <avr/io.h>                 // Defines all the registers, bits, and ports for the processor
#include <avr/pgmspace.h>           // Macros for dealing with program memory
#include <avr/interrupt.h>          // Defines and macros for interrupts
#include <avr/eeprom.h>             // Defines and macros for using the on-chip EEPROM
#include <avr/wdt.h>                // Watchdog macros
#include <util/atomic.h>            // For atomic operations
#include <util/delay.h>             // Macros for delay operations
#include <avr/sleep.h>              // Macros for sleep operations
#include <stdlib.h>                 // Standard C library

// Defines for PORT A, used as A/D.
#define ADC_BattVoltage 0
#define ADC_PotInput    1
#define ADC_CSense      2

// Defines for PORT B pins.
#define LedPin        0   // Led (output)
#define BSpare1       1   // Unused
#define BSpare2       2   // Unused
#define BSpare3       3   // Unused
#define BSpare4       4   // Unused
#define MOSIPin       5   // SPI for SD Reader and programming interface
#define MISOPin       6   // SPI for SD Reader and programming interface
#define SCKPin        7   // SPI for SD Reader and programming interface

// Defines for PORT C pins.
#define Sw_MOnOffPin    0   // Switch on Pot, to control Motor Power Relay (input)
#define Sw_PwrOnOffPin  1   // Power On/Off Switch (input)
#define Button_0        2   // Button (input)
#define Button_1        3   // Button (input)
#define Button_2        4   // Button (input)
#define PwrOnRlyPin     5   // Controls power relay (output)
#define MotorRlyPin     6   // Controls motor relay (output)
#define CSpare0         7   // Unused

// Defines for PORT D
#define LcdSclPin     0   // Unused
#define LcdSiPin      1   // LCD control
#define DSpare2       2   // Unused
#define DSpare3       3   // Unused
#define PWMPin        4   // PWM to control motor speed (output)
#define LcdCsPin      5   // LCD control
#define LcdResetPin   6   // LCD control
#define LcdA0Pin      7   // LCD control

// Standard booleans that DAL likes:
typedef uint8_t bool8;
#define True 1
#define False 0

// Useful Macros 
#define BitOn(Pt, Bt) (Pt |= _BV(Bt))               // Turn a PORT bit On
#define BitOff(Pt, Bt) (Pt &= ~(_BV(Bt)))           // Turn a PORT bit Off
#define BitTest(v, Bt) (v & _BV(Bt))                // Tests a bit
#define LedRedOn() BitOn(PORTB, LedPin)             // Turns the on-board LED on.
#define LedRedOff() BitOff(PORTB, LedPin)           // Turns the on-board LED off.
#define GetButtons() ((~PINC) & (_BV(Button_0) | _BV(Button_1) | _BV(Button_2) ))		// Gets the bit pattern that represents the button press condition.

#define PwrRelayOn() BitOn(PORTC, PwrOnRlyPin)      // Turns the power on
#define PwrRelayOff() BitOff(PORTC, PwrOnRlyPin)    // Turns the power off
#define MotorRelayOn() BitOn(PORTC, MotorRlyPin)      // Turns the power on
#define MotorRelayOff() BitOff(PORTC, MotorRlyPin)    // Turns the power off


// General routines.  
void V0HardwareSetup();			// Call this for basic setup at power on.
void SetupFastTimer();	        // Normally called at power on.
void EnableFastTimerInterrupt(void (*pCallback)(), uint16_t Ticks);
#define DisableFastTimerInterrupt() BitOff(TIMSK1, OCIE1A);
#define GetFastTime() (TCNT1)     // Returns the Timer1 Count.

#ifdef MainFile
#define EXTERN
#else
#define EXTERN extern
#endif

// These are handy macros to define EEPROM variables in H files with initialization.
#ifdef MainFile
#define EEu8(name, v) EEMEM uint8_t name = v
#define EEu16(name, v) EEMEM uint16_t name = v
#define EEu32(name, v) EEMEM uint32_t name = v
#define EEi8(name, v)  EEMEM int8_t name = v
#define EEi16(name, v) EEMEM int16_t name = v
#define EEi32(name, v) EEMEM int32_t name = v
#else
#define EEu8(name, v) extern EEMEM uint8_t name
#define EEu16(name, v) extern EEMEM uint16_t name
#define EEu32(name, v) extern EEMEM uint32_t name
#define EEi8(name, v) extern EEMEM int8_t name
#define EEi16(name, v) extern EEMEM int16_t name
#define EEi32(name, v) extern EEMEM int32_t name
#endif

// These are handy macros to define Ram variables in H files with initialization.
#ifdef MainFile
#define RAMu8(name, v)   uint8_t name = v
#define RAMu16(name, v)  uint16_t name = v
#define RAMu32(name, v)  uint32_t name = v
#define RAMi8(name, v)   int8_t name = v
#define RAMi16(name, v)  int16_t name = v
#define RAMi32(name, v)  int32_t name = v
#else
#define RAMu8(name, v)  extern uint8_t name
#define RAMu16(name, v) extern uint16_t name
#define RAMu32(name, v) extern uint32_t name
#define RAMi8(name, v)  extern int8_t name
#define RAMi16(name, v) extern int16_t name
#define RAMi32(name, v) extern int32_t name
#endif

#endif /* MAINDEF_H_ */