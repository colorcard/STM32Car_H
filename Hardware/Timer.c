#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Encoder.h"
#include "pid.h"



void Timer_Init(void)
{
    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);       // 开启 TIM5 的时钟

    /* 配置时钟源 */
    TIM_InternalClockConfig(TIM5);                            // 选择 TIM5 为内部时钟

    /* 时基单元初始化 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;         // 定义结构体变量
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频，选择不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;         // 计数周期，即 ARR 的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;       // 预分频器，即 PSC 的值
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);       // 将结构体变量交给 TIM_TimeBaseInit，配置 TIM5 的时基单元

    /* 中断输出配置 */
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);                     // 清除定时器更新标志位

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);                // 开启 TIM5 的更新中断

    /* NVIC 中断分组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           // 配置 NVIC 为分组2

    /* NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStructure;                      // 定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;           // 选择配置 NVIC 的 TIM5 中断线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 指定 NVIC 线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 指定 NVIC 线路的抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 指定 NVIC 线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure);                           // 将结构体变量交给 NVIC_Init，配置 NVIC 外设

    /* TIM 使能 */
    TIM_Cmd(TIM5, ENABLE);                                    // 使能 TIM5，定时器开始运行
}


