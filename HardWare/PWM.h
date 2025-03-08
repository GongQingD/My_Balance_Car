#ifndef __PWM_H
#define __PWM_H
#include "stdint.h"
void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
void PWM_Limit(int16_t *Motor_PWM_out_Left, int16_t *Motor_PWM_out_Right);
#endif
