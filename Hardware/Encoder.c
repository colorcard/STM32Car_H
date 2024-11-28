#include <limits.h>
#include "stm32f10x.h"                  // Device header
#include "Encoder.h"


#define PI 3.14159265358979323846


volatile int32_t overflow_count_TIM2 = 0; // 全局变量记录溢出次数（TIM2）
volatile int32_t overflow_count_TIM4 = 0; // 全局变量记录溢出次数（TIM4）


/**************************************************************************
Function: Initialize TIM2 to encoder interface mode
Input   : none
Output  : none
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/

void Encoder_Init_TIM_All(void)
{
    Encoder_Init_TIM2();
    Encoder_Init_TIM4();
}

void Encoder_Init_TIM5(void) // Motor A
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 时钟使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);       // 使能 TIM5 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);      // 使能 GPIOA 时钟

    /* GPIO 配置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;     // 配置 PA0 和 PA1 为 TIM5 的通道 1 和通道 2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             // 设置为上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                    // 初始化 GPIOA

    /* TIM 基础配置 */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;              // 预分频器，时钟不分频
    TIM_TimeBaseStructure.TIM_Period = 65536 - 1;             // 自动重装值，最大计数值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   // 不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);           // 初始化 TIM5 的基础设置

    /* 编码器接口配置 */
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);       // 编码器模式 3，双通道上升沿捕获

    /* 输入捕获配置 */
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                   // 滤波器设置为最大值（15）
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    /* 中断配置 */
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);                     // 清除更新标志位
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);                // 开启更新中断

    /* 计数器复位并使能 */
    TIM_SetCounter(TIM5, 0);                                  // 复位计数器
    TIM_Cmd(TIM5, ENABLE);                                    // 使能 TIM5
}



/**************************************************************************
Function: Initialize TIM4 to encoder interface mode
Input   : none
Output  : none
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)//Motor B
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器
    TIM_TimeBaseStructure.TIM_Period = 65536 - 1; //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    //Reset counter
    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4, ENABLE);
}





// 获取定时器方向的函数
uint8_t Get_TIM_Direction(TIM_TypeDef* TIMx) {
    // 检查 TIMx->CR1 的 DIR 位
    return (TIMx->CR1 & TIM_CR1_DIR) ? 1 : 0;  // 返回 1 表示向下计数，0 表示向上计数
}

/**************************************************************************
Function: Read encoder count per unit time
Input   : TIMX：Timer
Output  : none
函数功能：单位时间读取编码器计数
入口参数：TIMX：定时器
返回  值：速度值
**************************************************************************/
int16_t Encoder_Get(TIM_TypeDef* TIMx)
{
    /*使用Temp变量作为中继，目的是返回CNT后将其清零*/
    int16_t Temp;
    Temp = TIM_GetCounter(TIMx);
    TIM_SetCounter(TIMx, 0);
    return Temp;
}






/**************************************************************************
Function: TIM4、TIM2 interrupt service function
Input   : none
Output  : none
函数功能：TIM4、TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM5_IRQHandler(void)
{
    if (TIM5->SR & TIM_SR_UIF) // 检测更新中断标志
    {
        overflow_count_TIM2++;     // 增加溢出次数
        TIM5->SR &= ~TIM_SR_UIF;   // 清除中断标志位
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM4->SR & TIM_SR_UIF) // 检测更新中断标志
    {
        overflow_count_TIM4++;     // 增加溢出次数
        TIM4->SR &= ~TIM_SR_UIF;   // 清除中断标志位
    }
}





void initEncoder(Encoder* ecd, const Parameter param)
{
    ecd->param = param;

    // 初始化速度
    ecd->velocity.angular = 0;
    ecd->velocity.linear = 0;

    // 初始化位置
    ecd->position.rotations = 0;
    ecd->position.distance = 0;
    ecd->position.angle = 0;

    // 初始化计数器
    ecd->counter.count_now = 0;
    ecd->counter.count_last = 0;
    ecd->counter.count_increment = 0;
    ecd->counter.count_total = 0;

    ecd->direction = INIT;
}




void updateEncoderLoopSimpleVersion(Encoder* ecd, uint16_t loop_period,TIM_TypeDef *TIMx){

    //-----counter
    // counter_now
    ecd->counter.count_now = Encoder_Get(TIMx);

    // counter_increment
    ecd->counter.count_increment = (int64_t)((int16_t)(ecd->counter.count_now - 0));

    // counter_total
    ecd->counter.count_total += ecd->counter.count_increment;

    //-----position
    ecd->position.rotations = (double)ecd->counter.count_total / ((double)ecd->param.multiple * ecd->param.reduction_ratio * (double)ecd->param.ppr);
    ecd->position.angle = ecd->position.rotations * 360.0;
    ecd->position.distance = ecd->position.rotations / 60.0 * PI * 2.0 * ecd->param.r;

    //-----velocity
    ecd->velocity.angular = (float)(ecd->counter.count_increment/((float)ecd->param.multiple * ecd->param.reduction_ratio * (float)ecd->param.ppr) * 1000.0 / loop_period * 60);
    ecd->velocity.linear = (float)(ecd->velocity.angular/60.0 * PI * 2.0 * ecd->param.r);
}
