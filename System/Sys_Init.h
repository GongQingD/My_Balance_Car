#ifndef __SYS_INIT_
#define __SYS_INIT_

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "exti.h"
#include "Delay.h"
#include "Encoder.h"

extern int16_t L_encoder, R_encoder;

void MySys_Init(void);

#endif 


