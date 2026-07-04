#include "bsp_dht11.h"

// 输出模式：推挽输出
static void DHT11_OUT_MODE(void)
{
    R_IOPORT_PinCfg(&g_ioport_ctrl, DHT11_PORT, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
}

// 输入模式 + 内部上拉
static void DHT11_IN_MODE(void)
{
    R_IOPORT_PinCfg(&g_ioport_ctrl, DHT11_PORT, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);
}

// 输出高低电平，自动切输出
#define DHT_HIGH    do{DHT11_OUT_MODE();R_BSP_PinWrite(DHT11_PORT, BSP_IO_LEVEL_HIGH);}while(0)
#define DHT_LOW     do{DHT11_OUT_MODE();R_BSP_PinWrite(DHT11_PORT, BSP_IO_LEVEL_LOW);}while(0)

// 读取引脚电平，自动切带上拉输入
#define Read_Data   ({ \
    DHT11_IN_MODE(); \
    bsp_io_level_t lv; \
    R_IOPORT_PinRead(&g_ioport_ctrl, DHT11_PORT, &lv); \
    (lv == BSP_IO_LEVEL_HIGH) ? Bit_SET : Bit_RESET; \
})

static uint8_t Read_Byte(void);

void DHT11_Init(void)
{
    R_IOPORT_Open (&g_ioport_ctrl, g_ioport.p_cfg);
    DHT_HIGH;
}

void DHT11_DELAY_US(uint32_t delay)
{
    R_BSP_SoftwareDelay(delay, BSP_DELAY_UNITS_MICROSECONDS);
}

void DHT11_DELAY_MS(uint32_t delay)
{
    R_BSP_SoftwareDelay(delay, BSP_DELAY_UNITS_MILLISECONDS);
}

void DHT11_Start(void)
{
    DHT_HIGH;
    DHT11_DELAY_US(30);
    DHT_LOW;
    DHT11_DELAY_MS(20);
    DHT_HIGH;
    DHT11_DELAY_US(30);
}

static uint8_t Read_Byte(void)
{
    uint8_t i, temp = 0;
    for(i = 0; i < 8; i++)
    {
        while(Read_Data == Bit_RESET);
        DHT11_DELAY_US(40);
        if(Read_Data == Bit_SET)
        {
            while(Read_Data == Bit_SET);
            temp |= (uint8_t)(0x01 << (7 - i));
        }
        else
        {
            temp &= (uint8_t)~(0x01 << (7 - i));
        }
    }
    return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{
    uint16_t count;
    DHT11_Start();

    if(Read_Data == Bit_RESET)
    {
        count = 0;
        while(Read_Data == Bit_RESET)
        {
            count++;
            if(count > 1000)
                return 0;
            DHT11_DELAY_US(10);
        }
        count = 0;
        while(Read_Data == Bit_SET)
        {
            count++;
            if(count > 1000)
                return 0;
            DHT11_DELAY_US(10);
        }

        DHT11_Data->humi_int  = Read_Byte();
        DHT11_Data->humi_deci = Read_Byte();
        DHT11_Data->temp_int  = Read_Byte();
        DHT11_Data->temp_deci = Read_Byte();
        DHT11_Data->check_sum = Read_Byte();

        DHT_HIGH;
        DHT11_DELAY_US(55);

        if(DHT11_Data->check_sum == (DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci))
            return 1;
        else
            return 0;
    }
    return 0;
}
