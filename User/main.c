#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "Delay.h"
#include "ESP8266.h"
#include "OLED.h"
#include "DHT11.h"
#include "LED.h"
#include "LUX.h"
#include "KEY.h"
#include "ADC.h"
#include "Mortor.h"
#include "HCSR504.h"
#include "BEEP.h"
#include "Threshold.h"
#include "Speed.h"
#include "PWM.h"
#include "Data.h"
#include "Upload.h"
#include "Serial.h"
#include "math.h"
#include "MQ-2.h"
u8 times;
u8 BMP[]={0x03,0x3F,0x42,0x81,0x81,0x81,0x46,0x00};/*"¡æ",0*/
extern uint8_t HCSwi;
extern uint8_t HC;

u8 Upload = 0;
u8 Threshold = 0;
u8 Serial_ON = 0;
u8 Serial_OFF = 0;
u8 Key = 0;
u16 value = 0;
int main(void)
{	
	Serial_Init();
	OLED_Init();
	OLED_ScrollDisplayWiFi(16,"Connecting Wi-Fi",2,24,1);
	OLED_Clear();
	printf("AT+NTP\r\n");
	OLED_ScrollDisplayTime(16,"Getting Time From Sever",2,24,1);
	OLED_Clear();
	OLED_ShowChinese(24,16,0,24,1);
	OLED_ShowChinese(48,16,1,24,1);
	OLED_ShowChinese(72,16,2,24,1);
	OLED_Refresh();
	ESP8266_Init();
	Mortor_Init();
	PWM_Init();
	AD_Init();
	SpeedIT_Init();
	TIM1IT_Init();
	TIM2IT_Init();
	TIM4IT_Init();
	KeyIT_Init();
	HCSR504Key_Init();
	HCSR504_Init();
	LED_Init();
	OLED_Clear();
	
	while(1)
	{

		//HC
		if(HC == 1)
		{
			BEEP_ON();
			delay_ms(1000);
			ESP8266_Send(0,"BreakTimes",1);	
			HC = 0;
		}
		else
			BEEP_OFF();
		
		//UPload
		if(Upload == 1)
		{
			LED6_ON();
			
			ESP8266_Send(0,"Temp",Window_Data.temp);
			Delay_ms(80);
			
			ESP8266_Send(0,"Humi",Window_Data.humi);
			Delay_ms(80);
			
			ESP8266_Send(0,"WindSpeed",Window_Data.Wind_Speed);
			Delay_ms(80);
			
			ESP8266_Send(0,"PPM",Window_Data.PPM);				
			Delay_ms(80);
			
			ESP8266_Send(0,"Lux",Window_Data.Lux);	
			Delay_ms(80);
			
			LED6_OFF();
			Upload = 0;
		}
		
		//ThresHold
		if(Threshold == 1)
		{
			Window_Data.PPM = MQ5_CalPPM(97124,-4.918);
			DHT11_Read_data(&humi,&temp,&dechumi,&dectemp);
			Window_Data.temp = temp +  (dectemp*0.1);
			Window_Data.humi = humi + (dechumi*0.1);
			Window_Data.Wind_Speed = Velocity;
			for(int i=0; i<10; i++)
				value = value + ADC_vaule[2];
			value = (u16)value/10;
			Window_Data.Lux = 39810*pow(2.71828,-0.01534*value)+3467*pow(2.71828,-0.002379*value);
			
			if(Window_Data.PPM>200)
				LED8_ON();
			else
				LED8_OFF();
			
			printf("AT+NTP\r\n");
			Threshold_PPM();
			Threshold_Lux();
			Threshold_Wind();
			Threshold_Rain();
			Threshold = 0;
		}
		
		//Serial
		if(Serial_ON == 1)
		{
			Mortor_ON();
			Window_Condition = 1;
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",7);	
			Serial_ON = 0;
		}
		if(Serial_OFF == 1)
		{
			Mortor_OFF();
			Window_Condition = 0;
			Delay_ms(40);
			ESP8266_Send(0,"LogTag",6);	
			Serial_OFF = 0;
		}
		
		//KEY
		if(Key == 1)
		{
			if(Window_Condition == 1)
			{
				Mortor_OFF();
				Window_Condition = 0;
				Delay_ms(40);
				ESP8266_Send(0,"LogTag",6);	
			}
			else
			{
				Mortor_ON();
				Window_Condition = 1;
				Delay_ms(40);
				ESP8266_Send(0,"LogTag",7);	
			}
			Key = 0;
		}
		
		
		OLED_ShowString(0,0,(u8*)Date,8,1);
		OLED_ShowString(64,0,(u8*)Time,8,1);
		OLED_ShowString(0,8,"Temp:",12,1);
		OLED_ShowString(64,8,"Humi:",12,1);
		OLED_ShowNum(30,8,Window_Data.temp,2,12,1);
		OLED_ShowNum(94,8,Window_Data.humi,2,12,1);
		OLED_ShowPicture(48,10,8,8,BMP,1);
		OLED_ShowString(110,8,"%",12,1);
		
		
		OLED_ShowString(0,20,"Wind:",12,1);
		OLED_ShowString(64,20,"m/s",12,1);
		OLED_ShowWindDecNum(30,20,Window_Data.Wind_Speed,1,12,1);	
		OLED_ShowString(98,34,"State",8,1);
		
		if(Window_Condition == 1)
			OLED_ShowString(98,46,"Open",8,1);
		else
			OLED_ShowString(98,46,"Close",8,1);
		
		OLED_ShowString(0,32,"PPM :",12,1);
		OLED_ShowString(64,32,"mg/L",12,1);
		OLED_ShowNum(30,32,Window_Data.PPM,4,12,1);
		
		OLED_ShowString(0,44,"Lux :",12,1);
		OLED_ShowString(64,44,"lx",12,1);
		OLED_ShowNum(30,44 ,Window_Data.Lux,4,12,1);
		
		
		//OLED_ShowChinese(0,0,0,16,1);
		OLED_ShowString(0,56,"By.201952290203",8,1);
		//OLED_ShowNum(100,56,HCSwi,1,8,1);
		//OLED_ScrollDisplayString(128,56,"Window Control System By.WangHongSen201952290203",2,8,1);	
		
		
		OLED_Refresh();
	}
	
}



