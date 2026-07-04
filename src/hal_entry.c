#include "hal_data.h"
#include "gpt/bsp_gpt_timing.h"
#include "gpt/bsp_servo.h"
#include "led/bsp_led_long.h"
#include "oled/oled.h"
#include "adc/bsp_light.h"
#include "adc/bsp_smoke.h"
#include "adc/bsp_soil.h"
#include "adc/fire_sensor.h"
#include "motor/bsp_pump.h"
#include "motor/bsp_motor.h"
#include "motor/bsp_motor_gpio.h"
#include "motor/bsp_motor_key.h"
#include "use/bsp_dht11.h"
#include "use/bsp_mist.h"
#include "led/bsp_led.h"
FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

#define FIRE_DO_PIN BSP_IO_PORT_00_PIN_01

#define KEY_PIN BSP_IO_PORT_04_PIN_14
#define KEY_SERVO_PIN BSP_IO_PORT_08_PIN_00
#define KEY_MIST_PIN BSP_IO_PORT_02_PIN_09
#define KEY_DUAL_SERVO_PIN BSP_IO_PORT_02_PIN_14

#define WS2812_PORT     R_PORT5
#define WS2812_MASK     ((uint16_t)1U << 11U)
#define WS_LOW()        WS2812_PORT->PORR = WS2812_MASK

#define KEY_FILTER_MS 10

typedef enum {
    COLOR_OFF = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_WHITE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_PURPLE,
    COLOR_ORANGE,
    COLOR_PINK,
    COLOR_MODE_MAX
} color_mode_t;

uint8_t ws2812_on_flag = 0;
uint16_t tick_ws2812 = 0;
uint8_t lock_ws2812 = 0;
color_mode_t current_color_mode = COLOR_OFF;

uint8_t servo_open_flag = 0;
uint16_t tick_servo = 0;
uint8_t lock_servo = 0;

uint8_t dual_servo_flag = 0;
uint16_t tick_dual_servo = 0;
uint8_t lock_dual_servo = 0;

uint8_t mist_on_flag = 0;
uint16_t tick_mist = 0;
uint8_t lock_mist = 0;

uint8_t motor_a_run = 0;
uint8_t motor_b_run = 0;
uint8_t motor_b_dir_flag = 0;
uint16_t motor_run_cnt = 0;
#define MOTOR_RUN_MS 3000
#define BRIGHT_HIGH 50
#define BRIGHT_LOW 30
#define SMOKE_HIGH_THRESHOLD 2500

#define SOIL_DRY_THRESHOLD 2500

void WS2812_SetAllColor(uint8_t R, uint8_t G, uint8_t B)
{
    for(uint8_t i = 0; i < WS2812_NUM; i++)
    {
        BSP_WS2812_SetColor(i, R, G, B);
    }
    BSP_WS2812_Refresh();
}

void WS2812_SetMode(color_mode_t mode)
{
    current_color_mode = mode;
    
    switch(mode)
    {
        case COLOR_OFF:
            BSP_WS2812_ClearAll();
            break;
        case COLOR_RED:
            WS2812_SetAllColor(100, 0, 0);
            break;
        case COLOR_GREEN:
            WS2812_SetAllColor(0, 100, 0);
            break;
        case COLOR_BLUE:
            WS2812_SetAllColor(0, 0, 100);
            break;
        case COLOR_WHITE:
            WS2812_SetAllColor(80, 80, 80);
            break;
        case COLOR_YELLOW:
            WS2812_SetAllColor(100, 100, 0);
            break;
        case COLOR_CYAN:
            WS2812_SetAllColor(0, 100, 100);
            break;
        case COLOR_PURPLE:
            WS2812_SetAllColor(100, 0, 100);
            break;
        case COLOR_ORANGE:
            WS2812_SetAllColor(100, 50, 0);
            break;
        case COLOR_PINK:
            WS2812_SetAllColor(100, 0, 100);
            break;
        default:
            BSP_WS2812_ClearAll();
            break;
    }
}

void Hardware_init(void)
{
    R_BSP_PinAccessEnable();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "Smo:");        // 烟雾
    OLED_ShowString(2, 1, "Fire:");       // 火焰
    OLED_ShowString(3, 1, "T:");          // 温度
    OLED_ShowString(3, 6, "C H:");        // 湿度
    OLED_ShowString(4, 1, "Pump:");       // 水泵
    OLED_ShowString(4, 7, "Light:");       // 光照
    ADC_Init();
    Fire_ADC_Init();
    Pump_Init();
    Motor_GPIO_Init();
    Motor_PWM_Init();
    Motor_Key_Init();
    DHT11_Init();
    Mist_Init();
    BSP_LED_Init();
    Motor_SetDir(MOTOR_CH_A, MOTOR_STOP);
    Motor_SetDir(MOTOR_CH_B, MOTOR_STOP);
}

