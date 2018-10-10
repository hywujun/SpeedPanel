#include "stm32f1xx_hal.h"
#include "ST7529.h"
#include <stdio.h>
#include "HMC5883L.h"
#include "UI.h"
#include "18B20.h"
#include "page.h"
#include "RotaryEncoder.h"

//指南针
#define CAMX 42
#define CAMY 4

//时间显示位置
#define TIME_X	0
#define TIME_Y	0

//温度显示位置
#define TEM_X		174
#define TEM_Y		0

//速度显示位置
#define SPEED_X 88
#define SPEED_Y 32

//电压电流
#define VOL_X  188
#define VOL_Y  60

/* 主显示页面*********************************************/
void MainUI_dis(void);
void MainUI_loop(void);
void MainUI_exit(void);

extern PageTypeDef SettingUi;

PageTypeDef PageMainUi=
{
	1,
	MainUI_dis,
	MainUI_loop,
	MainUI_exit	
};



extern uint8_t UI;		//当前显示界面
extern uint8_t COMP_state;		//磁力计状态
extern I2C_HandleTypeDef hi2c1;
extern RTC_HandleTypeDef hrtc;
extern uint8_t BUTTON;	//当前按下的按键
extern PageTypeDef *CurrentPage;


RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void Display_speed(void);  //速度值在Speed.c文件中计算
float Voltage=0.0;
float Current=0.0;

uint8_t B20_err;


__IO uint8_t SEC_INT=0;    //秒中断，sys_tick中断1000次，秒中断置1
__IO uint8_t SPEED_REF_INT=0; // 速度更新中断

DS18B20 B20;

