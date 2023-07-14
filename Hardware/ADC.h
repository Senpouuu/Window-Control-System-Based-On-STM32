#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"                  // Device header

void AD_Init(void);
void ADC_GetValue(void);
extern uint16_t ADC_vaule[4];

#endif

