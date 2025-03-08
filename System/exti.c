#include "exti.h"
#include "stm32f10x.h"  // 包含STM32F10x系列微控制器的设备头文件

// MPU6050外部中断初始化函数
void MPU6050_EXTI_Init(void)
{
    // 定义并初始化EXTI和GPIO的结构体变量
    EXTI_InitTypeDef EXTI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 开启GPIOB和AFIO（复用功能IO）的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置GPIOB的Pin5为上拉输入模式
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;      // 选择Pin5
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // GPIO速度设置为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStruct);  // 初始化GPIOB的Pin5
    
    // 配置GPIOB的Pin5为外部中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
    
    // 配置外部中断线5
    EXTI_InitStruct.EXTI_Line = EXTI_Line5;  // 选择外部中断线5
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;  // 使能外部中断线
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  // 设置为中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // 设置为上升沿触发
    EXTI_Init(&EXTI_InitStruct);  // 初始化外部中断]
		
		/*NVIC中断分组*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				//配置NVIC为分组2
																	//即抢占优先级范围：0~3，响应优先级范围：0~3
																	//此分组配置在整个工程中仅需调用一次
																	//若有多个中断，可以把此代码放在main函数内，while循环之前
																	//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
		
		/*NVIC配置*/
		NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;		//选择配置NVIC的EXTI15_10线
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
		NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
}

