#include "stm32f10x.h" // Device header

/**
  * 函    数：PWM 初始化
  * 参    数：无
  * 返 回 值：无
  */
void PWM_Init(void)
{
    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启 TIM3 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启 GPIOA 的时钟

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置 PA7 (TIM3_CH2)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;       // 配置 PA7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化 PA7

    // 配置 PA6 (TIM3_CH1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       // 配置 PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化 PA6

    /* 配置时钟源 */
    TIM_InternalClockConfig(TIM3); // TIM3 使用内部时钟

    /* 时基单元初始化 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                 // ARR 值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;               // 预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 高级定时器使用
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             // 初始化 TIM3

    /* 输出比较初始化 (PA7 - TIM3_CH2) */
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure); // 初始化结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM 模式 1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性为高
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 0; // CCR 初始值
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); // 配置 TIM3 的通道 2 (PA7)

    /* 输出比较初始化 (PA6 - TIM3_CH1) */
    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // 配置 TIM3 的通道 1 (PA6)

    /* TIM 使能 */
    TIM_Cmd(TIM3, ENABLE); // 使能 TIM3
}

/**
  * 函    数：PWM 设置 CCR2 (PA7 - TIM3_CH2)
  * 参    数：Compare 要写入的 CCR 值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR 和 ARR 共同决定占空比，此函数仅设置 CCR 的值，并不直接是占空比
  *           占空比 Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM3, Compare); // 设置 CCR2 的值 (TIM3_CH2)
}

/**
  * 函    数：PWM 设置 CCR1 (PA6 - TIM3_CH1)
  * 参    数：Compare 要写入的 CCR 值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR 和 ARR 共同决定占空比，此函数仅设置 CCR 的值，并不直接是占空比
  *           占空比 Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM3, Compare); // 设置 CCR1 的值 (TIM3_CH1)
}
