#include "stm32f10x.h"                  // Device header
#include "Sys_Init.h"
/**
  * ��    ����ֱ�������ʼ��
  * ��    ������
  * �� �� ֵ����
  * ��    �ܣ�����ֱ�����ת�������ٶ�
  */
void Motor_Init(void)
{
	/*����ʱ�ӣ���ʼ���������ģ��ķ�������AIN1��AIN2��BIN1��BIN2*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOA��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PA4��PA5���ų�ʼ��Ϊ�������	
}

/**
  * ��    ��������ֱ����������ٶ�
  * ��    �������ҵ����CCRֵ
  * �� �� ֵ����
  */
void Motor_SetSpeed(int16_t Motor_PWM_out_Left, int16_t Motor_PWM_out_Right)
{
	/*******************************����*****************************************/
	if (Motor_PWM_out_Left >= 0)							//���������ת���ٶ�ֵ
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);	//PB13�øߵ�ƽ��AIN1=1��
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);	//PB12�õ͵�ƽ�����÷���Ϊ��ת��AIN2=0��
		PWM_SetCompare4(Motor_PWM_out_Left);				//PWM����Ϊ�ٶ�ֵ
	}
	else									//���򣬼����÷�ת���ٶ�ֵ
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);	//PB13�õ͵�ƽ��AIN1=0��
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	//PB12�øߵ�ƽ�����÷���Ϊ��ת��AIN2=1��
		PWM_SetCompare4(-Motor_PWM_out_Left);			//PWM����Ϊ�����ٶ�ֵ����Ϊ��ʱ�ٶ�ֵΪ��������PWMֻ�ܸ�����
	}
	/*******************************�ҵ��*****************************************/
	if (Motor_PWM_out_Right >= 0)							//���������ת���ٶ�ֵ
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	//PB14�øߵ�ƽ��BIN1=1��
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	//PB15�õ͵�ƽ�����÷���Ϊ��ת��BIN2=0��
		PWM_SetCompare1(Motor_PWM_out_Right);				//PWM����Ϊ�ٶ�ֵ
	}
	else									//���򣬼����÷�ת���ٶ�ֵ
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	//PB14�õ͵�ƽ��BIN1=0��
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	//PB15�øߵ�ƽ�����÷���Ϊ��ת��BIN2=1��
		PWM_SetCompare1(-Motor_PWM_out_Right);			//PWM����Ϊ�����ٶ�ֵ����Ϊ��ʱ�ٶ�ֵΪ��������PWMֻ�ܸ�����
	}
}
