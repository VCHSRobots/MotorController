/*
 * ADC.h
 *
 * Created: 5/18/2013 8:50:58 AM
 * Author: Dal
 */ 

#ifndef ADC_H_
#define ADC_H_

void ADC_Enable();
int16_t ADC_GetBatteryVoltage();
int16_t ADC_GetPot();
int16_t ADC_GetCurrent();
int16_t ADC_GetConversion(uint8_t iChannel);

#endif /* ADC_H_ */

