#ifndef __MQ_2_H
#define __MQ_2_H
#include "stm32f10x.h"                  // Device header


float MQ5_GetVolt(void);
float MQ5_CalRs(void);
float MQ5_CalR0(void);
float MQ5_CalPPM(float a,float b);

#endif

