#ifndef __UPLOAD_H
#define __UPLOAD_H
#include "stm32f10x.h"                  // Device header

void TIM2IT_Init(void);
void TIM2IT_DeInit(void);
void Upload_Data(uint8_t a,char* b,float c);

	
#endif
