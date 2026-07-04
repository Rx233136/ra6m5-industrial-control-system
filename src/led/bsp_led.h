#ifndef BSP_LED_H_
#define BSP_LED_H_

#include "hal_data.h"

#define LED_PIN     BSP_IO_PORT_04_PIN_05
// 阳极接IO，高电平亮、低电平灭，修正定义
#define LED_ON      BSP_IO_LEVEL_HIGH
#define LED_OFF     BSP_IO_LEVEL_LOW

// 函数声明
void BSP_LED_Init(void);
void BSP_LED_Toggle(void);
void BSP_LED_Set(bsp_io_level_t level);

#endif
