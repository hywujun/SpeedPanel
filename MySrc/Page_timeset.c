#include "stm32f1xx_hal.h"
#include "page.h"
#include "ST7529.h"
#include <stdio.h>
#include "RotaryEncoder.h"

extern uint8_t BUTTON;	//当前按下的按键

void timeset_init(void);
void timeset_loop(void);
void timeset_exit(void);


