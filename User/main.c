#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "pid.h"
#include "Timer.h"
#include "Serial.h"
#include "gray_track.h"
#include "MPU6050.h"

#define ADDKey 2
#define DECKey 1
#define SETKey 3

#define LEFT 1
#define RIGHT 2



/*------- ------IMU变量------------- ---*/

int16_t Ax,Ay,Az,Gx,Gy,Gz;

// 量程缩放示例 (假设 ±2000°/s 对应缩放因子约为 16.4，需根据实际配置)
#define GYRO_SCALE 16.4f

// 陀螺仪零漂：可在静止状态下测量一段时间的数据做平均值校准
static float gyroOffsetZ = 0.0f;

// 偏航角，全局维护
static float yaw = 0.0f;

// 中断周期（秒），由你的硬件配置得知 (100ms = 0.1s)
#define DT 0.1f


/*-------------按键变量-------------------*/

uint8_t KeyNum;		//定义用于接收按键键码的变量
uint8_t KeyNumMenu=1;		//定义用于接收按键页码
uint8_t KeyNumMenuTotal=2;		//定义用于接收按键页码的总和
uint8_t MenuFlag=0;        //定义菜单标志,0为主菜单,1为子菜单

uint8_t LastKeyNumMenu=1;		//定义用于上一次接收按键页码
uint8_t LastMenuFlag=0;        //上一次定义菜单标志,0为主菜单,1为子菜单



/*-------------电机变量-------------------*/

int16_t Speed;		//定义速度变量

Encoder ecd_left;
Encoder ecd_right;

PID vec_left;
PID vec_right;

/*-------------函数声明-------------------*/

void myCarControlCodeInit(){
    Parameter param = {4, 28, 13, 0.065};
    initEncoder(&ecd_left,param);
    initEncoder(&ecd_right,param);
    //vec pid init
    initPID(&vec_left, 2100, 5000);
    setPIDParam(&vec_left, 11, 0.5,0.5);
    setPIDTarget(&vec_left, 0);

    initPID(&vec_right, 2100, 5000);
    setPIDParam(&vec_right, 11, 0.5,0.5);
    setPIDTarget(&vec_right, 0);

}

/*-------------主函数-------------------*/

int main(void) {
    /*模块初始化*/
    OLED_Init();
	Motor_Init();
    Encoder_Init_TIM_All();
    MPU6050_Init();
    Delay_ms(1000);//延时1s

    Timer_Init();//定时器初始化
	Key_Init();
	Serial_Init();
	gray_init();

    /*变量初始化*/
    myCarControlCodeInit();

    /*主循环*/
    while (1) {

        /*电机速度控制*/
        Motor_SetSpeedA(vec_left.output);
        Motor_SetSpeedB(vec_right.output);


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
                OLED_ShowString(2,1,"2 Test");//2 Menu2
//                OLED_ShowString(1,5,"3 Menu3");//3 Menu3
//                OLED_ShowString(1,7,"4 Menu4");//4 Menu4
            }
            if (KeyNumMenu==2)
            {
                OLED_ShowString(1,1,"1 Speed");//1 Speed
                OLED_ShowString(2,1,"X Test");//2 Menu2
//                OLED_ShowString(1,5,"3 Menu3");//3 Menu3
//                OLED_ShowString(1,7,"4 Menu4");//4 Menu4
            }
        }

        /*菜单操作*/
        if (MenuFlag==1){

            /*  速度操作 */
            if (KeyNumMenu==1)
            {
                OLED_ShowString(1,1,"Speed");//1 Speed
                OLED_ShowSignedNum(2,1,Speed,5);//SpeedNum
                OLED_ShowSignedNum(3,1,(int32_t)ecd_left.counter.count_increment,8);//
                OLED_ShowSignedNum(4,1,(int32_t)ecd_right.counter.count_increment,8);//

//                Serial_Printf("Speed:%lld,",ecd_left.counter.count_increment);
//				Serial_Printf("%lld\n",ecd_right.counter.count_increment);

            }
            /*  Test */
            if (KeyNumMenu==2)//目前测试的进程
            {

               OLED_ShowNum(1, 1, D1, 1);
               OLED_ShowNum(1, 2, D2, 1);
               OLED_ShowNum(1, 3, D3, 1);
               OLED_ShowNum(1, 4, D4, 1);
               OLED_ShowNum(1, 5, D5, 1);
               OLED_ShowNum(1, 6, D6, 1);
               OLED_ShowNum(1, 7, D7, 1);
               OLED_ShowNum(1, 8, D8, 1);
				//track();

            }







	    }
		


        


    }//while函数的大括号










}//main函数的大括号



void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        updateEncoderLoopSimpleVersion(&ecd_left, 100, TIM2);
        updateEncoderLoopSimpleVersion(&ecd_right, 100, TIM4);

        updatePID(&vec_left, ecd_left.counter.count_increment);
        updatePID(&vec_right, ecd_right.counter.count_increment);

        MPU6050_GetData(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);

        // 2. 去除零漂，并转换为度/秒
        float gz = (float)(Gz - (int16_t)gyroOffsetZ) / GYRO_SCALE;

        // 3. 积分计算 yaw （简单积分，单位：度）
        yaw += gz * DT;

        // 4. 限制 yaw 的范围在 -180° 到 180° 之间
        if (yaw > 180.0f)
            yaw -= 360.0f;
        else if (yaw < -180.0f)
            yaw += 360.0f;

        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}//速度计时器中断服务函数
