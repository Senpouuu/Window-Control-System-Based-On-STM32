#include "stm32f10x.h"                  // Device header
#include "DMA.h"
uint16_t ADC_vaule[4];

void AD_Init(void)
{	
	/*RCC开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 		//72/6 = 12Mhz
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ADC模拟输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*ADC配置*/
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;							//设置为ADC独立模式							
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//不使用外部触发模式
	/*转换模式配置（单次转换非扫描）*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//单次转换 	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;								//非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 4;										//4个通道	
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);	//设置规则组通道菜单
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);	//设置规则组通道菜单
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);	//设置规则组通道菜单
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);	//设置规则组通道菜单
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1,ENABLE);														//上电

	/*ADC校准*/
	ADC_ResetCalibration(ADC1);													//开始复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);							//等待复位校准完成
	ADC_StartCalibration(ADC1);													//开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);								//等待校准完成

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);										//开启软件转换
	DriectMemoryAccess_Init();
}


void ADC_GetValue(void)
{
	DMA_Reset();
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);				//等待规则组转换完成
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);							//等待转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1);																//清除标志位
}





