/*
 * SysClock.h
 *
 * Provides a system clock that counts in 1 ms ticks.  Roll over is 50 days.
 *
 * Created: 4/18/2013 11:08:17 AM
 * Author: Dal
 */ 


#ifndef SYSCLOCK_H_
#define SYSCLOCK_H_

#define SLOW_INTR_OFF  0  
#define SLOW_INTR_PROCESSING 1

void SetupSystemTimeCounter();
uint32_t GetSystemTime();
void SetSlowInterrupt(uint32_t NextTime);
void ClearSlowInterrupt();
uint32_t TimeTillSlowInterrupt();
void SlowInterrupt();						// This must be implemented by user.

#endif /* SYSCLOCK_H_ */