#include "stm32f1xx_hal.h"

#define UI_MAIN  0
#define UI_SET   1

void MainUI_dis(void);
void MainUI_loop(void);

void SetUI_dis(void);
void SetUI_loop(void);

void ST7529_Compass(uint8_t x1, uint8_t y1, uint8_t shift);
