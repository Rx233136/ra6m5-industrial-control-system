#include "bsp_smoke.h"
#include "adc/bsp_light.h"

extern volatile bool scan_complete_flag;

#define SMOKE_FILTER_SIZE 16
static uint16_t smoke_filter_buf[SMOKE_FILTER_SIZE];
static uint8_t smoke_filter_idx = 0;
static uint8_t smoke_filter_initialized = 0;

uint16_t Smoke_Read_Raw(void)
{
    uint16_t adc_data = 0;
    scan_complete_flag = false;
    R_ADC_ScanStart(&g_adc0_ctrl);
    while(!scan_complete_flag);
    R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_2, &adc_data);
    return adc_data;
}

uint16_t Smoke_Read_Filtered(void)
{
    uint16_t raw_val = Smoke_Read_Raw();
    
    if(smoke_filter_initialized == 0)
    {
        for(uint8_t i = 0; i < SMOKE_FILTER_SIZE; i++)
        {
            smoke_filter_buf[i] = raw_val;
        }
        smoke_filter_initialized = 1;
        return raw_val;
    }
    
    smoke_filter_buf[smoke_filter_idx] = raw_val;
    smoke_filter_idx = (smoke_filter_idx + 1) % SMOKE_FILTER_SIZE;
    
    uint32_t sum = 0;
    for(uint8_t i = 0; i < SMOKE_FILTER_SIZE; i++)
    {
        sum += smoke_filter_buf[i];
    }
    
    return (uint16_t)(sum / SMOKE_FILTER_SIZE);
}

uint8_t Smoke_Get_Density(uint16_t adc_val)
{
    if(adc_val > 4095) adc_val = 4095;
    uint32_t density = (uint32_t)adc_val * 100U / 4095U;
    return (uint8_t)density;
}