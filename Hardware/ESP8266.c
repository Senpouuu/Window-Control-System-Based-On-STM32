#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdio.h>
#include "Serial.h"
#include "ESP8266.h"
#include "Delay.h"
#include "LED.h"
#include "Data.h"
#include "OLED.h"

const char* WifiName = "17p";
const char* WifiPass = "senge666";
const char* ProductID="571107";
const char* UserID="316452";
const char* DeviceID="1044148397";
const char* Device_AuthID="ESP8266";
const char* Device_APIKey="P=6=lG7jDwtT1A8FAA1UDWrBCcY=";
const char* Master_APIKey="xrXOFe50Hmp4CakHLqZIjSfMcPQ=";
const char* Access_Key="KhRZHaYc/qO2RkhG4C+SC0pZveY7LyjscBvb5TzLQEg=";
const char* func1 = "Temp";
const char* func2 = "Humi";
const char* func3 = "LEDSwitch";

/*����OneNet*/
void ESP8266_Init(void)
{
	//Serial_Init();
	//while(1)
	//{
	//	if (strstr((const char*)Serial_RxPacket,"WIFI GOT IP"))				//�ȴ�wifi����
	//	{
	//			memset(Serial_RxPacket,0,500);														//����������
	//			break;
	//	}
	//}
	//Delay_ms(2000);
	
	memset(Serial_RxPacket,0,500);
	printf("AT+IOTSTATUS\r\n");							//���õ�ַ
	Delay_ms(2000);
	
	memset(Serial_RxPacket,0,500);
	printf("AT+IOTCFG=%s,%s,%s\r\n",DeviceID,ProductID,Device_AuthID);							//���õ�ַ
	Delay_ms(2000);

	
	memset(Serial_RxPacket,0,500);															//����������
	printf("AT+IOTSUB=Window_Switch\r\n");													//���õ�ַ
	Delay_ms(2000);
	
	ESP8266_Send(0,"WindowSwitch",0);	
	
	
}


/*ESP��������
ָ��AT+IOTSEND=a,b,c,(d)
a��0����������ֵ����
1���������ַ�������
2��������gps
b����������
c������ֵ
d������ֵ (ֻ���ϴ�GPSʱ�Ż��õ�)*/
void ESP8266_Send(uint8_t a,char* b,float c)
{
		printf("AT+IOTSEND=%d,%s,%.1f\r\n",a,b,c);												
}



