/*
 * ADC.c
 *
 * Driver for the Analog to Digital Converter. 
 *
 * Created: 5/18/2013 8:27:17 AM
 * Author: Dal
 */ 

#include "MainDef.h"
#include "ADC.h"

// -------------------------------------------------------
// ADC_Enable()
// Enables the ADC for use.   Requires power. Also, the
// ADC should be enabled before calling the conversion
// routines.

void ADC_Enable()
{
    ADCSRA =
        (1 << ADEN)  |  // Enable the ADC unit
        (0 << ADSC)  |  // Don't start conversion yet
        (0 << ADATE) |  // Don't use auto triggering yet
        (1 << ADIF)  |  // Clear interrupt flag (by writing 1 to it!)
        (0 << ADIE)  |  // Not using interrupts in this implementation
        (0x05 << ADPS0) ;  // Set Freq prescaler to divide by 16

    ADCSRB = 0x00;  // Free Running Mode (Ignored, cause ADATE=0)
    ADMUX =
        (0x03 << REFS0) | // Use Internal 2.56V Reference
        (0 << ADLAR)    | // Set for right justified data
        (0x00 << MUX0) ;  // The Mux selection.  Can be changed later.
}

// -------------------------------------------------------
// ADC_GetBatteryVoltage()
// Returns raw battery voltage, in mVolts.
int16_t ADC_GetBatteryVoltage()
{
	// The raw battery input is put into a 10K/2.2K voltage divider before being presented to the ADC.
	// Therefore, the bat voltage is multiplied by 2.2/(10+2.2) = 2.2/12.2 = 0.1803
	// Max reading is 1023 which means 2.56v.  Therefore, to convert to mV, scale by: 2560/1023 = 2.5.
	// Now, we want the voltage before the 10K/2.2K voltage divider.  Therefore scale again
	// by 12.2/2.2 = 5.55. (By measurement, it is actually 5.50).  Finally, total scale factor
	// to apply is 13.875. (Or, by measurement 13.763). This will convert the
	// ADC reading to mV.
	
    int16_t d = ADC_GetConversion(ADC_BattVoltage);   // Gets the voltage reading.
	int16_t d2 = d >> 1;
	int16_t d4 = d >> 2;
	int16_t d8 = d >> 3;
	int16_t d16 = 0; //d >> 4;
	d = (d * 13) + d2 + d4 + d8 + d16;
    return d;
}

int16_t ADC_GetPot()
{
	// The Pot is a simple voltage diver, from 0 to 3.3 volts.
	int16_t d = ADC_GetConversion(ADC_PotInput);	// Gets the pot setting.
	return d;
	//if (d != f) {
		//refreshTimer()
	//}
}

// -------------------------------------------------------
// ADC_GetCurrent()
// Returns current in 100th of Amps.
int16_t ADC_GetCurrent()
{
	// The current is run though a 0.01 ohm shut resistor.  Therefore, 100 Amps 
	// would produce one volt.  Full scale is 2.56 volts or 256 Amps.  The 
	// reading at full scale is 1023.  Therefore the LSB = 256Amps/1023 =
	// 0.25amps or 250 mA.
	int16_t d = ADC_GetConversion(ADC_CSense);
	d = 25*d;  // for 100th of amp, not mA.
    return d;
 }

 // -------------------------------------------------------
 // ADC_GetConversion()
 // Reads one value from the ADC converter and returns it.
 // Blocks until conversion is done -- usually takes about
 // 47uS with the prescaler set to divide by 32 and the 
 // system clock at 10Mhz.  (Its 30us for a division by 16).
 // Assumes that ADC_Enable() has been called first.
 
 int16_t ADC_GetConversion(uint8_t iChannel)
 {
    ADMUX = (ADMUX & 0xF0) | iChannel;
    BitOn(ADCSRA, ADSC);              // Start conversion.
    while(!BitTest(ADCSRA, ADIF))  ;  // Wait here till conversion is done.
    int16_t Data = ADC;               // Grab the data
    BitOn(ADCSRA, ADIF);              // Clear the "done" flag
    return Data;
 }