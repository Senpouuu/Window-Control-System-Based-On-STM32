#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Threshold.h"
float Count = 0;					//计数
uint16_t Round = 8;					//轮子周长 		
float Velocity = 0;					//速度


/*测速中断初始化*/
void SpeedIT_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);		//设置PA6作为中断信号

	/*中断初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;				//EXIT6中断信号
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//模式中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	/*配置NVIC优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;		//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI4_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line4) == SET)				//判断是否是EXIT14产生的中断
		Count++;
	EXTI_ClearITPendingBit(EXTI_Line4);					//清除中断标志位，防止循环中断
}


/*定时器中断初始化*/
void TIM1IT_Init(void)
{	
	/*配置TIM时基单元参数*/		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);					//开启TIM1时钟信号
	TIM_InternalClockConfig(TIM1);										//选择内部TIM1时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;					//PSC 200ms
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//关闭重复计数器
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	
	/*配置定时器中断*/
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);  								//手动清除中断标志位，避免刚初始化完就进入中断
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  							//打开更新中断到NVIC的通道。使能中断


	/*配置NVIC优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);						//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;						//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM1,ENABLE);
}

void TIM1IT_DeInit(void)
{
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);  							//打开更新中断到NVIC的通道。使能中断
}

void TIM1_UP_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)						//中断标志位判断，确认进入中断
	{
		Velocity = ((Count/20)*0.08)/0.2;
		Count = 0;
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update); 						//清除中断标志位
}








