/*
* bsp_motor_gpio.c
*
* Created on: 2026年6月27日
* Author: PC
*/
#include "bsp_motor_gpio.h"
#include "hal_data.h"

void Motor_GPIO_Init(void)
{
    // P601 AIN1 / P602 AIN2
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_02, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    // P801 BIN1 / P802 BIN2
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_01, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_02, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

    // 上电全部停机，输出低电平
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_02, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_01, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_02, BSP_IO_LEVEL_LOW);
}

void Motor_SetDir(motor_ch_t ch, motor_dir_t dir)
{
    if(MOTOR_CH_A == ch)
    {
        switch(dir)
        {
            case MOTOR_FORWARD:
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, BSP_IO_LEVEL_HIGH);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_02, BSP_IO_LEVEL_LOW);
                break;
            case MOTOR_BACKWARD:
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, BSP_IO_LEVEL_LOW);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_02, BSP_IO_LEVEL_HIGH);
                break;
            case MOTOR_STOP:
            default:
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, BSP_IO_LEVEL_LOW);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_02, BSP_IO_LEVEL_LOW);
                break;
        }
    }
    else if(MOTOR_CH_B == ch)
    {
        switch(dir)
        {
            case MOTOR_FORWARD:
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_01, BSP_IO_LEVEL_HIGH);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_02, BSP_IO_LEVEL_LOW);
                break;
            case MOTOR_BACKWARD:
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_01, BSP_IO_LEVEL_LOW);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_02, BSP_IO_LEVEL_HIGH);
                break;
            case MOTOR_STOP:
            default:
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_01, BSP_IO_LEVEL_LOW);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_08_PIN_02, BSP_IO_LEVEL_LOW);
                break;
        }
    }
}
