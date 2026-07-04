#include "bsp_motor_key.h"
#include "hal_data.h"

static uint8_t key_a_pressed = 0;
static uint8_t key_b_pressed = 0;

void Motor_Key_Init(void)
{
    R_IOPORT_PinCfg(&g_ioport_ctrl, KEY_MOTOR_A, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);
    R_IOPORT_PinCfg(&g_ioport_ctrl, KEY_MOTOR_B, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);
}

uint8_t Motor_Key_Scan(void)
{
    bsp_io_level_t key_a, key_b;
    R_IOPORT_PinRead(&g_ioport_ctrl, KEY_MOTOR_A, &key_a);
    R_IOPORT_PinRead(&g_ioport_ctrl, KEY_MOTOR_B, &key_b);

    if(key_a == BSP_IO_LEVEL_LOW && key_a_pressed == 0)
    {
        key_a_pressed = 1;
        return 1;
    }
    else if(key_a == BSP_IO_LEVEL_HIGH)
    {
        key_a_pressed = 0;
    }

    if(key_b == BSP_IO_LEVEL_LOW && key_b_pressed == 0)
    {
        key_b_pressed = 1;
        return 2;
    }
    else if(key_b == BSP_IO_LEVEL_HIGH)
    {
        key_b_pressed = 0;
    }

    return 0;
}
