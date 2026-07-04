#ifndef BSP_SERVO_KEY_H_
#define BSP_SERVO_KEY_H_
#include "hal_data.h"

#define KEY_SERVO_CTRL     BSP_IO_PORT_08_PIN_00

void Servo_Key_Init(void);
uint8_t Servo_Key_Scan(void);

#endif
