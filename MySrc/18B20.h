#include "stm32f1xx_hal.h"
#include "OWire_cube.h"

#define DS18B20 OWire

uint8_t DS18B20_Init(DS18B20 *B20, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
short DS18B20_Get_Temp(DS18B20 *B20);
void DS18B20_Start(DS18B20 *B20); // ds1820 start convert

