/*
* bsp_motor_pwm.c
*
* Created on: 2026年6月27日
* Author: PC
*/
#include "bsp_motor.h"
#include "hal_data.h"

void Motor_PWM_Init(void)
{
/* 1. 打开GPT8定时器硬件，初始化底层寄存器 */
R_GPT_Open(&g_timer_gpt8_ctrl, &g_timer_gpt8_cfg);
/* 2. 短暂延时等待定时器时钟稳定 */
R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
/* 3. 启动GPT PWM波形输出 */
R_GPT_Start(&g_timer_gpt8_ctrl);
/* 4. 上电电机默认80%占空比 */
Motor_PWM_SetDuty_A(80.0f);
Motor_PWM_SetDuty_B(80.0f);
}

/** 自定义函数：设置 A通道(P605 GTIOCA) PWM占空比，范围0~100%
@param duty 输入占空比百分比
*/
void Motor_PWM_SetDuty_A(float duty)
{
timer_info_t info;
uint32_t period_cnt, duty_cnt;
// 强制限制电机占空比有效区间 0~100
if(duty < 0.0f)
duty = 0.0f;
if(duty > 100.0f)
duty = 100.0f;

/* 获取GPT完整周期计数值 */
R_GPT_InfoGet(&g_timer_gpt8_ctrl, &info);
period_cnt = info.period_counts;
// 浮点运算计算匹配计数，和舵机代码运算逻辑保持一致
duty_cnt = (uint32_t)(((float)period_cnt * duty) / 100.0f);
/* 更新GTIOCA(P605)引脚PWM占空比 */
R_GPT_DutyCycleSet(&g_timer_gpt8_ctrl, duty_cnt, GPT_IO_PIN_GTIOCA);
}

/** 自定义函数：设置 B通道(P604 GTIOCB) PWM占空比，范围0~100%
@param duty 输入占空比百分比
*/
void Motor_PWM_SetDuty_B(float duty)
{
timer_info_t info;
uint32_t period_cnt, duty_cnt;
// 强制限制电机占空比有效区间 0~100
if(duty < 0.0f)
duty = 0.0f;
if(duty > 100.0f)
duty = 100.0f;

/* 获取GPT完整周期计数值 */
R_GPT_InfoGet(&g_timer_gpt8_ctrl, &info);
period_cnt = info.period_counts;
// 浮点运算计算匹配计数，和舵机代码运算逻辑保持一致
duty_cnt = (uint32_t)(((float)period_cnt * duty) / 100.0f);
/* 更新GTIOCB(P604)引脚PWM占空比 */
R_GPT_DutyCycleSet(&g_timer_gpt8_ctrl, duty_cnt, GPT_IO_PIN_GTIOCB);
}
