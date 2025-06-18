#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include "stm32f4xx.h"


void Timer2_Init();
void Delay_Timer2(uint32_t ms);
void Timer10_Init(uint32_t arr);
void Timer10_SetFreqz(uint32_t arr);
void Timer10_Stop(void);
void PB8_Init(void);
void PB8_Stop(void);
void Timer4_Init(void);


#endif