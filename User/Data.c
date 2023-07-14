#include "string.h"                  // Device header
#include "stm32f10x.h"                  // Device header

int findpos(const char* str1, const char* str2)
{
	const char* temp = strstr(str1, str2);
	return temp - str1;
}

int findcomma(const char* str1)
{
	u8 i;
	for(i = 0;i<strlen(str1);i++)
		if(str1[i] == ',')
			return i;
		return i;
}

float DataAvg(float data)
{
	float sum;
	for(u8 i = 0;i<10; i++)
		sum = sum + data;
	return sum/10;
}

