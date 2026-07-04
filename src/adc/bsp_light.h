#ifndef __BSP_LIGHT_H
#define __BSP_LIGHT_H

#include "hal_data.h"

#define LIGHT_CH  ADC_CHANNEL_10
extern volatile bool scan_complete_flag;

void ADC_Init(void);
float Read_ADC_Voltage(void);
uint8_t Light_Get_Brightness(uint16_t adc_val);
void adc_callback(adc_callback_args_t *p_args);
uint16_t Read_ADC_Light_Raw(void);
uint16_t Read_ADC_Light_Filtered(void);

#endif
