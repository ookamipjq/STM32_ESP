#include "beep.h"
#include "stm32f10x.h"
#include "delay.h"

/*
		�������Q��Beep_Init
				���ã�Ϊ�������ܽ�����ʼ��
*/
void Beep_Init()
{
	GPIO_InitTypeDef Beep_GPIO;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	Beep_GPIO.GPIO_Pin = GPIO_Pin_15;
	Beep_GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	Beep_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&Beep_GPIO);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void Beep_Open()
{
		GPIO_WriteBit(GPIOB,GPIO_Pin_15,0);
		Delay_ms(200);
		GPIO_WriteBit(GPIOB,GPIO_Pin_15,1);
		Delay_ms(200);
}
void Beep_Close()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,1);
}