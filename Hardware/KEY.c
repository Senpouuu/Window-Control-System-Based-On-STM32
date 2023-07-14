#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Mortor.h"
#include "Data.h"
#include "ESP8266.h"
#include "BEEP.h"

extern uint8_t KeyNum;
extern u8 Key;
uint8_t HCSwi = 1;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}



void KeyIT_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);		//����EXIT14��Ϊ�ж��ź�

	/*�жϳ�ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;				//EXIT14�ж��ź�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//�����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_Init(&EXTI_InitStructure);
	
	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);

}

void HCSR504Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);		//����EXIT1��Ϊ�ж��ź�
	
	/*�жϳ�ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;				//EXIT14�ж��ź�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//�����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�����ش���
	EXTI_Init(&EXTI_InitStructure);
	
	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0	;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
}



void KeyIT_DeInit(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;				//EXIT14�ж��ź�
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;				//�����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_Init(&EXTI_InitStructure);
}

/*�жϷ�������������жϷ����������������ļ��е�������ͬ*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) == SET)				//�ж��Ƿ���EXIT14�������ж�
	{
		//if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)
		//{
		//	Delay_ms(20);
		//	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0);
		//	Delay_ms(20);
		//}
			Key = 1;
		EXTI_ClearITPendingBit(EXTI_Line12);				//����жϱ�־λ����ֹѭ���ж�
	}
	
	
	if(EXTI_GetITStatus(GPIO_Pin_13) == SET)
	{
		BEEP_OFF();
		if(HCSwi == 1)
		{
			HCSwi = 0;
			LED7_ON();
		}
		else
		{
			HCSwi = 1;
			LED7_OFF();
		}
		EXTI_ClearITPendingBit(EXTI_Line13);				//����жϱ�־λ����ֹѭ���ж�
	}
}
