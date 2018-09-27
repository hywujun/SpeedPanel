#include "stm32f1xx_hal.h"
#include "RotaryEncoder.h"

//按键值
uint8_t BUTTON=NO_ACT;

//延时消抖
void Dithering(void)
{
	uint16_t tim=100;
	while(tim--);
	
}

void RE_call(uint16_t GPIO_Pin)
{
	static uint8_t status=0;	
	//消抖
	
	Dithering();	
	//如果是15脚中断
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)==GPIO_PIN_RESET)
	{
		BUTTON=PUSH_BUTTON;
		return;
	}


	//如果是11脚中断，上升延或下降延
	if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)==GPIO_PIN_RESET)&&(status==1)) //如果下降延中断
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)==GPIO_PIN_SET)
			BUTTON=TURN_CW;
		else
			BUTTON=TURN_CCW;
	}
	
	if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)==GPIO_PIN_SET)&&(status==0)) //如果上升延中断
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)==GPIO_PIN_SET)
			BUTTON=TURN_CCW;
		else
			BUTTON=TURN_CW;
	}
	//保存中断前11脚状态
	status=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
	
}

