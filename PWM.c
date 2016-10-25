#include "stm32f30x.h"                  // Device header
#include "PWM.h"

// Advanced control timer TIM1,
// PA8 > TIM1_CH1

uint16_t i=0;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
TIM_OCInitTypeDef 	TIM_OCInitStructure;

void PWM_Init()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // 1- Enable clock
	
	// 2- Configure corresponding GPIO pins
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3- Configure time base unit
	/* 1/72Mhz * 72(prescalar) = 1/1Mhz; 
	for 50Hz: (1/1Mhz)/(1/50Hz) = 20000 */
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period	    = 20000;
	TIM_TimeBaseInitStructure.TIM_Prescaler     = 72;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	// 4- Fill the TIM_OCInitStruct
	TIM_OCInitStructure.TIM_OCMode	      = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 	      = 600;
	
	// 5- Call TIM_OCxInit(TIMx, &TIM_OCInitStruct)
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	// **  (@) In case of PWM mode, this function is mandatory: TIM_OCxPreloadConfig(TIMx, TIM_OCPreload_ENABLE); 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	// 6- Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


/* basit delay ile git gel */
void ServoSur(int hiz)
{
		TIM_OCInitStructure.TIM_Pulse+=hiz;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
		for(i=0;i<=8000;i++);	// delay
		if(TIM_OCInitStructure.TIM_Pulse > 2200) // basa don
			TIM_OCInitStructure.TIM_Pulse=600;
}

/* gonderilen aci degerini anlamli hale cevirip o aci konumuna gececek. */
void ServoKonum(int Pulse)
{
	TIM_OCInitStructure.TIM_Pulse=(int)(Pulse*8.88 + 600);
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
}
