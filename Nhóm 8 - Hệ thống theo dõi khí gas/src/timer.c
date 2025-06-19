#include "stm32f4xx.h"
#include "timer.h"

void Timer2_Init(void)
{
    RCC->APB1ENR |= (1 << 0);      

    TIM2->PSC = 15999;              
    TIM2->ARR = 1000;               
    TIM2->CNT = 0;                 

    TIM2->CR1 |= (1 << 0);    
}      

void Delay_Timer2(uint16_t ms)
{
    TIM2->CNT = 0;
    while (TIM2->CNT < ms);         
}

void PB8_Init(void) {
    RCC->AHB1ENR |= (1 << 1);       

    GPIOB->MODER &= ~(3 << 16);     
    GPIOB->MODER |=  (2 << 16);    

    GPIOB->AFR[1] &= ~(15 << 0);   
    GPIOB->AFR[1] |=  (3 << 0);   
}

void Timer10_Init(uint32_t arr) {
    RCC->APB2ENR |= (1 << 17);      

    TIM10->PSC = 1599;             
    TIM10->ARR = arr - 1;
    TIM10->CCR1 = 1000;

    TIM10->CCMR1 &= ~(7 << 4);      
    TIM10->CCMR1 |=  (6 << 4);     

    TIM10->CCMR1 |= (1 << 3);      

    TIM10->CCER |= (1 << 0);       
    TIM10->CR1  |= (1 << 0);       
}

void Timer10_SetFreqz(uint32_t arr) {
    TIM10->CNT = 0;

    TIM10->ARR = arr - 1;

    TIM10->CCER |= (1 << 0);        
    TIM10->CR1  |= (1 << 0);     
    
}

void Timer10_Stop(void) {
    TIM10->CCER &= ~(1 << 0);      
    TIM10->CR1  &= ~(1 << 0);       
    TIM10->EGR |= (1 << 0);         
}

