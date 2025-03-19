#include "stm32f10x.h"                  // Device header

#include "Sys_Init.h"

#include "Control.h"



int main(void)
{
	MySys_Init();


	while (1)
	{
		
		OLED_ShowSignedNum(1,1,pitc,3);
		OLED_ShowSignedNum(2,1,rol,3);
		OLED_ShowSignedNum(3,1,ya,3);
		OLED_ShowSignedNum(4,1,B,3);
		
		
		OLED_ShowNum(1,8,sum_test,4);
		OLED_ShowSignedNum(2,8,Motor_Encoder_Left,5);
  	OLED_ShowSignedNum(3,8,Motor_Encoder_Right,5);
	}
}
