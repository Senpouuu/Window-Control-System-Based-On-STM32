#include "stm32f10x.h"                  // Device header
void Beep_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
}

void BEEP_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void BEEP_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

 void BEEP_Turn(void)
{
	/*如果是低电平，就给他一个相反的信号，高电平同理*/
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
	else
		GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
}
