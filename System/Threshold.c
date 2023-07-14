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
	/*����TIM����*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);			//����TIM4ʱ���ź�
	TIM_InternalClockConfig(TIM4);								//ѡ���ڲ�TIM4ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//ѡ���Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//ѡ�����ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;					//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;					//PSC ��ʱ���չ�ʽ 1���һ��
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//�ر��ظ�������
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);							//ʹ��TIM4�ж�,��Ϊ�����ж�
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);								//��ֹ�����ͽ��ж�

	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;			//ѡ��TIM4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4,ENABLE);									//ʹ��TIM4����
}

void TIM4IT_DeInit(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);							//ʹ��TIM2�ж�,��Ϊ�����ж�
}

//������ֵ ��-��
void Threshold_PPM(void)
{
	if(Threshold_Condition == 1 && Window_Condition == 0)			//�����������Ҵ����ر�
		if(Window_Data.PPM > PPM_Threshold)
		{

			Threshold_Condition = 0;
			Mortor_ON();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",2);	
		}
	Threshold_Condition = 1;
	
}

//������ֵ
void Threshold_Lux(void)
{
	//��->��
	if(Threshold_Condition == 1 && Window_Condition == 0)			//�����������Ҵ����ر�
		if(Window_Data.Lux > Sun_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_ON();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",4);	
		}
	//��->��
	if(Threshold_Condition == 1 && Window_Condition == 1)			//�����������Ҵ����ر�
		if(Window_Data.Lux < Night_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_OFF();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",5);	
		}
	Threshold_Condition = 1;
}

//������ֵ ��->��
void Threshold_Wind(void)
{
	if(Threshold_Condition == 1 && Window_Condition == 1)			//�����������Ҵ����ر�
		if(Window_Data.Wind_Speed > Wind_Threshold)
		{
			Threshold_Condition = 0;
			Mortor_OFF();
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",3);	
		}
	Threshold_Condition = 1;

}


//��ˮ��ֵ ��->��
void Threshold_Rain(void)
{
	if(Threshold_Condition == 1 && Window_Condition == 1)			//�����������Ҵ����ر�
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
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)						//�жϱ�־λ�жϣ�ȷ�Ͻ����ж�
	{	
			Threshold = 1;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}


