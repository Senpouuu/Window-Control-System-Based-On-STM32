#include "stm32f10x.h"                  // Device header
#include "math.h"
extern uint16_t ADC_vaule[4];

float Get_Lux(void)
{
	uint16_t value;
	float Lux;
	for(int i=0; i<10; i++)
		value = value + ADC_vaule[2];
	value = value/10;
	Lux = 39810*pow(2.71828,-0.01534*value)+3467*pow(2.71828,-0.002379*value);
	return Lux;
}

