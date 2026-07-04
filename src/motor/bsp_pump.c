#include "bsp_pump.h"

void Pump_Init(void)
{
    // AIN1 P612 / AIN2 P615
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_15, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

    // 上电停机
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_15, BSP_IO_LEVEL_LOW);

    // GPT7 PWM P603
    R_GPT_Open(&g_timer_gpt7_ctrl, &g_timer_gpt7_cfg);
    R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MILLISECONDS);
    R_GPT_Start(&g_timer_gpt7_ctrl);
}

void Pump_Ctrl(pump_state_t sta, float duty)
{
    timer_info_t info;
    uint32_t period, duty_cnt;

    // 限幅 0~100
    if(duty < 0) duty = 0;
    if(duty > 100) duty = 100;

    R_GPT_InfoGet(&g_timer_gpt7_ctrl, &info);
    period = info.period_counts;
    duty_cnt = (uint32_t)((float)period * duty / 100.0f);

    if(sta == PUMP_RUN)
    {
        // 正向
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_HIGH);
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_15, BSP_IO_LEVEL_LOW);
        R_GPT_DutyCycleSet(&g_timer_gpt7_ctrl, duty_cnt, GPT_IO_PIN_GTIOCA);
    }
    else
    {
        // 停机
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_LOW);
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_15, BSP_IO_LEVEL_LOW);
        R_GPT_DutyCycleSet(&g_timer_gpt7_ctrl, 0, GPT_IO_PIN_GTIOCA);
    }
}
