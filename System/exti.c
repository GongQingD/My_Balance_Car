#include "exti.h"
#include "stm32f10x.h"  // ����STM32F10xϵ��΢���������豸ͷ�ļ�

// MPU6050�ⲿ�жϳ�ʼ������
void MPU6050_EXTI_Init(void)
{
    // ���岢��ʼ��EXTI��GPIO�Ľṹ�����
    EXTI_InitTypeDef EXTI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // ����GPIOB��AFIO�����ù���IO����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // ����GPIOB��Pin5Ϊ��������ģʽ
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  // ��������ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;      // ѡ��Pin5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // GPIO�ٶ�����Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);  // ��ʼ��GPIOB��Pin5
    
    // ����GPIOB��Pin5Ϊ�ⲿ�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
    
    // �����ⲿ�ж���5
    EXTI_InitStruct.EXTI_Line = EXTI_Line5;  // ѡ���ⲿ�ж���5
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;  // ʹ���ⲿ�ж���
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  // ����Ϊ�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // ����Ϊ�����ش���
    EXTI_Init(&EXTI_InitStruct);  // ��ʼ���ⲿ�ж�]
		
		/*NVIC�жϷ���*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//����NVICΪ����2
																	//����ռ���ȼ���Χ��0~3����Ӧ���ȼ���Χ��0~3
																	//�˷������������������н������һ��
																	//���ж���жϣ����԰Ѵ˴������main�����ڣ�whileѭ��֮ǰ
																	//�����ö�����÷���Ĵ��룬���ִ�е����ûḲ����ִ�е�����
		
		/*NVIC����*/
		NVIC_InitTypeDef NVIC_InitStructure;						//����ṹ�����
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//ѡ������NVIC��EXTI15_10��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ָ��NVIC��·ʹ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//ָ��NVIC��·����ռ���ȼ�Ϊ1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
		NVIC_Init(&NVIC_InitStructure);								//���ṹ���������NVIC_Init������NVIC����
}

