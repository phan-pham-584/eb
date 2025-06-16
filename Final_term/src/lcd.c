#include "stm32f4xx.h"
#include "lcd.h"
#include "timer.h"

#define LCD_ADDR     0x27
#define BACKLIGHT    0x00
#define ENABLE       0x04
#define RS           0x01
#define RW           0x02

// I2C write, pulse, and LCD functions here (copy from your original code)
// ...

/* GPIO Configuration for I2C1 */
void I2C1_GPIO_Init(void) {
    GPIOB->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)));
    GPIOB->MODER |= (2 << (6 * 2)) | (2 << (7 * 2)); // Alternate function
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);           // Open-drain
    GPIOB->AFR[0] |= (4 << (6 * 4)) | (4 << (7 * 4));  // AF4 for I2C
}

/* I2C1 Configuration */
void I2C1_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16;     // APB1 = 16MHz
    I2C1->CCR = 80;     // 100kHz
    I2C1->TRISE = 17;   // 1000ns / (1/16MHz) + 1
    I2C1->CR1 |= I2C_CR1_PE;
}

/* Send one byte over I2C */
void I2C1_Write(uint8_t addr, uint8_t data) {
    while (I2C1->SR2 & I2C_SR2_BUSY);
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
    (void)I2C1->SR1;
    I2C1->DR = addr << 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR1; (void)I2C1->SR2;
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

/* Pulse Enable */
void LCD_Pulse(uint8_t data) {
    I2C1_Write(LCD_ADDR, data | ENABLE | BACKLIGHT);
    Delay_Timer2(1);
    I2C1_Write(LCD_ADDR, (data & ~ENABLE) | BACKLIGHT);
    Delay_Timer2(1);
}

/* Send 4-bit data to LCD */
void LCD_Write4Bits(uint8_t data) {
    I2C1_Write(LCD_ADDR, data | BACKLIGHT);
    LCD_Pulse(data);
}

/* Send command to LCD */
void LCD_SendCommand(uint8_t cmd) {
    LCD_Write4Bits((cmd & 0xF0));
    LCD_Write4Bits(((cmd << 4) & 0xF0));
}

/* Send data (character) to LCD */
void LCD_SendData(uint8_t data) {
    LCD_Write4Bits((data & 0xF0) | RS);
    LCD_Write4Bits(((data << 4) & 0xF0) | RS);
}

/* Initialize LCD */
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

/* Set cursor position */
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t offsets[] = {0x00, 0x40, 0x14, 0x54};
    LCD_SendCommand(0x80 | (offsets[row] + col));
}

/* Write string to LCD */
void LCD_WriteString(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}


