#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "math.h"

extern uint16_t ADC_vaule[4];
float RatioMQ5CleanAir = 6.5;
float Volt_Resolution = 5;	//5V电压
float RL = 10;
float R0 = 19.673;					//R0 in Fresh Air

/*计算电压
Voltage = (avg/ retries) * _VOLT_RESOLUTION / ((pow(2, _ADC_Bit_Resolution)) - 1);
Voltage = (ADC数值*供电电压)÷ADC分辨率 
*/
float MQ5_GetVolt(void)
{
	float Volt;
	float Adc_Avg;
	int temp;
	for(int i = 1;i<10;i++)
		temp += ADC_vaule[0]*2;
	Adc_Avg = temp/10;
	Volt = (Adc_Avg*5)/8190;
	return Volt;
}

float MQ5_CalRs(void)
{
  float RS_air; //Define variable for sensor resistance
	float Volt = MQ5_GetVolt(); 
	RS_air = ((Volt_Resolution * RL) / Volt) - RL;
	if(RS_air < 0)  //No negative values accepted.
		RS_air = 0; 			
	return RS_air;
}

/*计算R0和RS
  V = I x R 
  VRL = [VC / (RS + RL)] x RL 
  VRL = (VC x RL) / (RS + RL) 
  Así que ahora resolvemos para RS: 
  VRL x (RS + RL) = VC x RL
  (VRL x RS) + (VRL x RL) = VC x RL 
  (VRL x RS) = (VC x RL) - (VRL x RL)
  RS = [(VC x RL) - (VRL x RL)] / VRL
  RS = [(VC x RL) / VRL] - RL
 */
float MQ5_CalR0(void)
{
  float RS_air; //Define variable for sensor resistance
  float R0; 		//Define variable for R0
	float Volt = MQ5_GetVolt(); 
	RS_air = ((Volt_Resolution * RL) / Volt) - RL;
	if(RS_air < 0)  //No negative values accepted.
		RS_air = 0; 			
  R0 = RS_air/RatioMQ5CleanAir; 		
  if(R0 < 0)  		//No negative values accepted.
		R0 = 0; 							
  return R0;
}

/*计算PPM
 Exponential regression:
  Gas    | a      | b
  H2     | 1163.8 | -3.874
  LPG    | 80.897 | -2.431
  CH4    | 177.65 | -2.56
  CO     | 491204 | -5.826
  Alcohol| 97124  | -4.918
	_PPM =  a*ratio^b
 */

float MQ5_CalPPM(float a,float b)
{
	float PPM;
	float Rs;
	float ratio;
	Rs = MQ5_CalRs();
	ratio = Rs/R0;
	if(ratio <= 0)  
		ratio = 0; //No negative values accepted or upper datasheet recomendation.
	PPM = a*pow(ratio,b);
	return PPM;
}









