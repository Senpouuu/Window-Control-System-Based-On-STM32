#include "stm32f10x.h"                  // Device header
#include "BEEP.h"
#include "Delay.h"
#include "LED.h"
#include "ESP8266.h"
extern u8 times;
extern uint8_t HCSwi;
uint8_t HC = 0;
void HCSR504_Init(void)
{
	Beep_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);		//设置EXIT1作为中断信号
	
	/*中断初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;				//EXIT14中断信号
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//模式中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	/*配置NVIC优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//二级组优先
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0	;		//配置响应优先级
	NVIC_Init(&NVIC_InitStructure);
}

void HCSR504_DeInit(void)
{

	/*中断初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;				//EXIT14中断信号
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;					//开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//模式中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//上升沿触发
	EXTI_Init(&EXTI_InitStructure);
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)				//判断是否是EXIT14产生的中断
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 1 && HCSwi == 1)
			HC = 1;
		//while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 1);		//等待高电平结束
		EXTI_ClearITPendingBit(EXTI_Line5);				//清除中断标志位，防止循环中断
	}


}


