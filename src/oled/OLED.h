#ifndef __DRV_OLED_H
#define __DRV_OLED_H

#include "hal_data.h"
#include <stdint.h>
#define I2C_DELAY()  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS)

// SDA方向切换
void OLED_SDA_OUT(void);
void OLED_SDA_IN(void);
void OLED_W_SCL(uint8_t level);
void OLED_W_SDA(uint8_t level);
uint8_t OLED_R_SDA(void);

// I2C时序
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
uint8_t OLED_I2C_SendByte(uint8_t Byte);

// 底层发送封装
static void I2C_WriteBuf(uint8_t dev_addr, uint8_t *buf, uint8_t len);
void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);

// OLED显存操作
void OLED_SetCursor(uint8_t Y, uint8_t X);
void OLED_Clear(void);

// 显示接口
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// 工具函数
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
// OLED初始化
void OLED_Init(void);

#endif
