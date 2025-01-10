#include "stm32f10x.h"                  // Device header
#include "Timer.h"


void Timer_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_Period=1000-1;
	TimeBaseInitStructure.TIM_Prescaler=7200-1;
	TimeBaseInitStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM1,&TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM1,ENABLE);

}

void Timer8_Init(void) {
    // 使能 TIM8 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    // 配置 TIM8 使用内部时钟
    TIM_InternalClockConfig(TIM8);

    // 初始化定时器基础配置
    TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
    TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频系数：无分频
    TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TimeBaseInitStructure.TIM_Period = 10 - 1;                // 自动重装载值（ARR），计数到 999 产生中断
    TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;             // 预分频器（PSC），输入时钟分频为 7200
    TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器，未使用
    TIM_TimeBaseInit(TIM8, &TimeBaseInitStructure);

    // 清除更新中断标志
    TIM_ClearFlag(TIM8, TIM_FLAG_Update);

    // 使能更新中断
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

    // 配置 NVIC 优先级组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 配置 NVIC 初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;             // 定时器 8 更新中断
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                // 启用中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;             // 响应优先级
    NVIC_Init(&NVIC_InitStructure);

    // 使能定时器
    TIM_Cmd(TIM8, ENABLE);
}



