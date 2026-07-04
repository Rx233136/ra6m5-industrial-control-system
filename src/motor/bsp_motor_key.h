#ifndef BSP_MOTOR_KEY_H_
#define BSP_MOTOR_KEY_H_
#include "hal_data.h"

// 按键定义
#define KEY_MOTOR_A     BSP_IO_PORT_00_PIN_14
#define KEY_MOTOR_B     BSP_IO_PORT_00_PIN_15

// 按键初始化
void Motor_Key_Init(void);
// 按键扫描，返回0=无按键，1=A键按下，2=B键按下
uint8_t Motor_Key_Scan(void);

#endif
