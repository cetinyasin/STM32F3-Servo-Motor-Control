#include "stm32f30x.h"                  // Device header
#include "USART.h"
#include <string.h>
#include <stdlib.h>

// PB6: USART1_TX
// PB7: USART1_RX

unsigned char i_val;
unsigned char rx, e=0;
unsigned char rxbuff[4], valid_data[4];
// USART1 Init
void USART1_Init()
{
	// Init Structures
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	// Clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	// GPIO AF Config
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_7); // datasheet sayfa 44
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_7); 
	
	// PB6: TX, PB7: RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// USART1 Config
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	
	// Enable RXNE interrupt 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  // Enable USART1 global interrupt
  NVIC_EnableIRQ(USART1_IRQn);
}

/* main'den ulasip servoya basmak icin */
int getRxBuff(void)
{
	return i_val;
}

/* send string */
void USART1_Puts(volatile char *s)
{
	while(*s)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, *s);
		*s++;
	}
}

/***********************************/
// 				int -->>>  str
/***********************************/
char str[3];
char* intToStr(int32_t data) {
	uint8_t i=2;
	strcpy(str,"   \n");
	if (data==0) str[i]=48;
	else	while (data>0) {
		str[i]=(data%10)+48;
		data/=10;
		i--;
		}
	return str;
}

int j=0;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // data receive
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		rx = (unsigned char) USART_ReceiveData(USART1);
		rxbuff[e] = rx;
		e++;
	}
	if(e == 4) 
	{
		int checksum = 0;
		for(j=0;j<3;j++)
		{
			checksum = checksum ^ rxbuff[j];
		}
		if(checksum == rxbuff[3]) // dogru geldi demektir(hata olasiligi 256da 1)
		{
			for(j=0; j<e; j++)
			{
				valid_data[j] = rxbuff[j]; // yedekleme.
			}
			data_process();
			e = 0;
		}
		else{/* check = -1; veriyi tekrar iste.*/}
	}  
}

int u_kontrol=0;
void data_process(){
	if(valid_data[0] == 0x01) 	// veri bana mi ait?
	{
		if(valid_data[1] == 0x0B) // aci verme durumu: servoyu istenen konuma dondur, aciyi geri gonder.
		{
			u_kontrol = 1;
			i_val = valid_data[2];
			USART1_Puts(intToStr(i_val));
		}
		else if(valid_data[1] == 0x0A)
		{
			u_kontrol = -1;
		}
	}
}

int getKontrolInfo()
{
	return u_kontrol;
}
