#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "pid.h"
#include "Timer.h"
#include "Serial.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"

#define ADDKey 2
#define DECKey 1
#define SETKey 3

#define LEFT 1
#define RIGHT 2

uint64_t millis;                        //millis为当前程序运行的时间，类似arduino里millis()函数

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

MPU6050Params mpu6050 = {
        .MPU6050dt = 10,
        .preMillis = 0,
        .MPU6050ERROE = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
};

SensorMsg msg = {
        .A = {0.0f, 0.0f, 0.0f},
        .G = {0.0f, 0.0f, 0.0f}
};

uint8_t KeyNum;		//定义用于接收按键键码的变量
uint8_t KeyNumMenu=1;		//定义用于接收按键页码
uint8_t KeyNumMenuTotal=2;		//定义用于接收按键页码的总和
uint8_t MenuFlag=0;        //定义菜单标志,0为主菜单,1为子菜单

uint8_t LastKeyNumMenu=1;		//定义用于上一次接收按键页码
uint8_t LastMenuFlag=0;        //上一次定义菜单标志,0为主菜单,1为子菜单

int Temp=0;



int16_t Speed;		//定义速度变量

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
    setPIDParam(&vec_left, 11, 0.5,0.5);
    //setPIDTarget(&vec_left, 100);

    initPID(&vec_right, 2100, 5000);
    setPIDParam(&vec_right, 11, 0.5,0.5);
    //setPIDTarget(&vec_right, 100);

    //pos pid init
    initPID(&pos_left, 300, 5000);// 300 -> rpm
    setPIDParam(&pos_left, 0.9, 0.0,0.0);
    setPIDTarget(&pos_left, 0);

    initPID(&pos_right, 300, 5000);
    setPIDParam(&pos_right, 0.9, 0.0,0.0);
    setPIDTarget(&pos_right, 0);

}

void MPU6050Print() {
    OLED_ShowSignedNum(2, 1, msg.A[0], 3);					  //OLED显示数据
    OLED_ShowNum(2, 6, (uint32_t)(msg.A[0] * 100) % 100, 1);
    OLED_ShowSignedNum(3, 1, msg.A[1], 3);
    OLED_ShowNum(3, 6, (uint32_t)(msg.A[1] * 100) % 100, 1);
    OLED_ShowSignedNum(4, 1, msg.A[2], 3);
    OLED_ShowNum(4, 6, (uint32_t)(msg.A[2] * 100) % 100, 1);
    OLED_ShowSignedNum(2, 8, msg.G[0], 3);
    OLED_ShowNum(2, 13, (uint32_t)(msg.G[0] * 100) % 100, 1);
    OLED_ShowSignedNum(3, 8, msg.G[1], 3);
    OLED_ShowNum(3, 13, (uint32_t)(msg.G[1] * 100) % 100, 1);
    OLED_ShowSignedNum(4, 8, msg.G[2], 3);
    OLED_ShowNum(4, 13, (uint32_t)(msg.G[2] * 100) % 100, 1);
}

void EularPrint() {
    OLED_ShowString(2, 1, "Yaw:");
    OLED_ShowSignedNum(2, 8, getYaw(), 3);
    OLED_ShowNum(2, 13, (uint32_t)(getYaw() * 100) % 100, 2);
    OLED_ShowString(3, 1, "Roll:");
    OLED_ShowSignedNum(3, 8, getRoll(), 3);
    OLED_ShowNum(3, 13, (uint32_t)(getRoll() * 100) % 100, 2);
    OLED_ShowString(4, 1, "Pitch:");
    OLED_ShowSignedNum(4, 8, getPitch(), 3);
    OLED_ShowNum(4, 13, (uint32_t)(getPitch() * 100) % 100, 2);
}





int main(void) {
    /*模块初始化*/
    OLED_Init();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); // 禁用 JTAG/SWD 调试引脚
	Motor_Init();
	MPU6050_Init();
    Encoder_Init_TIM_All();
    Timer_Init();//定时器初始化
	//Timer8_Init();
	Key_Init();
    Serial_Init();
    

    begin(1000.0f / (float)mpu6050.MPU6050dt);
    //dataGetERROR();


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
				setPIDTarget(&vec_left, 100);
				setPIDTarget(&vec_right, 100);

                OLED_ShowString(1,1,"Speed");//1 Speed
                OLED_ShowSignedNum(2,1,Speed,5);//SpeedNum
                OLED_ShowSignedNum(3,1,(int32_t)ecd_left.counter.count_increment,8);//
                OLED_ShowSignedNum(4,1,(int32_t)ecd_right.counter.count_increment,8);//
