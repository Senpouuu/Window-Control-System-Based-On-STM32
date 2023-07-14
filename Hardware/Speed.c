#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Threshold.h"
float Count = 0;					//����
uint16_t Round = 8;					//�����ܳ� 		
float Velocity = 0;					//�ٶ�


/*�����жϳ�ʼ��*/
void SpeedIT_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);		//����PA6��Ϊ�ж��ź�

	/*�жϳ�ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;				//EXIT6�ж��ź�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_Init(&EXTI_InitStructure);
	
	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;		//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI4_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line4) == SET)				//�ж��Ƿ���EXIT14�������ж�
		Count++;
	EXTI_ClearITPendingBit(EXTI_Line4);					//����жϱ�־λ����ֹѭ���ж�
}


/*��ʱ���жϳ�ʼ��*/
void TIM1IT_Init(void)
{	
	/*����TIMʱ����Ԫ����*/		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);					//����TIM1ʱ���ź�
	TIM_InternalClockConfig(TIM1);										//ѡ���ڲ�TIM1ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//ѡ�񲻷�Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//ѡ�����ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;					//PSC 200ms
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//�ر��ظ�������
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	
	/*���ö�ʱ���ж�*/
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);  								//�ֶ�����жϱ�־λ������ճ�ʼ����ͽ����ж�
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  							//�򿪸����жϵ�NVIC��ͨ����ʹ���ж�


	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);						//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;						//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM1,ENABLE);
}

void TIM1IT_DeInit(void)
{
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);  							//�򿪸����жϵ�NVIC��ͨ����ʹ���ж�
}

void TIM1_UP_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)						//�жϱ�־λ�жϣ�ȷ�Ͻ����ж�
	{
		Velocity = ((Count/20)*0.08)/0.2;
		Count = 0;
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update); 						//����жϱ�־λ
}








