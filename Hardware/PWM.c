#include "stm32f10x.h"                  // Device header
#include "Threshold.h"
void PWM_Init(void)
{
	/*����GPIO����*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);


	/*����AFIO��ӳ�����*/
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM3,ENABLE);		//������ӳ��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);


	/*����TIMʱ����Ԫ����*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);			//����TIM3ʱ���ź�
	TIM_InternalClockConfig(TIM3);								//ѡ���ڲ�TIM3ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//ѡ���Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//ѡ�����ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;						//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;					//PSC ��ʱ���չ�ʽ һ���һ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//�ر��ظ�������
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	/*����TIM����Ƚϵ�Ԫ����*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);							//�ṹ���Ա����ֵ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//����Ƚ�ģʽPMW1
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		//����Ƚϼ��Ըߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�������ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0 ;								//CCR
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	
	TIM_Cmd(TIM3,ENABLE);												//ʹ��TIM3����


}


void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}


