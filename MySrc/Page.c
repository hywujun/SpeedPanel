#include "stm32f1xx_hal.h"
#include "page.h"
#include "stdio.h"

extern uint8_t X, Y, FONT;


void DrawMenu(MenuItemTypeDef *Menuitem)
{
		X=Menuitem->PosX;
		Y=Menuitem->PosY;
		printf("%s", Menuitem->s);
}
