#include "stm32f4xx.h"
#include "button.h"

void init_button(void) {
    RCC->APB2ENR |= 1 << 0; // Enable SYSCFG clock

    GPIOA->MODER &= ~(3 << (1 * 2)); // Set PA1 as input
    GPIOA->MODER &= ~(3 << (2 * 2)); // Set PA2 as input

    GPIOA->PUPDR &= ~(3 << (1 * 2)); // Clear pull-up/pull-down bits for PA1
    GPIOA->PUPDR &= ~(3 << (2 * 2)); // Clear pull-up/pull-down bits for PA2

    // Set PA1 and PA2 to pull-down
    GPIOA->PUPDR |= (2 << (1 * 2)); // Set pull-down for PA1
    GPIOA->PUPDR |= (2 << (2 * 2)); // Set pull-down for PA2

    // Clear EXTI line configuration for PA1 (EXTI1) and PA2 (EXTI2)
    SYSCFG->EXTICR[0] &= ~(0xF << 4); // Clear EXTI1 configuration
    SYSCFG->EXTICR[0] |= (0x0 << 4);  // Set EXTI1 to PA1

    SYSCFG->EXTICR[0] &= ~(0xF << 8); // Clear EXTI2 configuration
    SYSCFG->EXTICR[0] |= (0x0 << 8);  // Set EXTI2 to PA2

    EXTI->IMR |= (1 << 1) | (1 << 2);  // Unmask EXTI line 1 & 2
    EXTI->FTSR &= ~((1 << 1) | (1 << 2)); // Disable falling edge trigger for both lines
    EXTI->RTSR |= (1 << 1) | (1 << 2); // Enable on rising edge for line 1 & 2
}
