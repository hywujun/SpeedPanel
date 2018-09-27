#include "stm32f1xx_hal.h"
#include "ST7529.h"


//#define READ_HIGH 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
//#define READ_LOW	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
//#define WRITE_HIGH		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
//#define WRITE_LOW			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
//#define CMD						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
//#define DATA					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
//#define CS_HIGH 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
//#define CS_LOW		 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
//#define RES_HIGH 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
//#define RES_LOW 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)

//RD=PB7
#define READ_HIGH 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define READ_LOW	 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
//WR=PB6
#define WRITE_HIGH		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define WRITE_LOW			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
//A0=PB0
#define CMD						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define DATA					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
//CS=PB10
#define CS_HIGH 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define CS_LOW		 		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
//RES=PB11
#define RES_HIGH 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define RES_LOW 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)


uint8_t X=0, Y=0, FONT=ASCII9X12, REVERSE=0;

void Delay_us(uint16_t time)
{
	uint8_t i=5;
	while(time--)
		while(i--);
}

void Write_CMD(uint8_t cmd)
{
	uint16_t tem;
	
	CMD;					//write command	
	CS_LOW;				//Chip select

	READ_HIGH;		//set read pin high
	
	tem=GPIOA->ODR;	
	tem=tem&0xff00;
	tem=tem|cmd;	

	WRITE_LOW;		//write pin low
	GPIOA->ODR=tem;
	Delay_us(1);	
	WRITE_HIGH;		//wirte pin high		
	CS_HIGH;
	Delay_us(1);
}

void Write_Data(uint8_t data)
{
	uint16_t tem;
	DATA;	
	CS_LOW;
	READ_HIGH;	
	
	tem=GPIOA->ODR;
	tem=tem&0xff00;
	tem=tem|data;
	
	WRITE_LOW;	
	GPIOA->ODR=tem;
	Delay_us(1);
	WRITE_HIGH;	
	CS_HIGH;
	Delay_us(1);
}

uint8_t Read_Data(void)
{
	uint16_t tem;
	uint8_t data=0;
	DATA;	
	CS_LOW;
	WRITE_HIGH;	
	
	WRITE_LOW;	
	GPIOA->ODR=tem;
	//Delay_us(1);
	WRITE_HIGH;	
	CS_HIGH;
	//Delay_us(1);
	
	return data;
}

void ST7529_Init(void)
{
	CS_LOW;
	RES_LOW;
	HAL_Delay(100);
	RES_HIGH;
	
	Write_CMD(0x30);		//EXT=0
	Write_CMD(0x94);		//SLEEP OUT
	Write_CMD(0xD1);		//OSC ON
	Write_CMD(0x20);		//Power control set
	Write_Data(0x08);		//Booster on
	HAL_Delay(10);				//delay 1ms
	Write_CMD(0x20);		//
	Write_Data(0x0B);		//Booster on, regulater, follower on
	Write_CMD(0x81);		//对比度调节。与BIAS电压有关系。BIAS 1/11，对应3e0左右。BIAS 1/7，对应210左右。
	Write_Data(0xe0);		//VPR[5:0]
	Write_Data(0x03);		//VPR[8:6] 对比度3e0
	Write_CMD(0xCA);		//Display control
	Write_Data(0x04);		//不分频（X1)
	Write_Data(0x23);		//Duty =1/144
	Write_Data(0x00);		//设置几行为一帧，使用默认值11
	Write_CMD(0XA6);		//设置RAM中数据为正常显示
	Write_CMD(0XBB);		//控制COM反转指令
	Write_Data(0X01);		//设置COM扫描效度为0-79 159-80
	Write_CMD(0XBC);		//控制DATA扫描次序指令
	Write_Data(0x00);		//设置DATA扫描次序为0-254
	Write_Data(0x00);		//设置RGB的排列顺序
	Write_Data(0x01);		//设置32级灰度2B3P模式
	Write_CMD(0X75);		//控制行地址的指令
	Write_Data(0x10);		//设置起始行为16
	Write_Data(0x8F);		//设置终止行为144
	Write_CMD(0X15);		//控制列地址的指令
	Write_Data(0x00);		//控制起始列为0
	Write_Data(0x54);		//设置终止列为84

	Write_CMD(0X31);		//设置EXT=1
	Write_CMD(0X32);		//控制附加电路设置的指令
	Write_Data(0x03);		//设置时钟频率为12.7K，默认值
	Write_Data(0x01);		//设置倍压电容的频率为6KHZ, 默认值
	Write_Data(0x03);		//设置BIAS=1/11
	Write_CMD(0X34);		//SOFTWARE INITIAL
	Write_CMD(0X30);		//设置EXT=0
	Write_CMD(0XAF);		//打开显示	
	
	/*测试*/
	Write_CMD(0XA9);		//退出部分显示
	Write_CMD(0XEE);		//退出部分显示	
	
	CS_HIGH;
	HAL_Delay(100);
	ST7529_Fill(0,84,0,127,0xff);
	
}



