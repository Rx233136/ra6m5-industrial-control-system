#include "fire_sensor.h"
#include <assert.h>


void Fire_ADC_Init(void)
{
    fsp_err_t err;
    err = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    assert(FSP_SUCCESS == err);
    err = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
    assert(FSP_SUCCESS == err);
}

uint16_t Fire_Read_Raw(void)
{
    uint16_t adc_data = 0;
    fire_scan_flag = false;
    R_ADC_ScanStart(&g_adc0_ctrl);
    while (!fire_scan_flag);
    fire_scan_flag = false;
    R_ADC_Read(&g_adc0_ctrl, FIRE_ADC_CH, &adc_data);
    return adc_data;
}
