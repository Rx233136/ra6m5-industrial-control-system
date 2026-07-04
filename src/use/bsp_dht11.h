#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "hal_data.h"
#include <stdint.h>

#define Bit_RESET   0
#define Bit_SET     1
#define DHT11_LOW   0
#define DHT11_HIGH  1

// RA705 DHT11数据引脚 P705
#define DHT11_PORT  BSP_IO_PORT_07_PIN_05

typedef struct
{
    uint8_t  humi_int;
    uint8_t  humi_deci;
    uint8_t  temp_int;
    uint8_t  temp_deci;
    uint8_t  check_sum;
} DHT11_Data_TypeDef;

void DHT11_Init(void);
void DHT11_Start(void);
void DHT11_DELAY_US(uint32_t delay);
void DHT11_DELAY_MS(uint32_t delay);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);

#endif
