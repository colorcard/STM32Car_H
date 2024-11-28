#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "pid.h"
#include "Timer.h"

#define ADDKey 2
#define DECKey 1
#define SETKey 3

#define LEFT 1
#define RIGHT 2

#define LEFT_TIM TIM2
#define RIGHT_TIM TIM4



uint8_t KeyNum;		//定义用于接收按键键码的变量
uint8_t KeyNumMenu=1;		//定义用于接收按键页码
uint8_t KeyNumMenuTotal=2;		//定义用于接收按键页码的总和
uint8_t MenuFlag=0;        //定义菜单标志,0为主菜单,1为子菜单

uint8_t LastKeyNumMenu=1;		//定义用于上一次接收按键页码
uint8_t LastMenuFlag=0;        //上一次定义菜单标志,0为主菜单,1为子菜单



int8_t Speed;		//定义速度变量

Encoder ecd_left;
Encoder ecd_right;

PID vec_left;
PID pos_left;
PID vec_right;
PID pos_right;

void myCarControlCodeInit(){
    Parameter param = {4, 28, 13, 0.065};
    initEncoder(&ecd_left,param);
    initEncoder(&ecd_right,param);
    //vec pid init
    initPID(&vec_left, 2100, 5000);
    setPIDParam(&vec_left, 10, 0.5,0.0);

    initPID(&vec_right, 2100, 5000);
    setPIDParam(&vec_right, 10, 0.5,0.0);

    //pos pid init
    initPID(&pos_left, 300, 5000);// 300 -> rpm
    setPIDParam(&pos_left, 0.9, 0.0,0.0);
    setPIDTarget(&pos_left, 720);

    initPID(&pos_right, 300, 5000);
    setPIDParam(&pos_right, 0.9, 0.0,0.0);
    setPIDTarget(&pos_right, 720);
    //runMotorPWMValFB(LEFT,+1000);
}





int main(void) {
    /*模块初始化*/
    OLED_Init();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 禁用 JTAG/SWD 调试引脚
	Motor_Init();
    Encoder_Init_TIM_All();
    Timer_Init();
	Key_Init();


    /*变量初始化*/
    myCarControlCodeInit();





    /*Menu*/
    OLED_ShowString(1,1,"1 Speed");//1 Speed






    while (1) {
        if (LastKeyNumMenu!=KeyNumMenu||LastMenuFlag!=MenuFlag){
            OLED_Clear();
            LastKeyNumMenu=KeyNumMenu;
            LastMenuFlag=MenuFlag;
        }

        KeyNum = Key_GetNum();				//获取按键键码

        if (KeyNum==ADDKey && KeyNumMenu<KeyNumMenuTotal && MenuFlag==0)   KeyNumMenu++;//菜单页码按键加
        if (KeyNum==DECKey && KeyNumMenu>1 && MenuFlag==0)   KeyNumMenu--;//菜单页码按键减
        if (KeyNum==SETKey)  MenuFlag=!MenuFlag;


        /*显示菜单*/
        if (MenuFlag==0)
        {
            if (KeyNumMenu==1)
            {
                OLED_ShowString(1,1,"X Speed");//1 Speed
                OLED_ShowString(1,3,"2 Test");//2 Menu2
//                OLED_ShowString(1,5,"3 Menu3");//3 Menu3
//                OLED_ShowString(1,7,"4 Menu4");//4 Menu4
            }
            if (KeyNumMenu==2)
            {
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
                OLED_Clear();
            }



            /*  Test */
            if (KeyNumMenu==2)//目前测试的进程
            {
                OLED_ShowString(1,1,"Test");//1 Speed
            }







	}



    }//while函数的大括号










}//main函数的大括号



void TIM1_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        updateEncoderLoopSimpleVersion(&ecd_left, 100, (u8)LEFT_TIM);
        updateEncoderLoopSimpleVersion(&ecd_right, 100, (u8)RIGHT_TIM);

//        updatePID(&pos_left, ecd_left.position.angle);
//        setPIDTarget(&vec_left, pos_left.output);
//        updatePID(&vec_left, ecd_left.velocity.angular);
//
//        updatePID(&pos_right, ecd_right.position.angle);
//        setPIDTarget(&vec_right, pos_right.output);
//        updatePID(&vec_right, ecd_right.velocity.angular);
//
//        Motor_SetSpeedA(vec_left.output);
//        Motor_SetSpeedB(vec_right.output);

        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}//速度计时器中断服务函数
