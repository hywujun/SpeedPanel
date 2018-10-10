#include "stm32f1xx_hal.h"
#include "HMC5883L.h"
#include <math.h>
#include "font.h"



uint8_t HMC5883L_Init(I2C_HandleTypeDef *I2C, uint8_t MODE, uint8_t ODR, uint8_t GAIN)
{
	uint8_t I2C_err;
	uint8_t tem[2];
		
	tem[0]=HMC5883L_CRA;
	tem[1]=ODR;
	I2C_err=HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1);

	
	tem[0]=HMC5883L_CRB;
	tem[1]=GAIN<<5;
	I2C_err=HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1);

	tem[0]=HMC5883L_MOD;
	tem[1]=MODE;	
	I2C_err=HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1);

	return I2C_err;
}

uint8_t HMC5883L_Read(I2C_HandleTypeDef *I2C, short *x, short *y, short *z)
{
	uint8_t I2C_err;
	uint8_t buffer[6];
	uint8_t reg;

	reg=HMC5883L_SR;	
	//检查DATAREADY位
//	do {
//	I2C_err=HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, &reg, 1, 1);
//	I2C_err=HAL_I2C_Master_Receive(I2C, HMC5883L_ADD, &status, 1, 1);
//	if (I2C_err!=HAL_OK)
//		return I2C_err;
//	status&=0x01; //DATAREADY BITS
//	} while(status!=0x01);
	
	reg=HMC5883L_XM;
	I2C_err=HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, &reg, 1, 1);
	I2C_err=HAL_I2C_Master_Receive(I2C, HMC5883L_ADD, buffer, 6, 1);
	*x=((short) (((uint16_t) buffer[0])<<8)|buffer[1]);  
	*z=((short) (((uint16_t) buffer[2])<<8)|buffer[3]);
	*y=((short) (((uint16_t) buffer[4])<<8)|buffer[5]);

	return I2C_err;
	
}

/* 自检 2018-6-23
	返回各个通道的自检值
*/
void HMC5883L_Selftest(I2C_HandleTypeDef *I2C, short *result)
{
	uint8_t tem[2];
	short x, y, z;
	
	//增益为缺省值，
	tem[0]=HMC5883L_CRB;
	tem[1]=0x40;	
	HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1); //GAIN=2, +/-1.3Ga

	tem[0]=HMC5883L_CRA;
	tem[1]=HMC5883L_ODR15|0x01;		
	HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1); //8-average, 15 Hz default, positive self test measurement
	
	tem[0]=HMC5883L_MOD;
	tem[1]=HMC5883L_SIN;		
	HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1); //selftest, positive current

	HAL_Delay(10);
	//读出包括地磁场的自检值
	HMC5883L_Read(I2C, result, result+1, result+2);

	//读出地磁场值
	tem[0]=HMC5883L_MOD;
	tem[1]=HMC5883L_CON;		 //正常测量模式
	HAL_I2C_Master_Transmit(I2C, HMC5883L_ADD, tem, 2, 1); 
	HAL_Delay(10);
	//读出地磁场
	HMC5883L_Read(I2C, &x, &y, &z);
	
	//自检值-地磁场，得到自检值
	result[0]-=x;
	result[1]-=y;	
	result[2]-=z;	
}

/* 返回计算出的方向角
yx_coe=自检后Y轴校正系数，X/Y
dec=磁偏角
*/

uint16_t Direction(I2C_HandleTypeDef *I2C, float dec)
{
	short x, y, z;
	float rad, fx, fz, fy;
	uint16_t degree;
	uint16_t angle;
	HMC5883L_Read(I2C, &x, &y, &z);
	//x轴为前进方向
	
//消除外磁场影响
	x-=17;
	y+=171;
	z-=136;
	
//因为倒放，X，Z轴方向相反
	x=-x;
	z=-z;
	
	fx=(float) x;   
	fz=(float) z;
	fy=(float) y;

	rad=atanf(fabs(fy)/fabs(fx));
	degree=(uint16_t) (rad*360.0/6.28);  //转换为角度
	X=12;
	Y=72;
	printf("%3d, %3d", x, y);
	
	if ((x>0)&&(y>0))
	{
		if (degree<5)
			angle=360+degree+dec;
		else
			angle=degree+dec;
	}
	
	if ((x<=0)&&(y>0))
		angle=180-degree+dec;
	if ((x<=0)&&(y<=0))
		angle=180+degree+dec;
	if ((x>0)&&(y<=0))
		angle=360-degree+dec;
	
	if (angle>360)
		angle-=360;
	
	return angle;

}
