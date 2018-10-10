#include "stm32f1xx_hal.h"
#include "page.h"
#include "ST7529.h"
#include <stdio.h>
#include "RotaryEncoder.h"
#include "HMC5883L.h"

#define ITEMNO 3		//此页面菜单项数量

/**此页面函数*******************************************/
void SettingUI_dis(void);
void SettingUI_loop(void);
void SettingUI_exit(void);

/**菜单项函数*******************************************/
char TIMESETTING[]="DATA TIME SETTING";
char EXIT[]="EXIT";
char CORR[]="MAG CORRECTION";


void MenuItemHandle(void);			//处理按键事件
void ItemSwitch(uint8_t itemno, uint8_t button);
void MC_handle(void);   //磁力计校正菜单按键事件响应
void Mag_correctoin(void);
void MenuExit(void);

extern uint8_t BUTTON;	//当前按下的按键
extern I2C_HandleTypeDef hi2c1;
extern PageTypeDef *CurrentPage;
extern PageTypeDef PageMainUi;
extern RTC_HandleTypeDef hrtc;

extern RTC_TimeTypeDef sTime;

//当前菜单
uint8_t CurMenuID=0;

MenuItemTypeDef TimeSet=
{
	1,
	48,
	30,
	TIMESETTING,
	0,
	MenuItemHandle		
};

MenuItemTypeDef Mag_corect=
{
	2,
	48,
	44,
	CORR,
	0,
	MC_handle		
};

MenuItemTypeDef Exit=
{
	3,
	48,
	58,
	EXIT,
	0,
	MenuExit		
};

MenuItemTypeDef *Items[]={&TimeSet, &Mag_corect, &Exit};

/*页面处理函数*************************/
PageTypeDef SettingUi=
{
	2,
	SettingUI_dis,
	SettingUI_loop,
	SettingUI_exit	
};

void SettingUI_dis(void)
{
	uint8_t i;
	//清屏
	
	ST7529_Fill(0,84,0,127,0xff);
	CurMenuID=0;
	for (i=0;i<ITEMNO;i++)
	{
		DrawMenu(Items[i]);		
	}
	
	ItemGetFocus(CurMenuID);
		
}

void SettingUI_loop(void)
{
	Items[CurMenuID]->HandleButton();	
	
}

void SettingUI_exit(void)
{
	
}

/*菜单处理函数*************************/
void ItemGetFocus(uint8_t itemno)
{
	//显示箭头
	ST7529_Dispic(Arrow6x7, Items[itemno]->PosX-9, Items[itemno]->PosY+2, 6, 7);
}

void ItemLostFocus(uint8_t itemno)
{
	//显示空白
  ST7529_FillArea(Items[itemno]->PosX-9, Items[itemno]->PosY+2, 6, 7, 0xff);
}

//菜单项切换 
void ItemSwitch(uint8_t itemno, uint8_t button)
{
	ItemLostFocus(CurMenuID);
	if (button==TURN_CW)
	{
		if (CurMenuID==(ITEMNO-1))
			CurMenuID=0;		
		else
			CurMenuID++;

	}
	if (button==TURN_CCW)
	{
		if (CurMenuID==0)
			CurMenuID=ITEMNO-1;
		else
			CurMenuID--;
	}	
	BUTTON=NO_ACT;
	ItemGetFocus(CurMenuID);
}

void MenuItemHandle(void)
{	
	switch(BUTTON)
	{
		case TURN_CCW:
		case TURN_CW:
			ItemSwitch(CurMenuID, BUTTON);		
		break;		
		case PUSH_BUTTON:
			
		break;
	}
	BUTTON=NO_ACT;
}

void MC_handle(void)
{	
	switch(BUTTON)
	{
		case TURN_CCW:
		case TURN_CW:
			ItemSwitch(CurMenuID, BUTTON);		
		break;		
		case PUSH_BUTTON:
			
			Mag_correctoin();
		break;
	}
	BUTTON=NO_ACT;
}

//磁力计校正函数
void Mag_correctoin(void)
{
	static short x_max=0, y_max=0, z_max=0, x_min=0, y_min=0, z_min=0;
	short x, y, z;
	BUTTON=NO_ACT;
	ItemLostFocus(CurMenuID);
	while (BUTTON!=PUSH_BUTTON)
	{
		HMC5883L_Read(&hi2c1, &x, &y, &z);
		if (x>x_max)
			x_max=x;
		if (x<x_min)
			x_min=x;
		if (y>y_max)
			y_max=y;
		if (y<y_min)
			y_min=y;
		if (z>z_max)
			z_max=z;
		if (z<z_min)
			z_min=z;
		X=50;
		Y=80;
		printf("%4d %4d %4d", x_max, y_max, z_max);
		X=50;
		Y=94;	
		printf("%4d %4d %4d", x_min, y_min, z_min);		
	}
	BUTTON=NO_ACT;
	ItemGetFocus(CurMenuID);
}

//退出菜单
void MenuExit(void)
{
	switch(BUTTON)
	{
		case TURN_CCW:
		case TURN_CW:
			ItemSwitch(CurMenuID, BUTTON);	
			BUTTON=NO_ACT;		
		break;		
		case PUSH_BUTTON:
			PageMainUi.PageInit();
			CurrentPage=&PageMainUi;
			BUTTON=NO_ACT;
		break;
	}	
}

//时间设置
void Time_setting(void)
{
	uint8_t cus_pos=0;			//光标位置
	uint8_t is_pushed=0;		//是否按下
	
	X=48;
	Y=70;
	printf("Hour: %d", sTime.Hours);
	X=48;
	Y=84;
	printf("MINU: %d", sTime.Minutes);
	X=48;
	Y=96;
	printf("SAVE&EXIT");
	
	while (1)
	{
		
		switch(BUTTON)
		{
			case TURN_CCW:
			case TURN_CW:

			break;		
			case PUSH_BUTTON:

				BUTTON=NO_ACT;
			break;
		}	
	}
	
	
}

