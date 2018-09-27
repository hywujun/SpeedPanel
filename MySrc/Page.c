#include "stm32f1xx_hal.h"
#include "page.h"
#include "stdio.h"

extern uint8_t X, Y, FONT;


void DrawMenu(MenuItemTypeDef *Menuitem, uint8_t ItemNo)
{
	while (ItemNo)
	{
		X=Menuitem->PosX;
		Y=Menuitem->PosY;
		printf("%s", Menuitem->s);
		ItemNo--;
	}
}
