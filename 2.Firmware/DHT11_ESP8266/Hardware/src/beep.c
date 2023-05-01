/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	led.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		BEEP��ʼ��������
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "beep.h"



BEEP_INFO beepInfo = {0};


/*
************************************************************
*	�������ƣ�	Beep_Init
*
*	�������ܣ�	��������ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Beep_Init(void)
{

	GPIO_InitTypeDef gpioInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//��GPIOB��ʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//��ֹJTAG����
	
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
	gpioInitStruct.GPIO_Pin = GPIO_Pin_3;						//����ʼ����Pin��
	gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	
	GPIO_Init(GPIOB, &gpioInitStruct);							//��ʼ��GPIO
	
	Beep_Set(BEEP_OFF);											//��ʼ����ɺ󣬹رշ�����

}

/*
************************************************************
*	�������ƣ�	Beep_Set
*
*	�������ܣ�	����������
*
*	��ڲ�����	status�����ط�����
*
*	���ز�����	��
*
*	˵����		��-BEEP_ON		��-BEEP_OFF
************************************************************
*/
void Beep_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, status == BEEP_ON ? Bit_SET : Bit_RESET);		//���status����BEEP_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
	beepInfo.Beep_Status = status;

}