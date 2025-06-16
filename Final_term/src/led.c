#include "led.h"
#include "timer.h"
#include "stm32f4xx.h"

void led_init(void) {
    // Set PC0-PC2 as output
    for (int i = 0; i <= 4; i++) {
        GPIOC->MODER &= ~(0b11 << (i * 2));
        GPIOC->MODER |=  (0b01 << (i * 2));  // Output mode
        GPIOC->OTYPER &= ~(1 << i);         // Push-pull
        GPIOC->PUPDR &= ~(0b11 << (i * 2));  // No pull-up/down
    }

}

void led_on(uint8_t led_num) {
    GPIOC->ODR |= (1 << led_num);
}

void led_off(uint8_t led_num) {
    GPIOC->ODR &= ~(1 << led_num);
}




