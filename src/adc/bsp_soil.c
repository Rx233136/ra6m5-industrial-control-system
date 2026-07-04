#include "bsp_soil.h"
#include "adc/bsp_light.h"

extern volatile bool scan_complete_flag;

#define SOIL_FILTER_SIZE 16
static uint16_t soil_filter_buf[SOIL_FILTER_SIZE];
static uint8_t soil_filter_idx = 0;
static uint8_t soil_filter_initialized = 0;

uint16_t Soil_Read_Raw(void)
{
    uint16_t adc_data = 0;
    scan_complete_flag = false;
    R_ADC_ScanStart(&g_adc0_ctrl);
    while(!scan_complete_flag);
    R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_3, &adc_data);
    return adc_data;
}

uint16_t Soil_Read_Filtered(void)
{
    uint16_t raw_val = Soil_Read_Raw();
    
    // 首次调用时，用第一个采样值填充整个缓冲区
    if(soil_filter_initialized == 0)
    {
        for(uint8_t i = 0; i < SOIL_FILTER_SIZE; i++)
        {
            soil_filter_buf[i] = raw_val;
        }
        soil_filter_initialized = 1;
        return raw_val;
    }
    
    soil_filter_buf[soil_filter_idx] = raw_val;
    soil_filter_idx = (soil_filter_idx + 1) % SOIL_FILTER_SIZE;
    
    uint32_t sum = 0;
    for(uint8_t i = 0; i < SOIL_FILTER_SIZE; i++)
    {
        sum += soil_filter_buf[i];
    }
    
    return (uint16_t)(sum / SOIL_FILTER_SIZE);
}

uint8_t Soil_Get_Humidity(uint16_t adc_val)
{
    if(adc_val > 4095) adc_val = 4095;
    uint32_t hum = (uint32_t)(4095U - adc_val) * 100U / 4095U;
    return (uint8_t)hum;
}