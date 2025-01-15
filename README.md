# STM32的寻迹小车（2024年电赛H题）

### 题目要求：[H题_自动行驶小车.pdf](H%E9%A2%98_%E8%87%AA%E5%8A%A8%E8%A1%8C%E9%A9%B6%E5%B0%8F%E8%BD%A6.pdf)

### 代码组成说明：
- [main.c](User/main.c)：主程序
- [Motor.c](Hardware/Motor.c)：电机控制
- [Encoder.c](Hardware/Encoder.c)：编码器读数
- [pid.c](Hardware/pid.c)：PID控制
- [OLED.c](Hardware/OLED.c)：OLED屏幕显示
- [Timer.c](Hardware/Timer.c)：定时器
- [PWM.c](Hardware/PWM.c)：PWM输出
- [MyI2C.c](HardwareMyI2C.c)：I2C通信
- [Serial.c](Hardware/Serial.c)：串口通讯
- [MPU6050.c](Hardware/MPU6050.c)：MPU6050读数
- [MadgwickAHRS.c](Hardware/MadgwickAHRS.c)：姿态解算
- [Key.c](Hardware/Key.c)：按键
- [gray_track.c](Hardware/gray_track.c)：寻迹
- [Buzzer.c](Hardware/Buzzer.c)：蜂鸣器
