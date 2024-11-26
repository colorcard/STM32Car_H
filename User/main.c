#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"

#define ADDKey 2
#define DECKey 1
#define SETKey 3

uint8_t KeyNum;		//定义用于接收按键键码的变量
uint8_t KeyNumMenu=1;		//定义用于接收按键页码
uint8_t KeyNumMenuTotal=1;		//定义用于接收按键页码的总和
uint8_t MenuFlag=0;        //定义菜单标志,0为主菜单,1为子菜单


int8_t Speed;		//定义速度变量



int main(void) {
    /*模块初始化*/
    OLED_Init();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 禁用 JTAG/SWD 调试引脚
	Motor_Init();
	Key_Init();

    /*Menu*/
    OLED_ShowString(1,1,"1 Speed");//1 Speed






    while (1) {

        KeyNum = Key_GetNum();				//获取按键键码

        if (KeyNum==ADDKey && KeyNumMenu<KeyNumMenuTotal && MenuFlag==0)   KeyNumMenu++;
        if (KeyNum==DECKey && KeyNumMenu>1 && MenuFlag==0)   KeyNumMenu--;
        if (KeyNum==SETKey)  MenuFlag=!MenuFlag;


        /*显示菜单*/
        if (MenuFlag==0)
        {
            if (KeyNumMenu==1)
            {
                OLED_Clear();
                OLED_ShowString(1,1,"X Speed");//1 Speed
                OLED_ShowString(1,3,"2 Test");//2 Menu2
//                OLED_ShowString(1,5,"3 Menu3");//3 Menu3
//                OLED_ShowString(1,7,"4 Menu4");//4 Menu4
            }
            if (KeyNumMenu==2)
            {
                OLED_Clear();
                OLED_ShowString(1,1,"1 Speed");//1 Speed
                OLED_ShowString(1,3,"X Test");//2 Menu2
//                OLED_ShowString(1,5,"3 Menu3");//3 Menu3
//                OLED_ShowString(1,7,"4 Menu4");//4 Menu4
            }
        }

        /*菜单操作*/
        if (MenuFlag==SETKey){

            /*  速度操作 */
            if (KeyNumMenu==1)
            {
                OLED_Clear();
                OLED_ShowString(1,1,"Speed");//1 Speed
                OLED_ShowNum(1,1,Speed,4);//SpeedNum

                if (KeyNum == ADDKey)
                {
                    Speed += 20;					//速度变量自增20
                    if (Speed > 100)				//速度变量超过100后
                    {
                        Speed = -100;				//速度变量变为-100
                        //此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
                        //若出现了此现象，则应避免使用这样的操作
                    }
                }
                if (KeyNum == DECKey)
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



            /*  Test */
            if (KeyNumMenu==2)//目前测试的进程
            {
                OLED_Clear();
                OLED_ShowString(1,1,"Test");//1 Speed
            }







	}



    }//while函数的大括号










}//main函数的大括号
