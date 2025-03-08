#ifndef  _CONTROL_H
#define  _CONTROL_H

#include "stm32f10x.h"                  // Device header
#include "inv_mpu.h"
extern float pitc, rol, ya;
extern int8_t B,sum_test;
void EXTI9_5_IRQHandler(void);
#endif

