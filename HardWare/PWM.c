#include "stm32f10x.h"                  // Device header

/**
  * 函    数：PWM初始化
  * 参    数：无
  * 返 回 值：无
  */
  uint16_t ARR =  7200 -1;//设置PWM频率为10kHz
void PWM_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//开启TIM1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA2引脚初始化为复用推挽输出	
																	//受外设控制的引脚，均需要配置为复用模式
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM1);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR;                 //计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;               //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
	/*输出比较初始化*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体初始化，若结构体没有完整赋值
	                                                                //则最好执行此函数，给结构体所有成员都赋一个默认值
	                                                                //避免结构体初值不确定的问题
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//初始的CCR值
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC1Init，配置TIM1的输出比较通道1
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC4Init，配置TIM1的输出比较通道4
	/*TIM使能*/
	TIM_Cmd(TIM1, ENABLE);			//使能TIM1，定时器开始运行
	TIM_CtrlPWMOutputs(TIM1, ENABLE); // 启用高级定时器主输出
}

/**
  * 函    数：PWM设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：0~ARR
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);		//设置CCR4的值
}

/**
  * 函    数：PWM设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：0~ARR
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);		//设置CCR1的值
}

//**
//  * 函    数：PWM限幅（带硬限幅和抗饱和保护）
//  * 参    数：Motor_PWM_out_Left  左电机PWM输入（输入输出共用）
//  *         Motor_PWM_out_Right 右电机PWM输入（输入输出共用）
//  * 返 回 值：无
//  * 说    明：ARR=1800，安全限幅范围：±1500（保留300作为安全余量）

void PWM_Limit(int16_t *Motor_PWM_out_Left, int16_t *Motor_PWM_out_Right)
{
    /*------------------------ 参数定义 ------------------------*/
    const int16_t PWM_MAX = ARR+1; // 硬限幅最大值（ARR=1800时保留300余量）
		const int16_t PWM_MIN = - PWM_MAX;
    /*------------------------ 左电机处理 ------------------------*/
    
    // 正向硬限幅
    if (*Motor_PWM_out_Left > PWM_MAX) *Motor_PWM_out_Left = PWM_MAX;
		if (*Motor_PWM_out_Left < PWM_MIN) *Motor_PWM_out_Left = PWM_MIN;

    /*------------------------ 右电机处理（逻辑相同） ------------------------*/
		if (*Motor_PWM_out_Right > PWM_MAX) *Motor_PWM_out_Right = PWM_MAX;
		if (*Motor_PWM_out_Right < PWM_MIN) *Motor_PWM_out_Right = PWM_MIN;
}

