#include "stm32f1xx_hal.h"
#include "page.h"
#include "ST7529.h"
#include <stdio.h>
#include "RotaryEncoder.h"

/*********************************************/
void SettingUI_dis(void);
void SettingUI_loop(void);
void SettingUI_exit(void);

/*********************************************/
char TIMESETTING[]="DATA TIME SETTING";
void ItemGetFocus(void);
void ItemLostFocus(void);
void TimeSetButHandle(void);

extern uint8_t BUTTON;

short count=5;

uint8_t CurMenuID=0;

MenuItemTypeDef TimeSet=
{
	1,
	50,
	30,
	TIMESETTING,
	0,
	ItemGetFocus,
	ItemLostFocus,
	TimeSetButHandle		
};

PageTypeDef SettingUi=
{
	2,
	SettingUI_dis,
	SettingUI_loop,
	SettingUI_exit	
};

void SettingUI_dis(void)
{
	//«Â∆¡
	ST7529_Fill(0,84,0,127,0xff);
	X=20;
	Y=20;
	printf("%d", count);
	


}

void SettingUI_loop(void)
{
	switch (BUTTON)
	{
		case PUSH_BUTTON:
			count=0;
			BUTTON=NO_ACT;
		break;
		
		case TURN_CW:
			count++;
			BUTTON=NO_ACT;
		break;
		
		case TURN_CCW:
			count--;
			BUTTON=NO_ACT;
		break;
		
		default:
			break;
	}
	X=20;
	Y=20;
	printf("%d   ", count);	
		
	
}

void SettingUI_exit(void)
{
	
}

