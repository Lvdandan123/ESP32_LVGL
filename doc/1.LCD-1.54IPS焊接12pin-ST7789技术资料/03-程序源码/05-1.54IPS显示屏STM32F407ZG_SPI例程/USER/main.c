//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-0101
//  最近修改   : 
//  功能描述   :演示例程(STM32F4系列)
//              说明: 
//              ----------------------------------------------------------------
//             12 GND    电源地
//             11 RES   PD4
//             10 SDA   PD5（MOSI）
//             9 SCL   PG12（SCLK）
//             8 CS    PD1
//             7 DC    PD15
//             3 VCC  接5V或3.3v电源
//             2 BLK   PE8
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//  生成日期   : 2018-0101
//All rights reserved
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"


int main(void)
{ 
	u8 i,j;
	static unsigned int cnt = 0;

	float t=0;
	delay_init(168);
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LED0=0;
	while (1)
	{
		delay_ms(1000);
		cnt++;
		switch (cnt)
		{
			case 1:
				LCD_Fill(0,0,LCD_W,LCD_H,RED);
				break;
			case 2:
				LCD_Fill(0,0,LCD_W,LCD_H,BLUE);
				break;
			case 3:
				LCD_Fill(0,0,LCD_W,LCD_H,GREEN);
                cnt = 0;
				break;
			default:
				break;

		}
		LCD_ShowChinese(0,0,"中景园电子",RED,WHITE,32,0);
		LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
		LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
		LCD_ShowFloatNum1(128,70,t,4,RED,WHITE,16);
		t+=0.11f;
		for(j=0;j<3;j++)
		{
			for(i=0;i<6;i++)
			{
				LCD_ShowPicture(40*i,120+j*40,40,40,gImage_1);
			}
		}
	}
}
