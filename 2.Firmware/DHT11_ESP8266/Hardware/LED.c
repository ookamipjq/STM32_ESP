#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "delay.h"
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //PC13
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}
void LED_C(void){
	int i =0;
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	for(;i<5;i++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_ms(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

