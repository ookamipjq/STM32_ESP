#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "usart.h"
#include "dht11.h"
#include "led.h"
#include "lcd.h"
#include "lcd_init.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//C库
#include <string.h>

u8 b[4];
char PUB_BUF[256];
const char *topics[] = {"my_esp_SUB"};   ///订阅
const char *PUS_topic = "my_esp_PUS";		//发送

void HardWare_Init(void)
{
		LED_Init();
		Usart1_Init(115200);
		Usart2_Init(115200);
		ESP8266_Init();					//初始化ESP8266
}

int main(void)
{
	u8 temp,humi;
	
	unsigned short timeCount = 0;	//发送间隔变量
	
	unsigned char *dataPtr = NULL;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
	
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,BLUE);	
	LCD_ShowString(5,50,"HardWare_Init	",RED,BLUE,16,0);
	Delay_ms(1000);
	HardWare_Init();
	LCD_ShowString(5,50,"HardWare_Init",RED,BLUE,16,0);
	LCD_ShowString(30,90,"successful",RED,BLUE,16,0);
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	while(OneNet_DevLink())			//接入OneNET
		Delay_ms(500);
	OneNet_Subscribe(topics, 1);
	
	if(DHT11_Init()==0){ //DHT11初始化	返回0成功，1失败

		UsartPrintf(USART1,"Init successfu\n\r");
	}
	Delay_ms(1000);//DHT11初始化后必要的延时（不得小于1秒）
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);	
	while(1){
		if(timeCount % 40 == 0)
		{
		if(DHT11_ReadData(&temp,&humi)==0){//读出温湿度值  指针1是湿度 20～90%，指针2是温度 0～50C，数据为十进制
			UsartPrintf(USART1,"湿度:%d;温度:%d\n\r",humi,temp);
			LCD_ShowString(10,60,"humidity:",BLACK,WHITE,16,0);
			LCD_ShowIntNum(82,60,humi,2,BLACK,WHITE,16);
			LCD_ShowString(98,60,"%",BLACK,WHITE,16,0);
			LCD_ShowString(0,80,"temperature:",BLACK,WHITE,16,0);
			LCD_ShowIntNum(97,80,temp,2,BLACK,WHITE,16);
			LCD_ShowString(114,80,"/",BLACK,WHITE,16,0);
		}else{
			UsartPrintf(USART1,"\n 设备连接失败\n\r");
		}
	}
//		Delay_ms(1000); //延时，刷新数据的频率（不得小于1秒）
			
		if(++timeCount >= 200)									//发送间隔5s
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
