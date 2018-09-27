#include "stm32f1xx_hal.h"

uint32_t tyre_len=128;	//轮胎周长，CM
uint8_t Speed=0;
uint32_t Total_rounds=0;



void Speed_Cal(void)
{
	static uint32_t Last_time=0;
	uint32_t current_time;
	uint8_t spd;
	
	current_time=HAL_GetTick();
	//每调用一次，总里程+1
	Total_rounds++;
	//计算速度
	spd=(uint8_t)	(tyre_len*36/(current_time-Last_time));
	if (spd<(Speed+15))
		Speed=spd;
	//记录转一圈时的系统时间
	Last_time=current_time;
	
}

void Display_speed(void)
{
	static uint8_t p=0;
	static uint8_t last_speed=0;
	//如果速度值没有更新
	if (Speed>0)
	{
		printf("%02d", Speed);
		last_speed=Speed;
		Speed=0;
		p=0;		
	}
	else
	{
		switch (p)
		{
			case 0:
				if (last_speed>9)
				{
					printf("09");
					last_speed=9;
				}				
				else
					printf("%02d", last_speed);
				p++;
			break;
			case 1:
				if (last_speed>4)
				{
					printf("04");
					last_speed=4;
				}				
				else
					printf("%02d", last_speed);
				p++;
			break;
			case 2:
				if (last_speed>2)
				{
					printf("02");
					last_speed=2;
				}				
				else
					printf("%02d", last_speed);
				p++;
			break;		
			default:
				printf("00");
		}
		
	}
			
}
