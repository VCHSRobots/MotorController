/*
 * PWM.h
 *
 * Created: 12/24/2015 9:54:55 PM
 *  Author: Dal
 */ 


#ifndef PWM_H_
#define PWM_H_

void PWM_Init();
void PWM_On(uint16_t freq, uint16_t width);
void PWM_SetWidth(uint16_t width);
void PWM_Off();

#endif /* PWM_H_ */