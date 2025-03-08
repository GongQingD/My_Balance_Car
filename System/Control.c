#include "Control.h"

float pitc=0, rol=0, ya=0;
int8_t B=0, sum_test = 0;
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断5号线触发的中断
	{
		sum_test += 1;
		B = MPU6050_DMP_Get_Data(&pitc,&rol,&ya);
		EXTI_ClearITPendingBit(EXTI_Line5);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}

