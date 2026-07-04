#ifndef BSP_SOIL_H
#define BSP_SOIL_H
#include "hal_data.h"

uint16_t Soil_Read_Raw(void);
uint16_t Soil_Read_Filtered(void);
uint8_t Soil_Get_Humidity(uint16_t adc_val);

#endif