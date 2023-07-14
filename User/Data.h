#ifndef __DATA_H
#define __DATA_H
#include "string.h"
struct Data
{
	float humi;
	float temp;
	float Wind_Speed;
	uint32_t PPM;
	uint32_t Lux;
};

extern struct Data Window_Data;
extern uint8_t Window_Condition;
extern uint8_t Wind_Threshold;
extern uint16_t Sun_Threshold;
extern uint16_t Night_Threshold;
extern uint8_t PPM_Threshold;
extern uint16_t Rain_Threshold;
extern uint8_t Threshold_Condition;
extern uint16_t ADC_vaule[4];
extern char Serial_RxPacket[500];
extern char Time[];
extern char Date[];


int findpos(const char* str1, const char* str2);
int findcomma(const char* str1);
float DataAvg(float data);


#endif 
