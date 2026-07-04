#ifndef BSP_SMOKE_H
#define BSP_SMOKE_H
#include "hal_data.h"

uint16_t Smoke_Read_Raw(void);
uint16_t Smoke_Read_Filtered(void);
uint8_t Smoke_Get_Density(uint16_t adc_val);

#endif