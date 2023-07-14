#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "string.h"
#include <stdarg.h>
#include "Mortor.h"
#include "Data.h"
#include "ESP8266.h"
#include "Delay.h"
#include "time.h"
#include "stdlib.h"

extern u8 Serial_ON;
extern u8 Serial_OFF;

//uint8_t Serial_RxFlag;
uint8_t Window_Condition = 0;					//Ĭ�Ϲر�
char Serial_RxPacket[500];
int pRxPacket;

char RawTime[25] = "0000000000000000000000000";
char Time[15] = "00000000000000";
char Date[10] = "0000000000";

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure); 

	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure); 
		
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;											//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						//����ģʽ�����ͣ�+ �����գ�
	USART_InitStructure.USART_Parity = USART_Parity_No;									//У��ģʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//�ֳ�
	USART_Init(USART1,&USART_InitStructure);

	/*�����ж�����*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);										//����RXNE��־λ�ж�
	/*����NVIC�ж����ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//����������
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//ѡ��USART1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}


void Serial_DeInit(void)
{
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);										//����RXNE��־λ�ж�
}



void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);														//��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);							//�ȴ����ݽ���Ĵ���
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}


/*printf�ض���*/
int fputc(int ch,FILE *f )   
{
	USART_SendData(USART1,(uint8_t)ch);
	while(USART_GetFlagStatus (USART1,USART_FLAG_TC) == RESET);
	return ch;
}


/*��ָ���жϵ��жϺ���*/
//void USART1_IRQHandler (void)
//{								
//	static uint8_t RxState = 0;								//��������״̬
//	static uint8_t pRxPacket = 0;							//ָ�����յڼ�������;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);
//		
//		if (RxState == 0)									//0 ����ȴ���ͷ����
//		{
//			if (RxData == '#' && Serial_RxFlag == 0)		//ɨ�赽��ͷ
//			{
//				RxState = 1;								//��ת��״̬1
//				pRxPacket = 0;	
//			}
//		}
//		else if (RxState == 1)								//1 ����������ݳ���
//		{
//			if (RxData == '\r')								//ɨ�赽��β
//				RxState = 2;								//��ת��״̬2
//			else											//��ֵ���ݰ�
//			{
//				Serial_RxPacket[pRxPacket] = RxData;		
//				pRxPacket ++ ;
//			}
//		}
//		else if (RxState == 2)								//2 ����ȴ���β����
//		{
//			if (RxData == '\n')								//��βִ�н�������
//			{
//				RxState = 0;
//				Serial_RxPacket[pRxPacket] = '\0';
//				Serial_RxFlag = 1;
//			}
//		}
//		
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//�����־λ
//	}
//} 
//



/*����ָ���жϵ��жϺ���*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
	//Serial_RxFlag = 1;
	//memset(Serial_RxPacket,0,500);														//����������`
		Serial_RxPacket[pRxPacket ++]=USART_ReceiveData(USART1);
		if((Serial_RxPacket[pRxPacket - 2] == '\r')|(Serial_RxPacket[pRxPacket - 1] == '\n'))  
		{
			Serial_RxPacket[pRxPacket - 2] = '\0';
			pRxPacket = 0;
		}
		if (strstr((const char*)Serial_RxPacket,"LED_ON"))			//���ָ��
		{
			Serial_ON = 1;
		}
		if (strstr((const char*)Serial_RxPacket,"LED_OFF"))		//���ָ��
		{
			Serial_OFF = 1;
		}
		if (strstr((const char*)Serial_RxPacket,"2023"))
		{
				char TempTime[15];
				int i = 0, j = 0;
				for (j = 0, i = findpos((const char*)Serial_RxPacket,"2023"); j < 20; j++)
				{
					RawTime[j] = Serial_RxPacket[i];
					i++;
				}
				RawTime[j] = '\0';
				
				for (i = 0; i < 10; i++)
				{
					Date[i] = RawTime[i];
				}
				Date[9] = '\0';
				
				for (j = 0, i = 10; j < 14; j++)
				{
					TempTime[j] = RawTime[i];
					i++;
				}
				TempTime[14] = '\0';
			
				char* hour;
				char* min;
				char* sec;
				hour = strtok(TempTime, ":");
				min = strtok(NULL, ":");
				sec = strtok(NULL, ",");
				sprintf(Time,"%02s:%02s:%02s",hour,min,sec);
				Time[8] = '\0';
				//Time[5] = '\0';
		}	
	}
}


