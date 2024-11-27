#ifndef __ENCODER_H
#define __ENCODER_H

#define ENCODER_TIM_PERIOD (u16)(65535)   //最大限制65535  为F103的定时器16位的
#define LEFT_TIM TIM2
#define RIGHT_TIM TIM4



void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);


// !!!!!此文件仅适用于用STM32 PWM驱动的带有GMR(光电)or霍尔编码器的直流电机!!!!

typedef struct {
    uint8_t  multiple;			// x倍频 可取2 or 4，对应cubemx的配置
    float 	 reduction_ratio;	// 电机减速比，例如最常见的 30
    uint16_t ppr;				// 编码器线数，GMR编码器:500 霍尔编码器:13
    float	 r;					// 轮子半径,例如0.0123 m, 单位 m
}Parameter;

//P.S. 速度基本上不可能越过float的表示范围  光速也就才3*10^8
typedef struct{
    float angular;		//角速度，单位rpm
    float linear;		//线速度，单位m/s
} Velocity;

typedef struct{
    double rotations;	//电机输出轴转动圈数和	无单位
    double distance;	//轮子前进or后退距离	单位:m
    double angle;		//轮子正传or反转角度	单位:°
} Position;

typedef struct{
    uint32_t count_now;			//编码器当前计数
    uint32_t count_last;		//编码器上次计数
    int64_t  count_increment;	//编码器两帧增量计数, must int64 .+表示正转 -表示反方向转
    int64_t  count_total;		//编码器总计数


} Counter;

typedef enum{
    UP = 0,
    DOWN = 1,
    INIT = 2
}Direction;

typedef struct{
    Parameter param;

    Direction direction;

    Velocity velocity;

    Position position;

    Counter counter;
}Encoder;


void initEncoder(Encoder* ecd, const Parameter param);
void updateEncoderLoopSimpleVersion(Encoder* ecd, uint16_t loop_period,u8 TIMx);
void updateEncoderLoopOverflowVersion(Encoder* ecd, uint16_t loop_period,u8 TIMx);



#endif
