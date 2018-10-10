#include "stm32f1xx_hal.h"
#include "font.h"

void ST7529_DrawPix(uint8_t data);
void ST7529_Init(void);
void Delay_us(uint16_t time);
void Write_CMD(uint8_t cmd);
void Write_Data(uint8_t data);

void ST7529_Fill(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t pix);
void ST7529_Putchar(uint8_t ch);
void ST7529_Putpic(const char *pic, uint8_t x1, uint8_t y1, uint16_t W, uint8_t H); 
void ST7529_Dispic(const char *pic, uint8_t x1, uint8_t y1, uint16_t wid, uint8_t hei);
void ST7529_FillArea(uint8_t x1, uint8_t y1, uint16_t wid, uint8_t hei, uint8_t pix);

extern uint8_t X, Y, FONT, REVERSE;


