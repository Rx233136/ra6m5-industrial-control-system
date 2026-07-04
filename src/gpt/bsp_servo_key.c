#include "bsp_servo_key.h"
#include "hal_data.h"

void Servo_Key_Init(void)
{
    R_IOPORT_PinCfg(&g_ioport_ctrl, KEY_SERVO_CTRL, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);
}

uint8_t Servo_Key_Scan(void)
{
    bsp_io_level_t key_state;
    R_IOPORT_PinRead(&g_ioport_ctrl, KEY_SERVO_CTRL, &key_state);
    if(key_state == BSP_IO_LEVEL_LOW)
    {
        R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MILLISECONDS);
        R_IOPORT_PinRead(&g_ioport_ctrl, KEY_SERVO_CTRL, &key_state);
        if(key_state == BSP_IO_LEVEL_LOW)
        {
            while(key_state == BSP_IO_LEVEL_LOW)
            {
                R_IOPORT_PinRead(&g_ioport_ctrl, KEY_SERVO_CTRL, &key_state);
            }
            R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MILLISECONDS);
            return 1;
        }
    }
    return 0;
}
