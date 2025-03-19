#include "stm32f10x.h"                  // Device header
#include "Sys_Init.h"
/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  * 功    能：设置直流电机转向与电机速度
  */
void Motor_Init(void)
{
	/*开启时钟，初始化电机驱动模块的方向引脚AIN1，AIN2，BIN1，BIN2*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PA4和PA5引脚初始化为推挽输出	
}

/**
  * 函    数：两个直流电机设置速度
  * 参    数：左右电机的CCR值
  * 返 回 值：无
  */
void Motor_SetSpeed(int16_t Motor_PWM_out_Left, int16_t Motor_PWM_out_Right)
{
	/*******************************左电机*****************************************/
	if (Motor_PWM_out_Left >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_13);	//PB13置高电平（AIN1=1）
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);	//PB12置低电平，设置方向为正转（AIN2=0）
		PWM_SetCompare4(Motor_PWM_out_Left);				//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);	//PB13置低电平（AIN1=0）
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	//PB12置高电平，设置方向为反转（AIN2=1）
		PWM_SetCompare4(-Motor_PWM_out_Left);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
	/*******************************右电机*****************************************/
	if (Motor_PWM_out_Right >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	//PB14置高电平（BIN1=1）
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	//PB15置低电平，设置方向为正转（BIN2=0）
		PWM_SetCompare1(Motor_PWM_out_Right);				//PWM设置为速度值
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	//PB14置低电平（BIN1=0）
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	//PB15置高电平，设置方向为反转（BIN2=1）
		PWM_SetCompare1(-Motor_PWM_out_Right);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
}
