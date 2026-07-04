#ifndef BSP_LED_LONG_H_
#define BSP_LED_LONG_H_
#include "hal_data.h"

#define WS2812_NUM      8U
#define WS2812_PORT     R_PORT5
#define WS2812_PIN_BIT  (1U << 11U)

typedef struct {
    uint8_t G;
    uint8_t R;
    uint8_t B;
} ws2812_rgb_t;

extern ws2812_rgb_t ws2812_buf[WS2812_NUM];

void BSP_WS2812_Init(void);
void BSP_WS2812_Refresh(void);
void BSP_WS2812_SetColor(uint8_t index, uint8_t R, uint8_t G, uint8_t B);
void BSP_WS2812_ClearAll(void);
void BSP_WS2812_ClearBuf(void);

#endif
