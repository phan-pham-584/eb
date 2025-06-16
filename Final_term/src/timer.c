#include "stm32f4xx.h"

void Timer2_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable TIM2 clock
    TIM2->PSC = 15999;                   // Prescaler (16 MHz / (15999 + 1) = 1 kHz → 1 ms)
    TIM2->ARR = 1000;                     // Auto-reload at 1000 ms (1 second)
    TIM2->CNT = 0;                        // Reset counter
    TIM2->CR1 |= TIM_CR1_CEN;             // Enable TIM2
}

void Delay_Timer2(uint16_t ms)
{
    TIM2->CNT = 0;  
    while(TIM2->CNT < ms);  // Wait for the counter to reach the desired delay
}


void PB8_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    GPIOB->MODER &= ~(3 << (8 * 2));
    GPIOB->MODER |=  (2 << (8 * 2));     // Alternate function

    GPIOB->AFR[1] &= ~(0xF << ((8 - 8) * 4));
    GPIOB->AFR[1] |=  (3 << ((8 - 8) * 4)); // AF3 = TIM10_CH1
}


void Timer10_Init(uint32_t arr) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

    TIM10->PSC = 15999;          // 84MHz / 16000 = ~5.25kHz
    TIM10->ARR = arr - 1;
    TIM10->CCR1 = arr/2 - 1;

    TIM10->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM10->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
    TIM10->CCMR1 &= ~TIM_CCMR1_OC1PE;
    

    TIM10->CCER |= TIM_CCER_CC1E;
    TIM10->CR1 |= TIM_CR1_CEN;
    
}


void Timer10_SetFreqz(uint32_t arr) {    

    TIM10->ARR = arr - 1;
    TIM10->CCR1 = arr/2 - 1;    

    TIM10->CCER |= TIM_CCER_CC1E;
    TIM10->CR1 |= TIM_CR1_CEN;
}


void Timer10_Stop(void) {
    TIM10->CCER &= ~TIM_CCER_CC1E;
    TIM10->CR1 &= ~TIM_CR1_CEN;
    TIM10->EGR = TIM_EGR_UG;
}


void PC10_Stop(void) {
    GPIOB->MODER &= ~(3 << (8 * 2));
    GPIOB->MODER |=  (1 << (8 * 2));     // Output

    GPIOB->OTYPER &= ~(1 << 8);          // Push-pull
    GPIOB->PUPDR  &= ~(3 << (8 * 2));    // No pull

    GPIOB->ODR &= ~(1 << 8);             // Force low
}

