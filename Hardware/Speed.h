#ifndef __SPEED_H
#define __SPEED_H
#include "stm32f10x.h"                  // Device header
extern float Velocity;					//速度
extern float Count;					//计数
void TIM1IT_Init(void);
void SpeedIT_Init(void);
void TIM1IT_DeInit(void);

#endif


