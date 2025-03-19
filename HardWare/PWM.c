#include "stm32f10x.h"                  // Device header

/**
  * ��    ����PWM��ʼ��
  * ��    ������
  * �� �� ֵ����
  */
  uint16_t ARR =  7200 -1;//����PWMƵ��Ϊ10kHz
void PWM_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//����TIM1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//��PA2���ų�ʼ��Ϊ�����������	
																	//��������Ƶ����ţ�����Ҫ����Ϊ����ģʽ
	
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM1);		//ѡ��TIM2Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = ARR;                 //�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;               //Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM2��ʱ����Ԫ
	
	/*����Ƚϳ�ʼ��*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;							//����ṹ�����
	TIM_OCStructInit(&TIM_OCInitStructure);                         //�ṹ���ʼ�������ṹ��û��������ֵ
	                                                                //�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
	                                                                //����ṹ���ֵ��ȷ��������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //����Ƚ�ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //������ԣ�ѡ��Ϊ�ߣ���ѡ����Ϊ�ͣ�������ߵ͵�ƽȡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//��ʼ��CCRֵ
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                        //���ṹ���������TIM_OC1Init������TIM1������Ƚ�ͨ��1
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);                        //���ṹ���������TIM_OC4Init������TIM1������Ƚ�ͨ��4
	/*TIMʹ��*/
	TIM_Cmd(TIM1, ENABLE);			//ʹ��TIM1����ʱ����ʼ����
	TIM_CtrlPWMOutputs(TIM1, ENABLE); // ���ø߼���ʱ�������
}

/**
  * ��    ����PWM����CCR
  * ��    ����Compare Ҫд���CCR��ֵ����Χ��0~ARR
  * �� �� ֵ����
  * ע�����CCR��ARR��ͬ����ռ�ձȣ��˺���������CCR��ֵ������ֱ����ռ�ձ�
  *           ռ�ձ�Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);		//����CCR4��ֵ
}

/**
  * ��    ����PWM����CCR
  * ��    ����Compare Ҫд���CCR��ֵ����Χ��0~ARR
  * �� �� ֵ����
  * ע�����CCR��ARR��ͬ����ռ�ձȣ��˺���������CCR��ֵ������ֱ����ռ�ձ�
  *           ռ�ձ�Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);		//����CCR1��ֵ
}

//**
//  * ��    ����PWM�޷�����Ӳ�޷��Ϳ����ͱ�����
//  * ��    ����Motor_PWM_out_Left  ����PWM���루����������ã�
//  *         Motor_PWM_out_Right �ҵ��PWM���루����������ã�
//  * �� �� ֵ����
//  * ˵    ����ARR=1800����ȫ�޷���Χ����1500������300��Ϊ��ȫ������

void PWM_Limit(int16_t *Motor_PWM_out_Left, int16_t *Motor_PWM_out_Right)
{
    /*------------------------ �������� ------------------------*/
    const int16_t PWM_MAX = ARR+1; // Ӳ�޷����ֵ��ARR=1800ʱ����300������
		const int16_t PWM_MIN = - PWM_MAX;
    /*------------------------ �������� ------------------------*/
    
    // ����Ӳ�޷�
    if (*Motor_PWM_out_Left > PWM_MAX) *Motor_PWM_out_Left = PWM_MAX;
		if (*Motor_PWM_out_Left < PWM_MIN) *Motor_PWM_out_Left = PWM_MIN;

    /*------------------------ �ҵ�������߼���ͬ�� ------------------------*/
		if (*Motor_PWM_out_Right > PWM_MAX) *Motor_PWM_out_Right = PWM_MAX;
		if (*Motor_PWM_out_Right < PWM_MIN) *Motor_PWM_out_Right = PWM_MIN;
}

