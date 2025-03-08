#include "Control.h"
#include "Sys_Init.h"
float pitc=0, rol=0, ya=0;
int8_t B=0, sum_test = 0;
int16_t L_encoder = 0, R_encoder = 0, Motor_PWM_out_Right = 0, Motor_PWM_out_Left = 0;
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//�ж��Ƿ����ⲿ�ж�5���ߴ������ж�
	{
		sum_test += 1;
		B = MPU6050_DMP_Get_Data(&pitc,&rol,&ya);
		
		L_encoder = Encoder_Get2();
		R_encoder = Encoder_Get4();
		
		Motor_PWM_out_Right = 500;
		Motor_PWM_out_Left = 600;
		PWM_Limit(&Motor_PWM_out_Left,&Motor_PWM_out_Right);
		Motor_SetSpeed(Motor_PWM_out_Left,Motor_PWM_out_Right);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		EXTI_ClearITPendingBit(EXTI_Line5);		//����ⲿ�ж�14���ߵ��жϱ�־λ
													//�жϱ�־λ�������
													//�����жϽ��������ϵش�����������������
	}
}

