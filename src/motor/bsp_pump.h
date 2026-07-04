#ifndef BSP_PUMP_H
#define BSP_PUMP_H
#include "hal_data.h"

// 水泵状态
typedef enum
{
    PUMP_STOP,
    PUMP_RUN
} pump_state_t;

// 水泵硬件初始化 IO+GPT7 PWM
void Pump_Init(void);
// 设置水泵启停+功率 0~100
void Pump_Ctrl(pump_state_t sta, float duty);

#endif