//显示主界面
void MainUI_dis(void)
{
	//当前界面为主界面
	UI=UI_MAIN;
	//清屏
	ST7529_Fill(0,84,0,127,0xff);
	//显示箭头
	ST7529_Dispic(arrow, 124, 0, 7, 4);
	//读系统时间并显示
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	//HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	FONT=ASCII9X12;
//	X=0;
//	Y=0;	
//	printf("%02d/%02d", sDate.Month, sDate.Date);		
	//5.18-不显示日期
	
	Y=TIME_Y;
	X=TIME_X;
	printf("%2d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	X=189;
	Y=0;
	
	//初始化18B20
	B20_err=DS18B20_Init(&B20, GPIOB, GPIO_PIN_12);
	
	//显示摄氏度
	ST7529_Dispic(Degree, TEM_X+63, TEM_Y, 12, 12);

	//显示电压电流
//	ST7529_Dispic(DIAN, VOL_X, VOL_Y, 12, 12);
//	ST7529_Dispic(YA, VOL_X+12, VOL_Y, 12, 12);
//	ST7529_Dispic(DIAN, VOL_X, VOL_Y+12, 12, 12);
//	ST7529_Dispic(LIU, VOL_X+12, VOL_Y+12, 12, 12);	
	
}

//主界面循环函数
void MainUI_loop(void)
{
	uint16_t direction;	
	short tem;
	FONT=ASCII9X12;
	//如果到了一秒
	if (SEC_INT==1)
	{
/**更新时间******************************************/ 
		sTime.Seconds++;
		if (sTime.Seconds>=60)
		{
			sTime.Minutes++;
			if (sTime.Minutes>=60)
			{
				sTime.Hours++;
				if (sTime.Hours>=24)
					sTime.Hours=0;
				X=TIME_X;
				Y=TIME_Y;
				printf("%02d", sTime.Hours);
				sTime.Minutes=0;
			}
			X=27+TIME_X;
			Y=TIME_Y;
			printf("%02d", sTime.Minutes);
			sTime.Seconds=0;
		}
			X=54+TIME_X;
			Y=TIME_Y;
			printf("%02d", sTime.Seconds);
		
		SEC_INT=0;
/**更新温度**************************************/
		X=TEM_X+30;
		Y=TEM_Y;
		if (B20_err==1)
		{
			tem=DS18B20_Get_Temp(&B20);
			printf("%.1f", ((float) tem)/10.0);	
		}
		else
		{
			printf("ERR");
			B20_err=DS18B20_Init(&B20, GPIOB, GPIO_PIN_12);
		}
	}
	
//如果0.5s时间到
	if (SPEED_REF_INT==1)
	{
		//更新指南针
		if (COMP_state==HAL_OK)
		{
			direction=Direction(&hi2c1, 5);
			ST7529_Fill(CAMX-18, CAMX+18, CAMY, CAMY+19, 0XFF);
			ST7529_Compass(CAMX-16, CAMY, direction*53/100);
		}
		else
		{
			FONT=ASCII9X12;
			X=CAMX-18;
			Y=CAMY;
			printf("ERR=%d", COMP_state);
		}

		SPEED_REF_INT=0;
	}
	//更新速度
	FONT=DIG40X60;
	X=SPEED_X;
	Y=SPEED_Y;
	Display_speed();

	
	/*更新电压电流

	FONT=ASCII9X12;
	X=VOL_X+12;
	Y=VOL_Y;
	printf("%04.1fV", Voltage);
	X=VOL_X+12;
	Y=VOL_Y+14;
	printf("%04.1fA", Current);
	X=VOL_X+12;
	Y=VOL_Y+28;
	printf("%04.0fW", Current*Voltage);
	*/
	
	
	switch(BUTTON)
	{
		case TURN_CCW:
			BUTTON=NO_ACT;
		case TURN_CW:
			BUTTON=NO_ACT;		
		break;		
		case PUSH_BUTTON:
			SettingUi.PageInit();
			CurrentPage=&SettingUi;
			BUTTON=NO_ACT;
		break;
	}


}



/*偏移显示图片
x1 X (COLUMNM)
y1 Y
shift 偏移点数
0~48*4=192
0为正北
*/
/*此版本5.18日，显示在屏幕上方，箭头、标尺向下
void ST7529_Compass(uint8_t x1, uint8_t y1, uint8_t shift)
{
	uint8_t i, j, k, line, shift_t;
	uint8_t rem, tem;

	ST7529_DrawPix(0xe0); //复位
	shift_t=shift;
	//将0-196的偏移简化为0-48
	i=shift/48;
	shift=shift-48*i;
		
	Write_CMD(0x30);		//EXT=0
	Write_CMD(0X15);		//控制列地址的指令
	Write_Data(x1);			//控制起始列
	Write_Data(x1+31);		//设置终止列
	Write_CMD(0X75);		//控制行地址的指令
	Write_Data(y1+16);		//设置起始
	Write_Data(y1+7);		//设置终止行
	Write_CMD(0x5c);		//控制往显存写数据	

	j=shift/8;					//第一个点在的字节
	rem=shift%8;

  for (line=0;line<8;line++)
	{
		tem=ruller[line*6+j];
		//填第一个字节里的点
		tem>>=rem;
		for (i=0;i<8-rem;i++)
		{			
			if((tem&0x01)==0x01)
				ST7529_DrawPix(0x00);
			else
				ST7529_DrawPix(0xff);
			tem>>=1;	
		}
		//填一行剩下字节
		for (i=0;i<5-j;i++)
		{
			tem=ruller[line*6+j+i+1];
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}			
		}
		//填完整一行
		for (i=0;i<6;i++)
		{
			tem=ruller[line*6+i];
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}			
		}
		//填shift后的字节
		for (i=0;i<j;i++)
		{
			tem=ruller[line*6+i];
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}	
		}
		
		tem=ruller[line*6+j];
		for (i=0;i<rem;i++)
		{
			if((tem&0x01)==0x01)
				ST7529_DrawPix(0x00);
			else
				ST7529_DrawPix(0xff);
			tem>>=1;	
		}		
		
	}
	
	tem=x1*3;
	//显示北
	if ((shift_t<48)||(shift_t>144))
	{
		if (shift_t<48)
			ST7529_Dispic(HZ_12X12+72, 122-shift_t, y1+10, 12, 12);
		else
			ST7529_Dispic(HZ_12X12+72, 170-(shift_t-144), y1+10, 12, 12);
	}
	
	//显示东
	if ((shift_t<96))
	{
			ST7529_Dispic(HZ_12X12, 96-shift_t+74, y1+10, 12, 12);
	}
	
	//显示南
	if ((shift_t>48)&&(shift_t<144))
	{
			ST7529_Dispic(HZ_12X12+24, 170-(shift_t-48), y1+10, 12, 12);
	}

	//显示西
	if (shift_t>96)
	{
			ST7529_Dispic(HZ_12X12+48, 170-(shift_t-96), y1+10, 12, 12);
	}
}
*/

//此版本5.19日，箭头8*3，标尺48*6，字母显示东南西北，大小6*8
void ST7529_Compass(uint8_t x1, uint8_t y1, uint8_t shift)
{
	uint8_t i, j, k, line, shift_t;
	uint8_t rem, tem;

	ST7529_DrawPix(0xe0); //复位
	shift_t=shift;
	//将0-196的偏移简化为0-48
	i=shift/48;
	shift=shift-48*i;
		
	Write_CMD(0x30);		//EXT=0
	Write_CMD(0X15);		//控制列地址的指令
	Write_Data(x1);			//控制起始列
	Write_Data(x1+31);		//设置终止列
	Write_CMD(0X75);		//控制行地址的指令
	Write_Data(y1+16);		//设置起始
	Write_Data(y1+5);		//设置终止行，5.18 6行，加5
	Write_CMD(0x5c);		//控制往显存写数据	

	j=shift/8;					//第一个点在的字节
	rem=shift%8;

	//line图像行
  for (line=0;line<6;line++)
	{
		tem=ruller[line*6+j];	//6为48/8=6,每行为6BIT
		//填第一个字节里的点
		tem>>=rem;
		for (i=0;i<8-rem;i++)
		{			
			if((tem&0x01)==0x01)
				ST7529_DrawPix(0x00);
			else
				ST7529_DrawPix(0xff);
			tem>>=1;	
		}
		//填一行剩下字节
		for (i=0;i<5-j;i++)
		{
			tem=ruller[line*6+j+i+1];
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}			
		}
		//填完整一行
		for (i=0;i<6;i++)
		{
			tem=ruller[line*6+i];
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}			
		}
		//填shift后的字节
		for (i=0;i<j;i++)
		{
			tem=ruller[line*6+i];
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}	
		}
		
		tem=ruller[line*6+j];
		for (i=0;i<rem;i++)
		{
			if((tem&0x01)==0x01)
				ST7529_DrawPix(0x00);
			else
				ST7529_DrawPix(0xff);
			tem>>=1;	
		}		
		
	}
	
	tem=x1*3;
	//显示北，5.18改为显示字母
	if ((shift_t<48)||(shift_t>144))
	{
		if (shift_t<48)
			ST7529_Dispic(HZ_12X12+72, 120-shift_t, y1+3, 12, 12);
		else
			ST7529_Dispic(HZ_12X12+72, 168-(shift_t-144), y1+3, 12, 12);
	}
	
	//显示东
	if ((shift_t<96))
	{
			ST7529_Dispic(HZ_12X12, 94-shift_t+74, y1+3, 12, 12);
	}
	
	//显示南
	if ((shift_t>48)&&(shift_t<144))
	{
			ST7529_Dispic(HZ_12X12+24, 168-(shift_t-48), y1+3, 12, 12);
	}

	//显示西
	if (shift_t>96)
	{
			ST7529_Dispic(HZ_12X12+48, 168-(shift_t-96), y1+3, 12, 12);
	}
}

void MainUI_exit()
{
	
}