void hal_entry(void)
{
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, BSP_IO_LEVEL_HIGH);
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_01, BSP_IO_LEVEL_LOW);
    
    BSP_WS2812_Init();
    BSP_WS2812_ClearAll();
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    
    GPT_PWM_Init();
    GPT_PWM_SetDuty(12.5f);
    AGT_Servo_Init();
    AGT_Servo_SetDuty_A(12.5f);
    AGT_Servo_SetDuty_B(7.5f);
    servo_open_flag = 0;
    dual_servo_flag = 0;
    current_color_mode = COLOR_OFF;
    
    Hardware_init();
    
    uint16_t smoke_val = 0;
    uint16_t fire_val = 0;
    uint16_t soil_val = 0;
    uint8_t key_val = 0;
    uint16_t light_raw = 0;
    uint8_t light_bright = 0;
    
    DHT11_Data_TypeDef DHT11_Data;
    uint8_t temp_val = 0;
    uint8_t humi_val = 0;
    
    uint16_t smoke_buf = 0xFFFF;
    uint16_t fire_buf = 0xFFFF;
    uint16_t light_buf = 0xFFFF;
    uint8_t temp_buf = 0xFF;
    uint8_t humi_buf = 0xFF;
    uint8_t pump_state_buf = 0xFF;
    uint16_t adc_cnt = 0;
    uint16_t dht_cnt = 0;
    
    const uint16_t ADC_REFRESH_CNT = 25;
    const uint16_t DHT_REFRESH_CNT = 500;
    
    bsp_io_level_t key_ws2812_state;
    bsp_io_level_t key_servo_state;
    
    dht_cnt = DHT_REFRESH_CNT;

    while(1)
    {
        R_IOPORT_PinRead(&g_ioport_ctrl, KEY_PIN, &key_ws2812_state);
        if(key_ws2812_state == BSP_IO_LEVEL_LOW)
        {
            if(!lock_ws2812)
            {
                tick_ws2812++;
                if(tick_ws2812 >= KEY_FILTER_MS)
                {
                    current_color_mode = (current_color_mode + 1) % COLOR_MODE_MAX;
                    WS2812_SetMode(current_color_mode);
                    lock_ws2812 = 1;
                }
            }
        }
        else
        {
            tick_ws2812 = 0;
            lock_ws2812 = 0;
        }

        R_IOPORT_PinRead(&g_ioport_ctrl, KEY_SERVO_PIN, &key_servo_state);
        if(key_servo_state == BSP_IO_LEVEL_LOW)
        {
            if(!lock_servo)
            {
                tick_servo++;
                if(tick_servo >= KEY_FILTER_MS)
                {
                    servo_open_flag = !servo_open_flag;
                    if(servo_open_flag)
                    {
                        GPT_PWM_SetDuty(7.5f);
                    }
                    else
                    {
                        GPT_PWM_SetDuty(12.5f);
                    }
                    lock_servo = 1;
                }
            }
        }
        else
        {
            tick_servo = 0;
            lock_servo = 0;
        }

        bsp_io_level_t key_dual_servo_state;
        R_IOPORT_PinRead(&g_ioport_ctrl, KEY_DUAL_SERVO_PIN, &key_dual_servo_state);
        if(key_dual_servo_state == BSP_IO_LEVEL_LOW)
        {
            if(!lock_dual_servo)
            {
                tick_dual_servo++;
                if(tick_dual_servo >= KEY_FILTER_MS)
                {
                    dual_servo_flag = !dual_servo_flag;
                    if(dual_servo_flag)
                    {
                        AGT_Servo_SetDuty_A(7.5f);
                        AGT_Servo_SetDuty_B(12.5f);
                    }
                    else
                    {
                        AGT_Servo_SetDuty_A(12.5f);
                        AGT_Servo_SetDuty_B(7.5f);
                    }
                    lock_dual_servo = 1;
                }
            }
        }
        else
        {
            tick_dual_servo = 0;
            lock_dual_servo = 0;
        }

        bsp_io_level_t key_mist_state;
        R_IOPORT_PinRead(&g_ioport_ctrl, KEY_MIST_PIN, &key_mist_state);
        if(key_mist_state == BSP_IO_LEVEL_LOW)
        {
            if(!lock_mist)
            {
                tick_mist++;
                if(tick_mist >= KEY_FILTER_MS)
                {
                    mist_on_flag = !mist_on_flag;
                    if(mist_on_flag)
                    {
                        Mist_Ctrl(MIST_ON);
                    }
                    else
                    {
                        Mist_Ctrl(MIST_OFF);
                    }
                    lock_mist = 1;
                }
            }
        }
        else
        {
            tick_mist = 0;
            lock_mist = 0;
        }
        
        key_val = Motor_Key_Scan();
        
        if(key_val == 1)
        {
            motor_a_run = !motor_a_run;
            if(motor_a_run)
            {
                Motor_SetDir(MOTOR_CH_A, MOTOR_FORWARD);
                Motor_PWM_SetDuty_A(80.0f);
            }
            else
            {
                Motor_SetDir(MOTOR_CH_A, MOTOR_STOP);
            }
        }
        
        else if(key_val == 2 && motor_b_run == 0)
        {
            motor_b_run = 1;
            motor_run_cnt = 0;
            if(motor_b_dir_flag == 0)
            {
                Motor_SetDir(MOTOR_CH_B, MOTOR_FORWARD);
                Motor_PWM_SetDuty_B(70.0f);
            }
            else
            {
                Motor_SetDir(MOTOR_CH_B, MOTOR_BACKWARD);
                Motor_PWM_SetDuty_B(70.0f);
            }
        }
        
        if(motor_b_run == 1)
        {
            motor_run_cnt++;
            if(motor_run_cnt >= MOTOR_RUN_MS / 20)
            {
                Motor_SetDir(MOTOR_CH_B, MOTOR_STOP);
                Motor_PWM_SetDuty_B(0.0f);
                motor_b_run = 0;
                motor_b_dir_flag = !motor_b_dir_flag;
            }
        }
        
        adc_cnt++;
        if(adc_cnt >= ADC_REFRESH_CNT)
        {
            adc_cnt = 0;
            
            smoke_val = Smoke_Read_Filtered();
            fire_val = Fire_Read_Raw();
            soil_val = Soil_Read_Filtered();
            light_raw = Read_ADC_Light_Filtered();
            light_bright = Light_Get_Brightness(light_raw);
            
            if(smoke_val != smoke_buf)
            {
                smoke_buf = smoke_val;
                OLED_ShowNum(1, 5, smoke_val, 4);
            }
            
            if(smoke_val > SMOKE_HIGH_THRESHOLD)
            {
                if(motor_a_run == 0)
                {
                    motor_a_run = 1;
                    Motor_SetDir(MOTOR_CH_A, MOTOR_FORWARD);
                    Motor_PWM_SetDuty_A(80.0f);
                }
            }
            
            if(fire_val != fire_buf)
            {
                fire_buf = fire_val;
                OLED_ShowNum(2, 6, fire_val, 4);
            }
            
            if(soil_val > SOIL_DRY_THRESHOLD)
            {
                Pump_Ctrl(PUMP_RUN, 60.0f);
                if(pump_state_buf != 1)
                {
                    pump_state_buf = 1;
                    OLED_ShowString(4, 6, "ON ");
                }
            }
            else
            {
                Pump_Ctrl(PUMP_STOP, 0.0f);
                if(pump_state_buf != 0)
                {
                    pump_state_buf = 0;
                    OLED_ShowString(4, 6, "OFF");
                }
            }
            
            if(fire_val < 2000)
            {
                BSP_LED_Set(LED_ON);
            }
            else
            {
                BSP_LED_Set(LED_OFF);
            }
            
            if(light_raw != light_buf)
            {
                light_buf = light_raw;
                OLED_ShowNum(4, 7, light_bright, 3);
                OLED_ShowString(4, 10, "%");
            }
            
            if(light_bright > BRIGHT_HIGH && motor_b_run == 0)
            {
                motor_b_run = 1;
                motor_run_cnt = 0;
                Motor_SetDir(MOTOR_CH_B, MOTOR_BACKWARD);
                Motor_PWM_SetDuty_B(70.0f);
            }
            else if(light_bright < BRIGHT_LOW && motor_b_run == 0)
            {
                motor_b_run = 1;
                motor_run_cnt = 0;
                Motor_SetDir(MOTOR_CH_B, MOTOR_FORWARD);
                Motor_PWM_SetDuty_B(70.0f);
            }
            else if(light_bright >= BRIGHT_LOW && light_bright <= BRIGHT_HIGH)
            {
            }
        }
        
        dht_cnt++;
        if(dht_cnt >= DHT_REFRESH_CNT)
        {
            dht_cnt = 0;
            
            uint8_t read_state = Read_DHT11(&DHT11_Data);
            if(read_state == 1)
            {
                temp_val = DHT11_Data.temp_int;
                humi_val = DHT11_Data.humi_int;
            }
            else
            {
                temp_val = 0xFF;
                humi_val = 0xFF;
            }
            
            if(temp_val != temp_buf)
            {
                temp_buf = temp_val;
                if(temp_val == 0xFF)
                    OLED_ShowString(3, 3, "--");
                else
                    OLED_ShowNum(3, 3, temp_val, 2);
            }
            
            if(humi_val != humi_buf)
            {
                humi_buf = humi_val;
                if(humi_val == 0xFF)
                    OLED_ShowString(3, 9, "--");
                else
                    OLED_ShowNum(3, 9, humi_val, 2);
            }
        }
        
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0
        R_FACI_LP->DFLCTL = 1U;
#endif
    }
    if (BSP_WARM_START_POST_C == event)
    {
        R_IOPORT_Open (&g_ioport_ctrl, &IOPORT_CFG_NAME);
    }
}

#if BSP_TZ_SECURE_BUILD
FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{
}
FSP_CPP_FOOTER
#endif
