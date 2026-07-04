#ifndef BSP_MOTOR_PWM_H_
#define BSP_MOTOR_PWM_H_
#include "hal_data.h"

// 初始化GPT8两路电机PWM（P605 GTIOCA / P604 GTIOCB）
void Motor_PWM_Init(void);
// 设置A通道PWM占空比 0~100浮点
void Motor_PWM_SetDuty_A(float duty);
// 设置B通道PWM占空比 0~100浮点
void Motor_PWM_SetDuty_B(float duty);

#endif