void ST7529_Fill(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t pix)
{
	uint16_t i, tem[2];
	Write_CMD(0x30);		//EXT=0
	Write_CMD(0X15);		//控制列地址的指令
	Write_Data(x1);			//控制起始列
	Write_Data(x2);		//设置终止列
	Write_CMD(0X75);		//控制行地址的指令
	Write_Data(y1+16);		//设置起始
	Write_Data(y2+16);		//设置终止行
	Write_CMD(0x5c);		//控制往显存写数据
	
	tem[0]=pix<<3|(pix>>2&0x07);
	tem[1]=pix<<6|(pix&0x1f);
	
	for (i=0;i<((uint16_t)(x2-x1+1))*(y2-y1+1);i++)
		{
			Write_Data(tem[0]);	
			Write_Data(tem[1]);
		}
}


void ST7529_Putchar(uint8_t ch)
{
	uint8_t i, j, tem, x, y;
	x=X/3;
	y=Y+16;
	ST7529_DrawPix(0xe0); //复位
	if (FONT==ASCII9X12)
	{
		Write_CMD(0x30);		//EXT=0
		Write_CMD(0X15);		//控制列地址的指令
		Write_Data(x);			//控制起始列
		Write_Data(x+2);		//设置终止列
		Write_CMD(0X75);		//控制行地址的指令
		Write_Data(y);		//设置起始
		Write_Data(y+11);		//设置终止行
		Write_CMD(0x5c);		//控制往显存写数据
		for (i=0;i<12;i++)
		{
			tem=ASCII_9X12[(ch-0x20)*12+i];
			for (j=0;j<8;j++)
			{
				if (REVERSE==1)    //如果反相显示 
				{
					if((tem&0x01)==0x01)
						ST7529_DrawPix(0xff);
					else
						ST7529_DrawPix(0x00);
				}
				else
				{
					if((tem&0x01)==0x01)
						ST7529_DrawPix(0x00);
					else
						ST7529_DrawPix(0xff);
				}					
				tem>>=1;			
			}
			ST7529_DrawPix(0xff);
		}
		if (ch=='.')
			X+=6;		
		else
			X+=9;
	}
	
	if (FONT==DIG40X60)
	{
		ST7529_Dispic(DIG_40X60+(ch-0x30)*300, X, Y, 40, 60);
		X+=42;
	}		
	
}




/*显示图片
2B3P,2个字节写3个点
x1 X (pixel, 0-254)
y1 Y (0-128)
Wid 图片宽度 (实际像素)
Hei 图片高度
*/

void ST7529_Dispic(const char *pic, uint8_t x1, uint8_t y1, uint16_t wid, uint8_t hei)
{
	uint8_t col_s;  //图片起始列
	uint8_t col_num;  //图片所占列数
	uint8_t cols_rem;  //图片起始列余数（从起始列第几个像素显示）
	uint8_t cole_rem;  //图片结尾列余数（结尾列差几个像素）
	uint8_t i, j, k;
	uint8_t tem, w_rem;
	uint16_t p;

	ST7529_DrawPix(0xe0); //复位
	
	w_rem=wid%8;
	
	//算出实际列数(每列三个像素）
	col_s=x1/3;		
	cols_rem=x1%3;
	cole_rem=(wid+cols_rem)%3;				//图片行尾像素点在列中位置
	if (cole_rem!=0)									//如果刚好在列中最后一点（无余数）
		{
			col_num=(wid+cols_rem)/3+1;
			cole_rem=3-cole_rem;					//差几个点到最后一列尾
		}
	else 
		col_num=(wid+cols_rem)/3;
		
	
	Write_CMD(0x30);		//EXT=0
	Write_CMD(0X15);		//控制列地址的指令
	Write_Data(col_s);			//控制起始列
	Write_Data(col_s+col_num-1);		//设置终止列
	Write_CMD(0X75);		//控制行地址的指令
	Write_Data(y1+16);		//设置起始
	Write_Data(y1+15+hei);		//设置终止行
	Write_CMD(0x5c);		//控制往显存写数据

	p=0;
	for (i=0;i<hei;i++)
	{
		//如果起始点不是正好在一列的起点
		for (j=0;j<cols_rem;j++)
			ST7529_DrawPix(0xff);
		
		for (j=0;j<wid/8;j++)
		{
			tem=pic[p];
			p++;
			for (k=0;k<8;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}
		}	
		//如果每行点数/8不为零;
		if (w_rem>0)
		{
			tem=pic[p];
			p++;
			for (k=0;k<w_rem;k++)
			{
				if((tem&0x01)==0x01)
					ST7529_DrawPix(0x00);
				else
					ST7529_DrawPix(0xff);
				tem>>=1;	
			}			
		}
		//如果有余数，写入0;
		if (cole_rem>0)
		{
			for (j=0;j<cole_rem;j++)
				ST7529_DrawPix(0xff);
		}	
	}
}

/*
	2B3P写，每三个点写入ST7529一次。data为要写的点。
	如果DATA为0xe0, 则P=0
*/
void ST7529_DrawPix(uint8_t data)
{
	static uint8_t tem[3]={0,0,0};
	static uint8_t p=0;
	uint8_t w[2];
	
	if (data==0xe0)
	{
		p=0;
		return;
	}
	
	tem[p]=data;
	p++;		
	if (p>2)
	{
		w[0]=tem[0]<<3|((tem[1]>>2)&0x07);
		w[1]=tem[1]<<6|(tem[2]&0x1f);
		Write_Data(w[0]);
		Write_Data(w[1]);
		p=0;
	}
	
}
