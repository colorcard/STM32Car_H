#include "stm32f10x.h"
#include "Motor.h"
#include "gray_track.h"

// 简化 GPIO 初始化的封装函数
void gpio_init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode) {
    // 确保对应的外设时钟已启用
    if (GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	 //使能GPIOB时钟和复用功能（要先打开复用才能修改复用功能）
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	
    } else if (GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } else if (GPIOx == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    }

    // 配置 GPIO 引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

// 灰度传感器初始化
void gray_init() {
    gpio_init(GPIOA, GPIO_Pin_5, GPIO_Mode_IPU);  // D1
    gpio_init(GPIOA, GPIO_Pin_4, GPIO_Mode_IPU);  // D2
    //gpio_init(GPIOB, GPIO_Pin_3, GPIO_Mode_IPU);  // D3
    gpio_init(GPIOA, GPIO_Pin_8, GPIO_Mode_IPU);  // D4
    gpio_init(GPIOC, GPIO_Pin_5, GPIO_Mode_IPU);   // D5
    gpio_init(GPIOC, GPIO_Pin_4, GPIO_Mode_IPU);  // D6
    //gpio_init(GPIOB, GPIO_Pin_1, GPIO_Mode_IPU);  // D7
    //gpio_init(GPIOB, GPIO_Pin_0, GPIO_Mode_IPU);  // D8
}

void track()                        //1234 5678
{
		if((D4 == 0)&&(D5 == 0))        //1110 0111
		{
			motor_target_set(100,100);
		}
		else if((D4 == 0)&&(D5 != 0))   //1110 1111
		{
			motor_target_set(100,120);
		}
		else if((D4 != 0)&&(D5 == 0))   //1111 0111
		{
			motor_target_set(120,100);  
		}
		else if((D3 != 0)&&(D4 == 0))   //1100 1111
		{
			motor_target_set(90,130);
		}
		else if((D5 == 0)&&(D6 == 0))   //1111 0011
		{
			motor_target_set(130,90);
		}
		else if((D3 == 0)&&(D4 != 0))   //1101 1111
		{
			motor_target_set(90,130);
		}
		else if((D5 != 0)&&(D6 == 0))   //1111 1011
		{
			motor_target_set(130,90);
		}
		else if((D2 == 0)&&(D3 == 0))   //1001 1111
		{
			motor_target_set(80,150);
		}
		else if((D6 == 0)&&(D7 == 0))   //1111 1001
		{
			motor_target_set(150,80);
		}
		else if((D2 == 0)&&(D3 != 0))   //1011 1111
		{
			motor_target_set(80,150);
		}
		else if((D6 != 0)&&(D7 == 0))   //1111 1101
		{
			motor_target_set(150,80);
		}
		else if((D1 == 0)&&(D2 == 0))   //0011 1111
		{
			motor_target_set(60,180);
		}
		else if((D7 == 0)&&(D8 == 0))   //1111 1100
		{
			motor_target_set(180,60);
		}
		else if((D1 == 0)&&(D2 != 0))   //0111 1111
		{
			motor_target_set(40,180);
		}	
		else if((D7 !=0)&&(D8 == 0))    //1111 1110
		{
			motor_target_set(180,40);
		}
		else                            //1111 1111
		{
			motor_target_set(100,100);
		}
}

unsigned char gpio_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    // 判断引脚电平是否为高电平
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_SET) {
        return 1; // 高电平
    } else {
        return 0; // 低电平
    }
}

unsigned char digtal(unsigned char channel) // 1-8 获取对应通道的数值
{
    // 通道和对应的 GPIO 配置
    GPIO_TypeDef* gpio_ports[8] = {
            GPIOB,  // D8
            GPIOB,  // D7
            GPIOC,  // D6
            GPIOC,  // D5
            GPIOA,  // D4
            GPIOB,  // D3
            GPIOA,  // D2
            GPIOA   // D1
    };

    uint16_t gpio_pins[8] = {
            GPIO_Pin_0,  // D8
            GPIO_Pin_1,  // D7
            GPIO_Pin_4,  // D6
            GPIO_Pin_5,  // D5
            GPIO_Pin_8,  // D4
            GPIO_Pin_3,  // D3
            GPIO_Pin_4,  // D2
            GPIO_Pin_5   // D1
    };

    // 检查通道是否有效
    if (channel < 1 || channel > 8) {
        return 0; // 无效通道返回 0
    }

    // 调用 gpio_get 获取指定引脚的电平状态
    return gpio_get(gpio_ports[channel - 1], gpio_pins[channel - 1]);
}

