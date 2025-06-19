#include "stm32f4xx.h"
#include "button.h"

#include "stm32f4xx.h"
#include "button.h"

void init_button(void) {
    RCC->APB2ENR |= (1 << 14);             
    RCC->AHB1ENR |= (1 << 0) | (1 << 1);    

    GPIOA->MODER &= ~(3 << (3 * 2));         
    GPIOA->PUPDR &= ~(3 << (3 * 2));         
    GPIOA->PUPDR |=  (1 << (3 * 2));        

    GPIOB->MODER &= ~(3 << (1 * 2));         
    GPIOB->PUPDR &= ~(3 << (1 * 2));         
    GPIOB->PUPDR |=  (1 << (1 * 2));         

    SYSCFG->EXTICR[0] &= ~(0xF << 4);        
    SYSCFG->EXTICR[0] |=  (0x1 << 4);        

    SYSCFG->EXTICR[0] &= ~(0xF << 12);       
    SYSCFG->EXTICR[0] |=  (0x0 << 12);

    EXTI->IMR  |= (1 << 1) | (1 << 3);

    NVIC->IP[7] = 7 << 4; 
    NVIC->IP[9] = 9 << 4;

    EXTI->FTSR |= (1 << 1) | (1 << 3);
    EXTI->RTSR &= ~((1 << 1) | (1 << 3));

    NVIC->ISER[0] |= (1 << 7);  
    NVIC->ISER[0] |= (1 << 9);  
}

