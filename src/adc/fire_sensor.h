#ifndef FIRE_SENSOR_H
#define FIRE_SENSOR_H

#include "hal_data.h"

#define FIRE_ADC_CH  ADC_CHANNEL_1

extern volatile bool fire_scan_flag;

void Fire_ADC_Init(void);
uint16_t Fire_Read_Raw(void);

#endif
