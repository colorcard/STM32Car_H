#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeedA(int16_t Speed);
void Motor_SetSpeedB(int16_t Speed);
void Motor_SetSpeed(int MotorStatus,int16_t Speed);
void motor_target_set(int left,int right);

#endif
