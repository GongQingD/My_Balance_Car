#include "Control.h"

float pitc=0, rol=0, ya=0;
int8_t B=0, sum_test = 0;
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//�ж��Ƿ����ⲿ�ж�5���ߴ������ж�
	{
		sum_test += 1;
		B = MPU6050_DMP_Get_Data(&pitc,&rol,&ya);
		EXTI_ClearITPendingBit(EXTI_Line5);		//����ⲿ�ж�14���ߵ��жϱ�־λ
													//�жϱ�־λ�������
													//�����жϽ��������ϵش�����������������
	}
}

