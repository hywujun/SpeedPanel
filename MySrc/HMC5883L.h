#include "stm32f1xx_hal.h"

#define HMC5883L_CRA		0x00	//Configuration Register A
#define HMC5883L_CRB		0x01	//Configuration Register B
#define HMC5883L_MOD		0x02	//Mode Register
#define HMC5883L_XM			0x03	//Data Output X MSB Register
#define HMC5883L_XL			0x04	//Data Output X LSB Register
#define HMC5883L_ZM			0x05	//Data Output Z MSB Register
#define HMC5883L_ZL			0x06	//Data Output Z LSB Register
#define HMC5883L_YM			0x07	//Data Output Y MSB Register
#define HMC5883L_YL			0x08	//Data Output Y LSB Register
#define HMC5883L_SR			0x09	//Status Register
#define HMC5883L_IRA		0x0A	//Identification Register A
#define HMC5883L_IRB		0x0B	//Identification Register B
#define HMC5883L_IRC		0x0C	//Identification Register C

#define HMC5883L_ADD		0x3c	//ADDRESS OF HMC5883L FOR WRITE

#define HMC5883L_ODR15	0x70  //15HZ, Sample averaged=8
#define HMC5883L_ODR30	0x74  //30HZ, Sample averaged=8
#define HMC5883L_ODR75	0x78  //75HZ, Sample averaged=8

#define HMC5883L_CON		0x00	//continuous mode
#define HMC5883L_SIN		0x01	//自检，正电流
#define HMC5883L_SIP		0x02	//自检，负电流


// HMC5883L_Init(HMC5883L_CON, HMC5883L_ODR75, 1);
uint8_t HMC5883L_Init(I2C_HandleTypeDef *I2C, uint8_t MODE, uint8_t ODR, uint8_t GAIN);
uint8_t HMC5883L_Read(I2C_HandleTypeDef *I2C, short *x, short *y, short *z);
void HMC5883L_Selftest(I2C_HandleTypeDef *I2C, short *result);
void HMC5883L_Calibrate(I2C_HandleTypeDef *I2C, uint8_t GAIN);
uint16_t Direction(I2C_HandleTypeDef *I2C, float yx_coe, float dec);


