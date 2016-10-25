#include "stm32f30x.h"                  // Device header
#include "ADC.h"

// PA0 -> ADC1_IN1 

void ADC1_Init()
{
	// Init Structures
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	// Clocks
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);             // Configure ADC Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE); // ADC1 ve ADC2 anlaminda. birde 34 versiyonu var.
	
	// Configure GPIOA_0 as Analog Input (ADC1_IN1)
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; // NoPull 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Calibration procedure
  	ADC_VoltageRegulatorCmd(ADC1, ENABLE);
	
  	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  	ADC_StartCalibration(ADC1);
  	while(ADC_GetCalibrationStatus(ADC1) != RESET); 
		
	// Configure ADC Common
	ADC_CommonInitStructure.ADC_Mode  = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
	ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
	
	// Configure ADC
	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable; 
 	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;	
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigEventEdge_None;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5); // ADC1_IN1 
	
	ADC_Cmd(ADC1, ENABLE);
	
	while(!(ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY))); // wait ADC1 Ready Flag
	
	ADC_StartConversion(ADC1); // SW Start
}

uint16_t Read_ADC1(void)
{
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);   // End of regular conversion flag
	return ADC_GetConversionValue(ADC1); 		       // 12-bit. [0-4095]
}
