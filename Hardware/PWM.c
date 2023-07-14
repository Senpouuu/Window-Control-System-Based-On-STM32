#include "stm32f10x.h"                  // Device header
#include "Threshold.h"
void PWM_Init(void)
{
	/*配置GPIO参数*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);


	/*配置AFIO重映射参数*/
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM3,ENABLE);		//开启重映射
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);


	/*配置TIM时基单元参数*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);			//开启TIM3时钟信号
	TIM_InternalClockConfig(TIM3);								//选择内部TIM3时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//选择分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;						//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;					//PSC 此时按照公式 一秒记一次
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//关闭重复计数器
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	/*配置TIM输出比较单元参数*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);							//结构体成员赋初值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//输出比较模式PMW1
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//输出比较极性高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0 ;								//CCR
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	
	TIM_Cmd(TIM3,ENABLE);												//使能TIM3外设


}


void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}


