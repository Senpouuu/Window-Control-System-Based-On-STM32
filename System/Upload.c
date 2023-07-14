#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "MQ-2.h"
#include "LUX.h"
#include "ESP8266.h"
#include "Data.h"
#include "ADC.h"
#include "Speed.h"
#include "math.h"
#include "Delay.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>
struct Data Window_Data;
extern u8 Upload;
/*定时器中断初始化*/
void TIM2IT_Init(void)
{	
	/*配置TIM参数*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//开启TIM2时钟信号
	TIM_InternalClockConfig(TIM2);								//选择内部TIM2时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 40000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;					//PSC 此时按照公式 五秒记一次
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//关闭重复计数器
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);							//使能TIM2中断,记为更新中断
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);								//防止开机就进中断

	/*配置NVIC优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//选择TIM2中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2,ENABLE);									//使能TIM2外设
	
}

void TIM2IT_DeInit(void)
{
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);							//使能TIM2中断,记为更新中断
}

void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)						//中断标志位判断，确认进入中断
	{	
		Upload = 1;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

}


