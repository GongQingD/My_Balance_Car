#ifndef __MOTOR_H
#define __MOTOR_H
#include "stdint.h"
void Motor_Init(void);
void Motor_SetSpeed(int16_t Motor_PWM_out_Left, int16_t Motor_PWM_out_Right);

#endif
