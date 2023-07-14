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
/*��ʱ���жϳ�ʼ��*/
void TIM2IT_Init(void)
{	
	/*����TIM����*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//����TIM2ʱ���ź�
	TIM_InternalClockConfig(TIM2);								//ѡ���ڲ�TIM2ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//ѡ���Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//ѡ�����ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 40000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;					//PSC ��ʱ���չ�ʽ �����һ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//�ر��ظ�������
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);							//ʹ��TIM2�ж�,��Ϊ�����ж�
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);								//��ֹ�����ͽ��ж�

	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			//ѡ��TIM2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2,ENABLE);									//ʹ��TIM2����
	
}

void TIM2IT_DeInit(void)
{
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);							//ʹ��TIM2�ж�,��Ϊ�����ж�
}

void TIM2_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)						//�жϱ�־λ�жϣ�ȷ�Ͻ����ж�
	{	
		Upload = 1;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

}


