#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "MQ-2.h"
#include "LUX.h"
#include "ESP8266.h"
#include "Data.h"
#include "ADC.h"
#include "Speed.h"
#include "math.h"
#include "Mortor.h"
#include "Delay.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>

extern u8 Threshold;
uint8_t Wind_Threshold = 8;
uint16_t Sun_Threshold = 8000;
uint16_t Night_Threshold = 1;
uint8_t PPM_Threshold = 200;
uint16_t Rain_Threshold = 800;
uint8_t Threshold_Condition = 1;

void TIM4IT_Init(void)
{
	/*配置TIM参数*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);			//开启TIM4时钟信号
	TIM_InternalClockConfig(TIM4);								//选择内部TIM4时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;					//PSC 此时按照公式 1秒记一次
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//关闭重复计数器
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);							//使能TIM4中断,记为更新中断
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);								//防止开机就进中断

	/*配置NVIC优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;			//选择TIM4中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4,ENABLE);									//使能TIM4外设
}

void TIM4IT_DeInit(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);							//使能TIM2中断,记为更新中断
}

//烟雾阈值 关-开
void Threshold_PPM(void)
{
	if(Threshold_Condition == 1 && Window_Condition == 0)			//条件开启，且窗户关闭
		if(Window_Data.PPM > PPM_Threshold)
		{

			Threshold_Condition = 0;
			Mortor_ON();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",2);	
		}
	Threshold_Condition = 1;
	
}

//光照阈值
void Threshold_Lux(void)
{
	//关->开
	if(Threshold_Condition == 1 && Window_Condition == 0)			//条件开启，且窗户关闭
		if(Window_Data.Lux > Sun_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_ON();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",4);	
		}
	//开->关
	if(Threshold_Condition == 1 && Window_Condition == 1)			//条件开启，且窗户关闭
		if(Window_Data.Lux < Night_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_OFF();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",5);	
		}
	Threshold_Condition = 1;
}

//风速阈值 开->关
void Threshold_Wind(void)
{
	if(Threshold_Condition == 1 && Window_Condition == 1)			//条件开启，且窗户关闭
		if(Window_Data.Wind_Speed > Wind_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_OFF();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",3);	
		}
	Threshold_Condition = 1;

}


//雨水阈值 开->关
void Threshold_Rain(void)
{
	if(Threshold_Condition == 1 && Window_Condition == 1)			//条件开启，且窗户关闭
		if(ADC_vaule[3] < Rain_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_OFF();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",1);	
		}
	Threshold_Condition = 1;
}



void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)						//中断标志位判断，确认进入中断
	{	
			Threshold = 1;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}


