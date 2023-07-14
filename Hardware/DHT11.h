#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f10x.h"                  // Device header
#define DHT11 GPIO_Pin_12
#define DHT11_Port GPIOA
extern u8 humi,temp,dechumi,dectemp;

void DHT11_Init(void);
void DHT_Mode(u8 mode);
void DHT_Sends_Start(void);
void DHT_Sends_Response(void);
u8 DHT11_Read_Byte(void);
void DHT11_Read_data(u8 *humi,u8 *temp,u8 *dechumi,u8 *dectemp);
void DHT11_Show(u8 humi,u8 temp,u8 dechumi,u8 dectemp);
void DHT11_AppUpload(u8 humi,u8 temp,u8 dechumi,u8 dectemp);

#endif

