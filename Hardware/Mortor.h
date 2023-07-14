#ifndef __MORTOR_H
#define __MORTOR_H
#include "stm32f10x.h"                  // Device header

void Motor_SetSpeed(int8_t Speed);
void Mortor_Init(void);
void Mortor_OFF(void);
void Mortor_ON(void);
void Mortor_Appswitch(void);

#endif

