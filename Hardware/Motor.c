#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        // 开启 GPIOA 的时钟

    GPIO_InitTypeDef GPIO_InitStructure;

    // 初始化 AIN1 和 AIN2 (PA11, PA12)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                      // 将 PA11 和 PA12 初始化为推挽输出

    // 初始化 BIN1 和 BIN2 (PA14, PA15)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                      // 将 PA14 和 PA15 初始化为推挽输出

    PWM_Init();                                                 // 初始化直流电机的底层 PWM
}

/**
  * 函    数：直流电机 A 设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeedA(int8_t Speed)
{
    if (Speed >= 0)                         // 如果设置正转的速度值
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_12);   // PA12 置高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_11); // PA11 置低电平，设置方向为正转
        PWM_SetCompare2(Speed);             // PWM 设置为速度值
    }
    else                                    // 否则，即设置反转的速度值
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_12); // PA12 置低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_11);   // PA11 置高电平，设置方向为反转
        PWM_SetCompare2(-Speed);            // PWM 设置为负的速度值，因为此时速度值为负数，而 PWM 只能给正数
    }
}

/**
  * 函    数：直流电机 B 设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeedB(int8_t Speed)
{
    if (Speed >= 0)                         // 如果设置正转的速度值
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_15);   // PA15 置高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_14); // PA14 置低电平，设置方向为正转
        PWM_SetCompare1(Speed);             // PWM 设置为速度值 (PA6 对应 TIM3_CH1)
    }
    else                                    // 否则，即设置反转的速度值
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_15); // PA15 置低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_14);   // PA14 置高电平，设置方向为反转
        PWM_SetCompare1(-Speed);            // PWM 设置为负的速度值，因为此时速度值为负数，而 PWM 只能给正数
    }
}
