#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "usart.h"
#include "dht11.h"
#include "led.h"
#include "lcd.h"
#include "lcd_init.h"

//����Э���
#include "onenet.h"

//�����豸
#include "esp8266.h"

//C��
#include <string.h>

u8 b[4];
char PUB_BUF[256];
const char *topics[] = {"my_esp_SUB"};   ///����
const char *PUS_topic = "my_esp_PUS";		//����

void HardWare_Init(void)
{
		LED_Init();
		Usart1_Init(115200);
		Usart2_Init(115200);
		ESP8266_Init();					//��ʼ��ESP8266
}

int main(void)
{
	u8 temp,humi;
	
	unsigned short timeCount = 0;	//���ͼ������
	
	unsigned char *dataPtr = NULL;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������
	
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLUE);	
	LCD_ShowString(5,50,"HardWare_Init	",RED,BLUE,16,0);
	Delay_ms(1000);
	HardWare_Init();
	LCD_ShowString(5,50,"HardWare_Init",RED,BLUE,16,0);
	LCD_ShowString(30,90,"successful",RED,BLUE,16,0);
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	while(OneNet_DevLink())			//����OneNET
		Delay_ms(500);
	OneNet_Subscribe(topics, 1);
	
	if(DHT11_Init()==0){ //DHT11��ʼ��	����0�ɹ���1ʧ��

		UsartPrintf(USART1,"Init successfu\n\r");
	}
	Delay_ms(1000);//DHT11��ʼ�����Ҫ����ʱ������С��1�룩
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);	
	while(1){
		if(timeCount % 40 == 0)
		{
		if(DHT11_ReadData(&temp,&humi)==0){//������ʪ��ֵ  ָ��1��ʪ�� 20��90%��ָ��2���¶� 0��50C������Ϊʮ����
			UsartPrintf(USART1,"ʪ��:%d;�¶�:%d\n\r",humi,temp);
			LCD_ShowString(10,60,"humidity:",BLACK,WHITE,16,0);
			LCD_ShowIntNum(82,60,humi,2,BLACK,WHITE,16);
			LCD_ShowString(98,60,"%",BLACK,WHITE,16,0);
			LCD_ShowString(0,80,"temperature:",BLACK,WHITE,16,0);
			LCD_ShowIntNum(97,80,temp,2,BLACK,WHITE,16);
			LCD_ShowString(114,80,"/",BLACK,WHITE,16,0);
		}else{
			UsartPrintf(USART1,"\n �豸����ʧ��\n\r");
		}
	}
//		Delay_ms(1000); //��ʱ��ˢ�����ݵ�Ƶ�ʣ�����С��1�룩
			
		if(++timeCount >= 200)									//���ͼ��5s
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf(PUB_BUF,"{\"Temp\":%d ,\"hum\":%d }",temp,humi);
			
			OneNet_Publish(PUS_topic,PUB_BUF);
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		Delay_ms(10);
	}
}