//                OLED_ShowNum(4,1,Temp,8);//

                Serial_Printf("Speed:%lld,",ecd_left.counter.count_increment);
				Serial_Printf("%lld\n",ecd_right.counter.count_increment);
                if (KeyNum == ADDKey)
                {
                    Speed += 200;					//速度变量自增20
                    if (Speed > 4000)				//速度变量超过100后
                    {
                        Speed = 4000;				//速度变量变为-100
                        //此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
                        //若出现了此现象，则应避免使用这样的操作
                    }
                }
                if (KeyNum == DECKey)
                {
                    Speed -= 200;					//速度变量自增20
                    if (Speed < -4000)				//速度变量超过100后
                    {
                        Speed = -4000;				//速度变量变为-100
                        //此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
                        //若出现了此现象，则应避免使用这样的操作
                    }
                }


//                Motor_SetSpeedA(Speed);				//设置直流电机的速度为速度变量
//                Motor_SetSpeedB(Speed);
            }



            /*  Test */
            if (KeyNumMenu==2)//目前测试的进程
            {
                OLED_ShowNum(1, 1, millis, 7);
                if(millis - mpu6050.preMillis >= mpu6050.MPU6050dt) {
                    mpu6050.preMillis = millis;
                    dataGetAndFilter();		                            //获取MPU6050的数据
                    updateIMU(msg.G[0], msg.G[1], msg.G[2], msg.A[0], msg.A[1], msg.A[2]);
                }

                EularPrint();
            }







	    }
		


        


    }//while函数的大括号










}//main函数的大括号


void dataGetERROR() {
    for(uint8_t i = 0; i < 100; ++i) {
        getMPU6050Data();
        mpu6050.MPU6050ERROE[0] += msg.A[0];
        mpu6050.MPU6050ERROE[1] += msg.A[1];
        mpu6050.MPU6050ERROE[2] += msg.A[2] - 9.8;
        mpu6050.MPU6050ERROE[3] += msg.G[0];
        mpu6050.MPU6050ERROE[4] += msg.G[1];
        mpu6050.MPU6050ERROE[5] += msg.G[2];
        Delay_ms(10);
    }
    for(uint8_t i = 0; i < 6; ++i) {
        mpu6050.MPU6050ERROE[i] /= 100.0f;
    }
}


void getMPU6050Data() {
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
    msg.A[0] = (float)((float)AX / (float)32768) * 16 * 9.8;
    msg.A[1] = (float)((float)AY / (float)32768) * 16 * 9.8;
    msg.A[2] = (float)((float)AZ / (float)32768) * 16 * 9.8;
    msg.G[0] = (float)((float)GX / (float)32768) * 2000 * 3.5;
    msg.G[1] = (float)((float)GY / (float)32768) * 2000 * 3.5;
    msg.G[2] = (float)((float)GZ / (float)32768) * 2000 * 3.5;

}

void dataGetAndFilter() {
    getMPU6050Data();
    msg.A[0] -= mpu6050.MPU6050ERROE[0];
    msg.A[1] -= mpu6050.MPU6050ERROE[1];
    msg.A[2] -= mpu6050.MPU6050ERROE[2];
    msg.G[0] -= mpu6050.MPU6050ERROE[3];
    msg.G[1] -= mpu6050.MPU6050ERROE[4];
    msg.G[2] -= mpu6050.MPU6050ERROE[5];
}


void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
//        Temp = TIM_GetCounter(TIM2);
//        updateEncoderLoopSimpleVersion(&ecd_left, 10, TIM2);
//        updateEncoderLoopSimpleVersion(&ecd_right, 10, TIM4);

//        updatePID(&pos_left, ecd_left.position.angle);
//        setPIDTarget(&vec_left, pos_left.output);
//        updatePID(&vec_left, ecd_left.velocity.angular);
//
//        updatePID(&pos_right, ecd_right.position.angle);
//        setPIDTarget(&vec_right, pos_right.output);
//        updatePID(&vec_right, ecd_right.velocity.angular);

//        updatePID(&vec_left, ecd_left.counter.count_increment);
//        updatePID(&vec_right, ecd_right.counter.count_increment);


//        Motor_SetSpeedA(vec_left.output);
//        Motor_SetSpeedB(vec_right.output);

        millis++;
		



        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

//速度计时器中断服务函数

//void TIM8_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
//    {
//        millis++;
//        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
//        //中断标志位必须清除
//        //否则中断将连续不断地触发，导致主程序卡死
//    }
//}//定时器2中断服务函数
