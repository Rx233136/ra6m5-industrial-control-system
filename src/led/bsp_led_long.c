#include "led/bsp_led_long.h"

ws2812_rgb_t ws2812_buf[WS2812_NUM] = {0};

#define WS2812_PORT     R_PORT5
#define WS2812_MASK     ((uint16_t)1U << 11U)

static inline void WS_HIGH(void)
{
    WS2812_PORT->POSR = WS2812_MASK;
}
static inline void WS_LOW(void)
{
    WS2812_PORT->PORR = WS2812_MASK;
}


static void WS2812_SendBit(uint8_t bit)
{
    if(bit)
    {
        WS_HIGH();
        for(uint8_t i = 0; i < 14; i++) __NOP();
        WS_LOW();
        for(uint8_t i = 0; i < 24; i++) __NOP();
    }
    else
    {
        WS_HIGH();
        for(uint8_t i = 0; i < 24; i++) __NOP();
        WS_LOW();
        for(uint8_t i = 0; i < 14; i++) __NOP();
    }
}

static void WS2812_SendByte(uint8_t dat)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        WS2812_SendBit((uint8_t)(dat & (0x80 >> i)));
    }
}

void BSP_WS2812_Init(void)
{
    // 第一步：立即强制拉低引脚（不管之前什么状态）
    WS_LOW();
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);  // 复位1ms确保可靠
    
    // 清零缓冲区
    for(uint8_t i = 0; i < WS2812_NUM; i++)
    {
        ws2812_buf[i].R = 0;
        ws2812_buf[i].G = 0;
        ws2812_buf[i].B = 0;
    }
    
    // 发送清零数据
    BSP_WS2812_Refresh();
    
    // 再次复位确保关闭
    WS_LOW();
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);  // 复位1ms确保可靠
}

void BSP_WS2812_Refresh(void)
{
    uint32_t irq_save = __get_PRIMASK();
    __disable_irq();

    for(uint8_t i=0;i<WS2812_NUM;i++)
    {
        WS2812_SendByte(ws2812_buf[i].G);
        WS2812_SendByte(ws2812_buf[i].R);
        WS2812_SendByte(ws2812_buf[i].B);
    }

    if(irq_save == 0)
        __enable_irq();
    // 发送完毕后保持低电平
    WS_LOW();
}

void BSP_WS2812_SetColor(uint8_t index, uint8_t R, uint8_t G, uint8_t B)
{
    if(index >= WS2812_NUM) return;
    ws2812_buf[index].R = R;
    ws2812_buf[index].G = G;
    ws2812_buf[index].B = B;
}

void BSP_WS2812_ClearBuf(void)
{
    for(uint8_t i=0;i<WS2812_NUM;i++)
    {
        ws2812_buf[i].R=0;
        ws2812_buf[i].G=0;
        ws2812_buf[i].B=0;
    }
}

void BSP_WS2812_ClearAll(void)
{
    // 清零缓冲区
    BSP_WS2812_ClearBuf();
    // 发送清零数据
    BSP_WS2812_Refresh();
    // 复位延时
    WS_LOW();
    R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MICROSECONDS);
}
