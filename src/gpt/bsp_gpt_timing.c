/*
* bsp_gpt_timing.c
*
* Created on: 2026年6月24日
* Author: PC
*/
#include "bsp_gpt_timing.h"
#include "hal_data.h"
void GPT_PWM_Init(void)
{
/* 1. 打开GPT6定时器硬件，初始化底层寄存器 */
R_GPT_Open(&g_timer_gpt6_ctrl, &g_timer_gpt6_cfg);
/* 2. 短暂延时等待定时器时钟稳定 */
R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
/* 3. 启动GPT PWM波形输出 */
R_GPT_Start(&g_timer_gpt6_ctrl);
/* 4. 上电舵机默认0°，对应占空比2.5% */
GPT_PWM_SetDuty(2.5f);
}
/** 自定义函数：设置 PWM 占空比（适配MG996R舵机，范围2.5~12.5%）
@param duty 输入占空比百分比
*/
void GPT_PWM_SetDuty(float duty)
{
timer_info_t info;
uint32_t period_cnt, duty_cnt;
// 强制限制舵机有效识别区间
if(duty < 2.5f)
duty = 2.5f;
if(duty > 12.5f)
duty = 12.5f;
/* 获取GPT完整周期计数值 */
R_GPT_InfoGet(&g_timer_gpt6_ctrl, &info);
period_cnt = info.period_counts;
// 修复浮点转换警告：先转float运算，不产生uint64超大中间变量
duty_cnt = (uint32_t)(((float)period_cnt * duty) / 100.0f);
/* 更新GTIOCB(P600)引脚PWM占空比 */
R_GPT_DutyCycleSet(&g_timer_gpt6_ctrl, duty_cnt, GPT_IO_PIN_GTIOCB);
}
