#include "stm32f10x.h"                  // Device header
extern uint16_t ADC_vaule[4];

void DriectMemoryAccess_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	/*外设*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//数据1基地址 ADC1寄存器地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//自增关闭
	/*存储器*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_vaule;					//数据2基地址	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//字节方式传输
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//自增开启
	DMA_InitStructure.DMA_BufferSize = 4;										//传输次数
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//传输方向（存储器到存储器）
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//硬件触发or软件触发 (ADC1触发)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//循环模式（存储器到存储器不能使用自动重装）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//优先级高
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);									//ADC1 连 DMA1
	
	ADC_DMACmd(ADC1,ENABLE);													//ADC1 DMA触发源开启
	DMA_Cmd(DMA1_Channel1,ENABLE);

}


void DMA_Reset(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);												//关闭
	DMA_SetCurrDataCounter(DMA1_Channel1,4);									//重置计数器
	DMA_Cmd(DMA1_Channel1,ENABLE);	
}

