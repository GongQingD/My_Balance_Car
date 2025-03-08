#include "stm32f10x.h"                  // Device header
#include "Timer.h"
/**
  * ��    ����TIM3��TIM4��������ʼ��
  * ��    ������
  * �� �� ֵ����
  */

void Encoder_Init(void)
{
	
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//����TIM2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//��PA6��PA7���ų�ʼ��Ϊ��������
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM3��ʱ����Ԫ
	
	/*�������ӿ�����*/
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//���ñ�����ģʽ�Լ���������ͨ���Ƿ���
																	//ע���ʱ������Rising��Falling�Ѿ������������غ��½����ˣ����Ǵ����Ƿ���
																	//�˺������������벶���ʼ��֮����У��������벶������ûḲ�Ǵ˺����Ĳ�������
	
	/*���벶���ʼ��*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//����ṹ�����
	TIM_ICStructInit(&TIM_ICInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//ѡ�����ö�ʱ��ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0x10;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM2, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//ѡ�����ö�ʱ��ͨ��2
	TIM_ICInitStructure.TIM_ICFilter = 0x10;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM2, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	
	
	
	TIM_SetCounter(TIM2,0);//���㶨ʱ������ֵ
	/*TIMʹ��*/
	TIM_Cmd(TIM2, ENABLE);			//ʹ��TIM3����ʱ����ʼ����
	
	/*TIM4��ʼ��***************************************************************************************/
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//����TIM4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//��PA6��PA7���ų�ʼ��Ϊ��������
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM3��ʱ����Ԫ
	
	/*�������ӿ�����*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//���ñ�����ģʽ�Լ���������ͨ���Ƿ���
																	//ע���ʱ������Rising��Falling�Ѿ������������غ��½����ˣ����Ǵ����Ƿ���
																	//�˺������������벶���ʼ��֮����У��������벶������ûḲ�Ǵ˺����Ĳ�������
	
	/*���벶���ʼ��*/
	TIM_ICStructInit(&TIM_ICInitStructure);							//�ṹ���ʼ�������ṹ��û��������ֵ
																	//�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
																	//����ṹ���ֵ��ȷ��������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				//ѡ�����ö�ʱ��ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM4, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				//ѡ�����ö�ʱ��ͨ��2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;							//�����˲������������Թ����źŶ���
	TIM_ICInit(TIM4, &TIM_ICInitStructure);							//���ṹ���������TIM_ICInit������TIM3�����벶��ͨ��
	
	TIM_SetCounter(TIM4,0);//���㶨ʱ������ֵ
	/*TIMʹ��*/
	TIM_Cmd(TIM4, ENABLE);			//ʹ��TIM3����ʱ����ʼ����
}

/**
  * ��    ������ȡ������������ֵ����ʱ��TIM2��
  * ��    ������
  * �� �� ֵ�����ϴ˵��ô˺����󣬱�����������ֵ
  */
int16_t Encoder_Get2(void)
{
	/*ʹ��Temp������Ϊ�м̣�Ŀ���Ƿ���CNT��������*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2, 0);
	return Temp;
}
/**
  * ��    ������ȡ������������ֵ����ʱ��TIM4��
  * ��    ������
  * �� �� ֵ�����ϴ˵��ô˺����󣬱�����������ֵ
  */
int16_t Encoder_Get4(void)
{
	/*ʹ��Temp������Ϊ�м̣�Ŀ���Ƿ���CNT��������*/
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}

