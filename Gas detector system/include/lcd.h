#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void I2C1_GPIO_Init(void);
void I2C1_Init(void);
void LCD_Init(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteString(char *str);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Pulse(uint8_t data);
void LCD_Write4Bits(uint8_t data);
void LCD_Write(uint8_t data);
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_WriteString(char *str);

#endif
