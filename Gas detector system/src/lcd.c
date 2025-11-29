#include "stm32f4xx.h"
#include "lcd.h"
#include "timer.h"

#define LCD_ADDR     0x27
#define BACKLIGHT    0x08
#define ENABLE       0x04
#define RS           0x01
#define RW           0x02


void I2C1_GPIO_Init(void) {
    GPIOB->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)));
    GPIOB->MODER |= (2 << (6 * 2)) | (2 << (7 * 2)); 
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);           
    GPIOB->AFR[0] |= (4 << (6 * 4)) | (4 << (7 * 4));  
}

void I2C1_Init(void) {
    RCC->APB1ENR |= (1 << 21);     

    I2C1->CR1 &= ~(1 << 0);      

    I2C1->CR2 = 16;                

    I2C1->CCR = 80;                 

    I2C1->TRISE = 17;               

    I2C1->CR1 |= (1 << 0);         
}

void I2C1_Write(uint8_t addr, uint8_t data) {
    while (I2C1->SR2 & (1 << 1));          

    I2C1->CR1 |= (1 << 8);                 

    while (!(I2C1->SR1 & (1 << 0)));      

    (void)I2C1->SR1;                     
    I2C1->DR = addr << 1;                 

    while (!(I2C1->SR1 & (1 << 1)));       

    (void)I2C1->SR1; (void)I2C1->SR2;      

    while (!(I2C1->SR1 & (1 << 7)));      

    I2C1->DR = data;                      

    while (!(I2C1->SR1 & (1 << 2)));       

    I2C1->CR1 |= (1 << 9);                 
}


void LCD_Pulse(uint8_t data) {
    I2C1_Write(LCD_ADDR, data | ENABLE | BACKLIGHT);
    Delay_Timer2(1);
    I2C1_Write(LCD_ADDR, (data & ~ENABLE) | BACKLIGHT);
    Delay_Timer2(1);
}

void LCD_Write4Bits(uint8_t data) {
    I2C1_Write(LCD_ADDR, data | BACKLIGHT);
    LCD_Pulse(data);
}

void LCD_SendCommand(uint8_t cmd) {
    LCD_Write4Bits((cmd & 0xF0));
    LCD_Write4Bits(((cmd << 4) & 0xF0));
}

void LCD_SendData(uint8_t data) {
    LCD_Write4Bits((data & 0xF0) | RS);
    LCD_Write4Bits(((data << 4) & 0xF0) | RS);
}

void LCD_Init(void) {
    Delay_Timer2(50);
    LCD_Write4Bits(0x30);
    Delay_Timer2(5);
    LCD_Write4Bits(0x30);
    Delay_Timer2(1);
    LCD_Write4Bits(0x30);
    Delay_Timer2(1);
    LCD_Write4Bits(0x20); // 4-bit mode

    LCD_SendCommand(0x28); // 4-bit, 2 lines, 5x8 dots
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    LCD_SendCommand(0x01); // Clear Display
    Delay_Timer2(2);
    LCD_SendCommand(0x06); // Entry Mode
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};
    LCD_SendCommand(0x80 | (offsets[row] + col));
}

void LCD_WriteString(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}


