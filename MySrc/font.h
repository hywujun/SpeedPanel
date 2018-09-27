#include "stm32f1xx_hal.h"

#define ASCII9X12 0
#define DIG40X60	1

#define DONG		HZ_12X12					//东
#define NAN			HZ_12X12+24*1		//南
#define XI			HZ_12X12+24*2		//西
#define BEI			HZ_12X12+24*3		//北
#define DIAN		HZ_12X12+24*4		//电
#define YA			HZ_12X12+24*5		//压
#define LIU			HZ_12X12+24*6		//流
#define LIANG		HZ_12X12+24*7		//量
#define WEN			HZ_12X12+24*8		//温
#define DU 			HZ_12X12+24*9		//度
#define ZONG		HZ_12X12+24*10		//总
#define LI			HZ_12X12+24*11		//里
#define CHEN		HZ_12X12+24*12		//程
#define XU			HZ_12X12+24*13		//续
#define HANG		HZ_12X12+24*14		//航


extern uint8_t X, Y, FONT;
extern const char arrow[];
extern const char ruller[];
extern const char ASCII_9X12[];
extern const char HZ_12X12[];
extern const char DIG_40X60[];
extern const char Degree[];
extern const char ESWN[];

