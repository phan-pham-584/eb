#include "stm32f4xx.h"
#include "adc.h"

void ADC1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Enable ADC1 clock
    GPIOA->MODER |= (3U << 0); // PA0 analog mode

    ADC1->CR2 = 0; // Disable ADC for config
    ADC1->SQR3 = 0; // Channel 0 (PA0)
    ADC1->SMPR2 |= (7U << 0); // Sampling time
    ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC
}

uint16_t ADC1_Read(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART; // Start conversion
    while (!(ADC1->SR & ADC_SR_EOC)); // Wait
    return ADC1->DR;
}

// Read ADC every 500 ms using Timer4 with edge-triggered interrupt
void Timer4_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; // Enable TIM4 clock
    TIM4->PSC = 15999; // Prescaler (16 MHz / (15999 + 1) = 1 kHz → 1 ms)
    TIM4->ARR = 500; // Auto-reload at 500 ms
    TIM4->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM4->CR1 |= TIM_CR1_CEN; // Enable TIM4
}




