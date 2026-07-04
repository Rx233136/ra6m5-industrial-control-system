#include "hal_data.h"
#include "oled/OLED_Font.h"
#include "oled/oled.h"

#define I2C_DELAY()  R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS)

// SCL P112 开漏输出
void OLED_W_SCL(uint8_t level)
{
    bsp_io_level_t io_lv = level ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW;
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_12, io_lv);
    I2C_DELAY();
}

// SDA直接写，硬件开漏允许外设拉低电平，不再切换输入输出
void OLED_W_SDA(uint8_t level)
{
    bsp_io_level_t io_lv = level ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW;
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13, io_lv);
    I2C_DELAY();
}

// 读取SDA电平，硬件开启输入缓冲，无需改方向
uint8_t OLED_R_SDA(void)
{
    bsp_io_level_t pin_val;
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13, &pin_val);
    return (pin_val == BSP_IO_LEVEL_HIGH) ? 1 : 0;
}

// I2C起始
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    I2C_DELAY();
    OLED_W_SDA(0);
    I2C_DELAY();
    OLED_W_SCL(0);
}

// I2C停止
void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    I2C_DELAY();
    OLED_W_SDA(1);
    I2C_DELAY();
}

// 发送单字节，ACK时序修正（硬件开漏，无需切引脚方向）
uint8_t OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA((Byte & (0x80U >> i)) ? 1 : 0);
        OLED_W_SCL(1);
        I2C_DELAY();
        OLED_W_SCL(0);
        I2C_DELAY();
    }
    // 释放SDA，硬件开漏，OLED可主动拉低
    OLED_W_SDA(1);
    I2C_DELAY();
    OLED_W_SCL(1);
    I2C_DELAY();
    uint8_t ack = OLED_R_SDA();
    OLED_W_SCL(0);
    return ack;
}

// 写命令
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    if(OLED_I2C_SendByte(0x78U) != 0)
    {
        OLED_I2C_Stop();
        return;
    }
    OLED_I2C_SendByte(0x00U);
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

// 写数据
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    if(OLED_I2C_SendByte(0x78U) != 0)
    {
        OLED_I2C_Stop();
        return;
    }
    OLED_I2C_SendByte(0x40U);
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}

// 设置光标
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0U | Y);
    OLED_WriteCommand(0x10U | ((X & 0xF0U) >> 4U));
    OLED_WriteCommand(0x00U | (X & 0x0FU));
}

// 清屏
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0U; j < 8U; j++)
    {
        OLED_SetCursor(j, 0U);
        for (i = 0U; i < 128U; i++)
        {
            OLED_WriteData(0x00U);
        }
    }
}

// 单字符显示
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t y = (Line - 1U) * 2U;
    uint8_t x = (Column - 1U) * 8U;
    OLED_SetCursor(y, x);
    for (uint8_t i = 0U; i < 8U; i++)
    {
        OLED_WriteData(OLED_F8x16[(uint8_t)(Char - ' ')][i]);
    }
    OLED_SetCursor(y + 1U, x);
    for (uint8_t i = 0U; i < 8U; i++)
    {
        OLED_WriteData(OLED_F8x16[(uint8_t)(Char - ' ')][i + 8U]);
    }
}

// 字符串
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

// 次方工具
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1U;
    while (Y--) Result *= X;
    return Result;
}

// 无符号数字（4参数，和头文件完全匹配）
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        uint32_t div = OLED_Pow(10U, Length - i - 1U);
        uint8_t num = (Number / div) % 10U;
        OLED_ShowChar(Line, Column + i, '0' + num);
    }
}

// 带符号数字
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t num_val;
    if(Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        num_val = (uint32_t)Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        num_val = (uint32_t)(-Number);
    }
    for (i = 0; i < Length; i++)
    {
        uint32_t div = OLED_Pow(10U, Length - i - 1U);
        uint8_t num = (num_val / div) % 10U;
        OLED_ShowChar(Line, Column + i + 1, '0' + num);
    }
}

// 十六进制
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, tmp;
    for (i = 0; i < Length; i++)
    {
        uint32_t div = OLED_Pow(16U, Length - i - 1U);
        tmp = (Number / div) % 16U;
        if(tmp < 10) OLED_ShowChar(Line, Column + i, '0' + tmp);
        else OLED_ShowChar(Line, Column + i, 'A' + tmp - 10U);
    }
}

// 二进制
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        uint32_t div = OLED_Pow(2U, Length - i - 1U);
        uint8_t num = (Number / div) % 2U;
        OLED_ShowChar(Line, Column + i, '0' + num);
    }
}

// OLED初始化
// OLED初始化
void OLED_Init(void)
{
    R_IOPORT_Open(&g_ioport_ctrl, g_ioport.p_cfg);
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);

    OLED_WriteCommand(0xAEU);
    OLED_WriteCommand(0xD5U); OLED_WriteCommand(0x80U);
    OLED_WriteCommand(0xA8U); OLED_WriteCommand(0x3FU);
    OLED_WriteCommand(0xD3U); OLED_WriteCommand(0x00U);
    OLED_WriteCommand(0x40U);
    OLED_WriteCommand(0xA1U);
    OLED_WriteCommand(0xC8U);
    OLED_WriteCommand(0xDAU); OLED_WriteCommand(0x12U);
    OLED_WriteCommand(0x81U); OLED_WriteCommand(0x30U);
    OLED_WriteCommand(0xD9U); OLED_WriteCommand(0xF1U);
    OLED_WriteCommand(0xDBU); OLED_WriteCommand(0x30U);
    OLED_WriteCommand(0xA4U);
    OLED_WriteCommand(0xA6U);
    OLED_WriteCommand(0x8DU); OLED_WriteCommand(0x14U);
    OLED_WriteCommand(0xAFU);

    // 新增：上电强制清空全部显存，消除残留画面
    OLED_Clear();
    R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);
}
