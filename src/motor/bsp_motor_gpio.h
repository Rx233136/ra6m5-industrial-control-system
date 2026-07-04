#ifndef BSP_MOTOR_GPIO_H_
#define BSP_MOTOR_GPIO_H_
#include "hal_data.h"

// 电机通道枚举
typedef enum
{
    MOTOR_CH_A,
    MOTOR_CH_B
} motor_ch_t;

// 电机运行模式
typedef enum
{
    MOTOR_STOP,
    MOTOR_FORWARD,
    MOTOR_BACKWARD
} motor_dir_t;

// 初始化TB6612方向控制GPIO P601/P602/P801/P802
void Motor_GPIO_Init(void);
// 设置指定电机转向/停机
void Motor_SetDir(motor_ch_t ch, motor_dir_t dir);

#endif
