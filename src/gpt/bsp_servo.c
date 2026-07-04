/*
* bsp_servo.c
*
* Created on: 2026年6月27日
* Author: PC
*/
#include "bsp_servo.h"
#include "hal_data.h"

/* AGT0两路舵机初始化函数，对标官方模板 */
void AGT_Servo_Init(void)
{
/* 初始化 AGT0 模块 */
R_AGT_Open(&g_timer_agt0_ctrl, &g_timer_agt0_cfg);
/* 启动 AGT 定时器 */
R_AGT_Start(&g_timer_agt0_ctrl);
/* 上电两路舵机默认中位7.5%（90°） */
AGT_Servo_SetDuty_A(7.5f);
AGT_Servo_SetDuty_B(7.5f);
}

/** 自定义函数：设置A舵机(P500 AGTOA) PWM占空比
@param duty 占空比范围：2.5 ~ 12.5 %（SG90标准）
*/
void AGT_Servo_SetDuty_A(float duty)
{
    timer_info_t info;
    uint32_t current_period_counts;
    uint32_t duty_cycle_counts;
    // SG90限位
    if (duty < 2.5f) duty = 2.5f;
    if (duty > 12.5f) duty = 12.5f;

    /* 获得 AGT0 的信息 */
    R_AGT_InfoGet(&g_timer_agt0_ctrl, &info);
    /* 获得计时器一个周期需要的计数次数 */
    current_period_counts = info.period_counts;
    // 先强转float再运算，彻底消除类型转换警告
    duty_cycle_counts = (uint32_t)((float)current_period_counts * duty / 100.0f);
    /* 设置AGTOA P500 */
    R_AGT_DutyCycleSet(&g_timer_agt0_ctrl, duty_cycle_counts, SERVO_A_PIN);
}

/** 自定义函数：设置B舵机(P501 AGTOB) PWM占空比
@param duty 占空比范围：2.5 ~ 12.5 %（SG90标准）
*/
void AGT_Servo_SetDuty_B(float duty)
{
    timer_info_t info;
    uint32_t current_period_counts;
    uint32_t duty_cycle_counts;
    // SG90限位
    if (duty < 2.5f) duty = 2.5f;
    if (duty > 12.5f) duty = 12.5f;

    /* 获得 AGT0 的信息 */
    R_AGT_InfoGet(&g_timer_agt0_ctrl, &info);
    /* 获得计时器一个周期需要的计数次数 */
    current_period_counts = info.period_counts;
    // 先强转float再运算，彻底消除类型转换警告
    duty_cycle_counts = (uint32_t)((float)current_period_counts * duty / 100.0f);
    /* 设置AGTOB P501 */
    R_AGT_DutyCycleSet(&g_timer_agt0_ctrl, duty_cycle_counts, SERVO_B_PIN);
}
