#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Upload.h"
#include "Serial.h"
#include "Delay.h"
#include "Speed.h"
#include "OLED.h"
#include "ESP8266.h"
#include "Threshold.h"
#include "KEY.h"
#include "Data.h"
#include "HCSR504.h"
#include "string.h"
#include "BEEP.h"

extern char Serial_RxPacket[500];

void Mortor_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
}


//100正转停止
void Mortor_ON(void)
{
	BEEP_OFF();
	HCSR504_DeInit();
	Serial_DeInit();
	TIM1IT_DeInit();
	TIM2IT_DeInit();
	TIM4IT_DeInit();
	KeyIT_DeInit();
	OLED_Clear();
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	//起步
	for (int i = 90; i >= 80; i = i - 5)
	{
		OLED_ShowString(0,0,"Window Opining...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(300);
	}
	//加速
	for (int i = 80; i >= 60; i = i - 1)
	{
		OLED_ShowString(0,0,"Window Opining...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(160);
	}
	//保持
		Delay_s(1);
	//减速
	for (int i = 60; i <=80; i=i + 1)
	{
		OLED_ShowString(0,0,"Window Opining...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(160);
	}
	//停止
	for (int i = 80 ; i <=100; i=i + 1)
	{
		OLED_ShowString(0,0,"Window Opining...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(160);
	}
	OLED_Clear();
	Serial_Init();
	Delay_ms(50);
	ESP8266_Send(0,"WindowSwitch",1);	
	OLED_ShowString(0,0,"Success!",16,1);
	OLED_Refresh();
	Delay_ms(500);
	memset(Serial_RxPacket,0,500);
	Window_Condition = 1;
	HCSR504_Init();
	TIM1IT_Init();
	TIM2IT_Init();
	TIM4IT_Init();
	KeyIT_Init();
	OLED_Clear();
}

//0倒转停止
void Mortor_OFF(void)
{
	BEEP_OFF();
	HCSR504_DeInit();
	Serial_DeInit();
	TIM1IT_DeInit();
	TIM2IT_DeInit();
	TIM4IT_DeInit();
	KeyIT_DeInit();
	OLED_Clear();
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	//起步
	for (int i = 10; i <=20; i=i+1)
	{
		OLED_ShowString(0,0,"Window Closing...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(300);
	}
	//加速
	for (int i = 20; i <=40; i=i+1)
	{
		OLED_ShowString(0,0,"Window Closing...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(160);
	}
	//保持
		Delay_s(1);
	//减速
	for (int i = 40; i >=20; i = i - 1)
	{
		OLED_ShowString(0,0,"Window Closing...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(160);
	}
	//停止
	for (int i = 20; i>=0; i = i - 1)
	{
		OLED_ShowString(0,0,"Window Closing...",16,1);
		OLED_Refresh();
		PWM_SetCompare3(i);
		Delay_ms(160);
	}
	OLED_Clear();
	Delay_ms(50);
	Serial_Init();
	ESP8266_Send(0,"WindowSwitch",0);	
	OLED_ShowString(0,0,"Success!",16,1);
	OLED_Refresh();
	Delay_ms(500);
	memset(Serial_RxPacket,0,500);
	Window_Condition = 0;
	HCSR504_Init();
	TIM1IT_Init();
	TIM2IT_Init();
	TIM4IT_Init();
	KeyIT_Init();
	OLED_Clear();
}



