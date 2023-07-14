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
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);		//����EXIT1��Ϊ�ж��ź�
	
	/*�жϳ�ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;				//EXIT14�ж��ź�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//�����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش���
	EXTI_Init(&EXTI_InitStructure);
	
	/*����NVIC���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	//ѡ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0	;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
}

void HCSR504_DeInit(void)
{

	/*�жϳ�ʼ��*/
	EXTI_InitTypeDef EXTI_InitStructure;		
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;				//EXIT14�ж��ź�
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;					//�����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//�����ش���
	EXTI_Init(&EXTI_InitStructure);
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)				//�ж��Ƿ���EXIT14�������ж�
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 1 && HCSwi == 1)
			HC = 1;
		//while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 1);		//�ȴ��ߵ�ƽ����
		EXTI_ClearITPendingBit(EXTI_Line5);				//����жϱ�־λ����ֹѭ���ж�
	}


}


