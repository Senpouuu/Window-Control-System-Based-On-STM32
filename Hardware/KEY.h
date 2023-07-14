#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header

void Key_Init(void);
uint8_t Key_GetNum(void);
void KeyIT_Init(void);
uint8_t KeyCount(uint8_t KeyNum);
void KeyIT_DeInit(void);
void HCSR504Key_Init(void);
#endif

