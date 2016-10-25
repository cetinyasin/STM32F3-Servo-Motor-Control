#include "stm32f30x.h"                  // Device header
#include "PWM.h"
#include "ADC.h"
#include "USART.h"

float data_ADC; // 'll store 12-bit ADC data
float volt_ADC; // (data_ADC/2^12-1)*3
int k=0;
int kontrol=0;
int main(void)
{
	ADC1_Init();
	PWM_Init();
	USART1_Init();
	
	while(1)
	{		
		kontrol = getKontrolInfo();
		if(kontrol == -1)
		{
			data_ADC = Read_ADC1();  
			volt_ADC = (data_ADC/4095)*3;
			if(volt_ADC < 1)
				ServoSur(1);
			else if(volt_ADC>1 & volt_ADC <2)
				ServoSur(3);
			else if(volt_ADC>2)
				ServoSur(5);
			else 
				ServoSur(0);
		}			
		else if(kontrol == 1)
		{
			ServoKonum(getRxBuff());
			for(k=0;k<=8000;k++);	// delay
		}
	}
}
