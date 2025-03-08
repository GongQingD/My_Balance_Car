#include "stm32f10x.h"                  // Device header

#include "Sys_Init.h"
#include "Encoder.h"
#include "Control.h"

int8_t ID;
int16_t L_encoder, R_encoder;

int main(void)
{
	MySys_Init();
	
	

	L_encoder = Encoder_Get2();
	R_encoder = Encoder_Get4();

	while (1)
	{
		
		OLED_ShowSignedNum(1,1,pitc,3);
		OLED_ShowSignedNum(2,1,rol,3);
		OLED_ShowSignedNum(3,1,ya,3);
		OLED_ShowSignedNum(4,1,B,3);
		OLED_ShowNum(1,8,sum_test,4);
	}
}
