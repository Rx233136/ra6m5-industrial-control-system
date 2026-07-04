#include "bsp_light.h"

volatile bool scan_complete_flag = false;
volatile bool fire_scan_flag = false;

#define LIGHT_FILTER_SIZE 4
static uint16_t light_filter_buf[LIGHT_FILTER_SIZE];
static uint8_t light_filter_idx = 0;
static uint8_t filter_initialized = 0;

void ADC_Init(void)
{
    fsp_err_t err;
    err = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    err = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
    assert(FSP_SUCCESS == err);
    
    filter_initialized = 0;
    light_filter_idx = 0;
}

void adc_callback(adc_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    scan_complete_flag = true;
    fire_scan_flag = true;
}

uint16_t Read_ADC_Light_Raw(void)
{
    uint16_t adc_data = 0;
    scan_complete_flag = false;
    R_ADC_ScanStart(&g_adc0_ctrl);
    while(!scan_complete_flag);
    R_ADC_Read(&g_adc0_ctrl, LIGHT_CH, &adc_data);
    return adc_data;
}

uint16_t Read_ADC_Light_Filtered(void)
{
    uint16_t raw_val = Read_ADC_Light_Raw();
    
    // 首次调用时，用第一个采样值填充整个缓冲区
    if(filter_initialized == 0)
    {
        for(uint8_t i = 0; i < LIGHT_FILTER_SIZE; i++)
        {
            light_filter_buf[i] = raw_val;
        }
        filter_initialized = 1;
        return raw_val;
    }
    
    light_filter_buf[light_filter_idx] = raw_val;
    light_filter_idx = (light_filter_idx + 1) % LIGHT_FILTER_SIZE;
    
    uint32_t sum = 0;
    for(uint8_t i = 0; i < LIGHT_FILTER_SIZE; i++)
    {
        sum += light_filter_buf[i];
    }
    
    return (uint16_t)(sum / LIGHT_FILTER_SIZE);
}

float Read_ADC_Voltage(void)
{
    uint16_t adc_data = Read_ADC_Light_Raw();
    return (float)adc_data * 3.3f / 4095.0f;
}

uint8_t Light_Get_Brightness(uint16_t adc_val)
{
    // ADC范围0-4095，转换为百分比0-100
    // 光敏传感器：有光时ADC值大，遮光时ADC值小
    // 所以亮度百分比 = ADC值/4095 * 100
    if(adc_val > 4095) adc_val = 4095;
    uint32_t bright = (uint32_t)adc_val * 100U / 4095U;
    return (uint8_t)bright;
}