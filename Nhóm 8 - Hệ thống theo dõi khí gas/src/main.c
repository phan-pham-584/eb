#include "stm32f4xx.h"  
#include "adc.h"
#include "lcd.h"
#include "timer.h"
#include "led.h"
#include "button.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define RL 10000.0
#define R0 39000.0

volatile uint8_t state = 1;
volatile bool active_state = false;
volatile uint8_t uno = 0x00; 
uint16_t adc_value = 0;
volatile bool adc_display = 0;

void gpio_clock_init(void) {
    RCC->AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2);
}

void systick_init() {
    SysTick->LOAD  = 16000000 - 1;  
    SysTick->VAL   = 0;
    SysTick->CTRL  = (1 << 0)  | (1 << 1)  | (1 << 2);
    SCB->SHP[11] = (10 << 4);  
}

void SysTick_Handler(void) {
    
    adc_value = ADC1_Read();
    adc_display = true;
}



void system_display() {

    // Xấp xỉ giá trị ppm
    float Rs = RL * (4095.0 - adc_value) / adc_value;
    float ratio = Rs / R0;
    float m = -0.38;
    float b = 1.50;
    float ppm = pow(10, ((log10(ratio) - b) / m));

    static char adc_str[6];
    sprintf(adc_str, "%u", (unsigned int)ppm );  

    if (uno != ppm / 20000) {
        uno = ppm / 20000;
    }

    if (active_state == 0) {
        LCD_SendCommand(0x01); 
        LCD_SetCursor(1, 0);
        LCD_WriteString("System state: 1");
        LCD_SetCursor(2, 0);
        LCD_WriteString("PPM value: ");
        LCD_SetCursor(3, 0);
        LCD_WriteString("Gas state: ");
        active_state = 1;
        uno = 0;
    }

    if(adc_display) { // da sua
        LCD_SetCursor(2, 11);
        LCD_WriteString("         "); 
        LCD_SetCursor(2, 11);
        LCD_WriteString(adc_str);
        adc_display = false;
        if (ppm >= 60000) { 
            Timer10_SetFreqz(5000 - 4000 * (ppm - 60000) / 100000);
        }
        else if(ppm > 40000 && ppm < 60000) {
            Timer10_SetFreqz(10000);
        }
       
    }

    if (uno == 0x00) {
        LCD_SetCursor(3, 11);
        LCD_SendData('0');
        led_on(0);
        led_off(1); led_off(2); led_off(3); led_off(4);
        uno = 0x05;
        Timer10_Stop();
    } else if (uno == 0x01) {
        LCD_SetCursor(3, 11);
        LCD_SendData('1');
        led_off(0); led_on(1); 
        led_off(2); led_off(3); led_off(4);
        uno = 0x05;
        Timer10_Stop();
    } else if (uno == 0x02) {
        LCD_SetCursor(3, 11);
        LCD_SendData('2');
        led_off(0); led_off(1); led_off(2); led_off(3); led_off(4);
        uno = 0x05;
    } else if (uno >= 0x03) {
        LCD_SetCursor(3, 11);
        LCD_SendData('3');
        led_off(0); led_off(1); led_off(2); 
        led_on(3); led_on(4); 
        uno = 0x05;
    }

}


void EXTI3_IRQHandler(void) {
    if (EXTI->PR & (1 << 3)) {
        NVIC->ICER[0] |= (1 << 9); // da sua
        EXTI->PR |= (1 << 3);
        state = (state + 1) % 2; 
        active_state = false; 
        }
}


void EXTI1_IRQHandler(void) {
    if (EXTI->PR & (1 << 1)) {
        NVIC->ICER[0] |= (1 << 7);  // da sua
        EXTI->PR |= (1 << 1);
        state = 2;
    }
}

void system_init(void) {
    gpio_clock_init();
    systick_init();
    Timer2_Init();
    PB8_Init();
    Timer10_Init(1000);
    I2C1_GPIO_Init();
    I2C1_Init();
    LCD_Init();
    ADC1_Init();
    led_init();
    init_button();
    active_state = 0;
    adc_display = 0;
    state = 1;
    LCD_SendCommand(0x01);
    LCD_SetCursor(1, 0);
    LCD_WriteString("System init done");
    Delay_Timer2(1000);    
}

void enter_sleep_mode(void) {
    LCD_SendCommand(0x01);
    LCD_SetCursor(1, 0);
    LCD_WriteString("Enter sleep mode");
    led_on(2);
    Timer10_Stop();

    
    SCB->SCR &= ~(1 << 2);

    NVIC->ISER[0] |= (1 << 9); 

    SysTick->CTRL &= ~(1 << 1);
    ADC1->CR1 &= (1 << 0);


    __WFI(); 
    LCD_SendCommand(0x01);
    LCD_SetCursor(1, 0);
    LCD_WriteString("Exit sleep mode");
    Delay_Timer2(1000);

    ADC1->CR1 |= (1 << 0);
    SysTick->CTRL |= (1 << 1);
    NVIC->ISER[0] |= (1 << 7); 
    NVIC->ISER[0] |= (1 << 9); 
}


void system_reset(void) {
    state = 1;
    
    LCD_SendCommand(0x01);

    
    RCC->AHB1RSTR |= (1 << 0);  
    Delay_Timer2(1);
    RCC->AHB1RSTR &= ~(1 << 0);

    RCC->AHB1RSTR |= (1 << 2);  
    Delay_Timer2(1);
    RCC->AHB1RSTR &= ~(1 << 2);

  
    RCC->APB2RSTR |= (1 << 8);  
    Delay_Timer2(1);
    RCC->APB2RSTR &= ~(1 << 8);


    RCC->APB2RSTR |= (1 << 14);  
    Delay_Timer2(1);
    RCC->APB2RSTR &= ~(1 << 14);


    EXTI->IMR = 0;
    EXTI->RTSR = 0;
    EXTI->FTSR = 0;
    EXTI->PR = 0xFFFFFFFF;

    SYSCFG->EXTICR[0] &= ~(0xF << 4);  
    SYSCFG->EXTICR[0] &= ~(0xF << 12); 

    NVIC->ICER[0] = (1 << 7);  
    NVIC->ICER[0] = (1 << 9);  
    NVIC->ICPR[0] = (1 << 7);  
    NVIC->ICPR[0] = (1 << 9);  

    SysTick->CTRL  = 0;      
    SysTick->LOAD  = 0;      
    SysTick->VAL   = 0;      


    LCD_SetCursor(1,0);
    LCD_WriteString("System reset done");
    Delay_Timer2(1000);
}

volatile uint8_t reset_count = 0;

int main(void) {
    system_init();
    while (1) {
        switch (state) {
            case 0:
                enter_sleep_mode();
                break;
            case 1:
                system_display();
                break;
            case 2: 
                system_reset();
                system_init();
                break;
            default: 
                LCD_SetCursor(1,1);
                LCD_WriteString("Invalid state");
                break;
        }
    }
}
