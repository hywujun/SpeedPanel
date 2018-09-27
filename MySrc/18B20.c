#include "18B20.h"


/*初始化18B20，指定端口*/
uint8_t DS18B20_Init(DS18B20 *B20, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t r;
	OWInit(B20, GPIOx, GPIO_Pin);
	r=OWReset(B20);
	return r;	
}

void DS18B20_Start(DS18B20 *B20)// ds1820 start convert
{   						               
    OWReset(B20);	   
	 
    OWWrite(B20, 0xcc);// skip rom
    OWWrite(B20, 0x44);// convert
} 

short DS18B20_Get_Temp(DS18B20 *B20)
{
    uint8_t temp;
    uint8_t TL,TH;
	short tem;
    DS18B20_Start (B20);                    // ds1820 start convert
    OWReset(B20);
	 
    OWWrite(B20, 0xcc);// skip rom
    OWWrite(B20, 0xbe);// convert	    
    TL= OWRead(B20); // LSB   
    TH=OWRead(B20); // MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(float)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;    
} 

