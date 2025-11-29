#include "stm32f4xx.h"

void ADC1_Init(void) {
    RCC->APB2ENR |= (1 << 8);         

    GPIOA->MODER |= (3 << 0);          

    ADC1->CR2 = 0;

    ADC1->SQR3 = 0;

    ADC1->SMPR2 |= (7 << 0);       

    ADC1->CR2 |= (1 << 0);       
}

uint16_t ADC1_Read(void) {
    ADC1->CR2 |= (1 << 30);          
    while (!(ADC1->SR & (1 << 1)));   

    return ADC1->DR;
}
