#ifndef __SYS_INIT_
#define __SYS_INIT_

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "exti.h"
#include "Delay.h"
#include "Encoder.h"
#include "PWM.h"
#include "Motor.h"
extern int16_t L_encoder, R_encoder, Motor_PWM_out_Left, Motor_PWM_out_Right;

void MySys_Init(void);

#endif 


