#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "stm32f4xx.h"

void led_init(void);
void led_on(uint8_t led_num);
void led_off(uint8_t led_num);
void led_blink(uint8_t led_num, uint32_t ms);

#endif
