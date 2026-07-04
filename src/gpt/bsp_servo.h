#ifndef BSP_SERVO_H_
#define BSP_SERVO_H_
#include "hal_data.h"

// AGT官方通道宏
#define SERVO_A_PIN     AGT_OUTPUT_PIN_AGTOA
#define SERVO_B_PIN     AGT_OUTPUT_PIN_AGTOB

// AGT0两路舵机初始化
void AGT_Servo_Init(void);
// 设置A舵机占空比 SG90范围2.5~12.5
void AGT_Servo_SetDuty_A(float duty);
// 设置B舵机占空比 SG90范围2.5~12.5
void AGT_Servo_SetDuty_B(float duty);

#endif
