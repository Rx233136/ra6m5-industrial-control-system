#ifndef BSP_MIST_H
#define BSP_MIST_H
#include "hal_data.h"

// 雾化状态
typedef enum
{
    MIST_OFF,
    MIST_ON
} mist_state_t;

// 雾化模块初始化
void Mist_Init(void);
// 设置雾化开关状态
void Mist_Ctrl(mist_state_t sta);

#endif
