#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum;		//定义用于接收按键键码的变量
int8_t Speed;		//定义速度变量

int main(void) {
    /*模块初始化*/
//	OLED_Init();
//	OLED_ShowString(1,1,"HelloWorld!");
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 禁用 JTAG/SWD 调试引脚
	Motor_Init();
	Key_Init();


    while (1) {
		KeyNum = Key_GetNum();				//获取按键键码
		if (KeyNum == 2)					//按键1按下
		{
			Speed += 20;					//速度变量自增20
			if (Speed > 100)				//速度变量超过100后
			{
				Speed = -100;				//速度变量变为-100
											//此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
											//若出现了此现象，则应避免使用这样的操作
			}
		}
		if (KeyNum == 1)					//按键1按下
		{
			Speed -= 20;					//速度变量自增20
			if (Speed < -100)				//速度变量超过100后
			{
				Speed = 100;				//速度变量变为-100
											//此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
											//若出现了此现象，则应避免使用这样的操作
			}
		}
		Motor_SetSpeedA(Speed);				//设置直流电机的速度为速度变量
		Motor_SetSpeedB(Speed);
        
	}
}
