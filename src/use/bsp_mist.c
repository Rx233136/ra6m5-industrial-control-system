#include "bsp_mist.h"

void Mist_Init(void)
{
    // FSP已配置P512为输出，此处仅上电默认关闭雾化
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_LOW);
}

void Mist_Ctrl(mist_state_t sta)
{
    if(sta == MIST_ON)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_HIGH);
    }
    else
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_LOW);
    }
}
