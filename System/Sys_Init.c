#include "Sys_Init.h"


/**************************************************
功能：.c文件把全部初始化函数写到一起，.h文件把所有用到的头函数文件以及变量写到一起
**************************************************/
void MySys_Init(void)
{
	OLED_Init();	
	OLED_Clear();
	Serial_Init();
	
	MPU6050_Init();
	MPU6050_DMP_Init();
	MPU6050_EXTI_Init();
	
	Encoder_Init();
	PWM_Init();
	Motor_Init();
	
	
	
	
}
