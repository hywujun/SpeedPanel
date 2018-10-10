/*




*/
#include "stm32f1xx_hal.h"

#ifndef __page			//以防重复定义
#define __page
//页面类型定义 
typedef struct 
{
	uint8_t Page_id;		//页面号
	void (*PageInit)(void); 	//页面入口函数
	void (*PageLoop)(void);		//页面内循环
	void (*PageExit)(void);		//关闭页面
	
}	PageTypeDef;

typedef struct 
{
	uint8_t MItemId;		//
	uint8_t PosX;				//X坐标
	uint8_t PosY;
	char *s;					//要显示的字符串
	uint8_t language;		//显示中文还是英文
	void (*HandleButton) (void);
		
} MenuItemTypeDef;

void DrawMenu(MenuItemTypeDef *Menuitem);
void ItemGetFocus(uint8_t itemno);
void ItemLostFocus(uint8_t itemno);

#endif

