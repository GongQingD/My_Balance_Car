#include "Control.h"
#include "Sys_Init.h"
float pitc=0, rol=0, ya=0;
int8_t B=0, sum_test = 0;
int16_t Motor_Encoder_Left, Motor_Encoder_Right, Motor_PWM_out_Left, Motor_PWM_out_Right;
int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;//MPU6050的加速度和角加速度
float Vertical_Error = 0;
int16_t DeadZone_Compensator = 390;//死区补偿
/***********************///需要设置的值
struct PID_param {
	float Velocity_Kp;//速度环p
	float Velocity_Ki;//速度环i
	float Vertical_Kp;//直立环p
	float Vertical_Kd;//直立环d
	float Turn_Kp;    //转向环p
}PID_Parameter;
/***********************///需要设置的值

struct PID_param PID_Parameter = {1, 0.005, 100, 0, 0.6};//定义PID的参数
float Car_Median = -0.5;//小车的机械中值，小车平衡时的roll


/**
  * 函    数：速度环PID
  * 参    数：Target：Motor_Encoder_Left（左电机编码器值），Motor_Encoder_Right（右电机编码器值）
  * 返 回 值：小车的期望角度（直立环的输入）
  */
//int16_t Velocity(int16_t Motor_Encoder_Left, int16_t Motor_Encoder_Right)
//{
//	int16_t PWM_Out, Motor_Encoder_Err, Motor_Encoder_Lowout;
//	static int16_t Motor_Encoder_S, Motor_EnC_Err_Lowout_last;//Encoder_S积分累积值，需要持续保存，EnC_Err_Lowout_last低通滤波公式要用到
//	float a = 0.7; //低通滤波系数
//	
//	// 1.计算速度偏差
//	Motor_Encoder_Err = (Motor_Encoder_Left + Motor_Encoder_Right) - 0;//直立时的速度为0，因此期望速度是0
//	// 2.对速度偏差进行低通滤波
//	Motor_Encoder_Lowout = (1 - a) * Motor_Encoder_Err + a * Motor_EnC_Err_Lowout_last; // 使得波形更加平滑，滤除高频干扰，放置速度突变
//    Motor_EnC_Err_Lowout_last = Motor_Encoder_Lowout;   // 防止速度过大影响直立环的正常工作
//	// 3.对速度偏差积分
//	Motor_Encoder_S += Motor_Encoder_Lowout;
//	// 4.积分限幅
//	Motor_Encoder_S = 
//			Motor_Encoder_S > 10000 ? 10000:			// 如果值 > 10000，设为10000
//			(Motor_Encoder_S < (-10000) ? (-10000):		// 如果值 < -10000，设为-10000
//			Motor_Encoder_S);							// 否则保持原值
//	if (rol < -40 || rol >40)  Motor_Encoder_S = 0;
//	 // 5.速度环控制输出
//	PWM_Out = PID_Parameter.Velocity_Kp * Motor_Encoder_Lowout+PID_Parameter.Velocity_Ki * Motor_Encoder_S;	
//	
//	return PWM_Out;
//}

/**
  * 函    数：直立环PID
  * 参    数：Car_Median小车的机械中值，roll倾角，GyroY，Y轴角速度
  * 返 回 值：电机的PWM输入
  */
int16_t Vertical(float Car_Median,float roll, int16_t GyroX) 
{
	int16_t PWM_Out = 0;
	Vertical_Error = roll - Car_Median;
	PWM_Out = PID_Parameter.Vertical_Kp * Vertical_Error + PID_Parameter.Vertical_Kd * (GyroX - 0);

	
	return PWM_Out;
}

///**
//  * 函    数：转向环PID
//  * 参    数：Car_Median小车的机械中值，pitch倾角，GyroY，Y轴角速度
//  * 返 回 值：
//  */
//int16_t Turn(int16_t GyroZ)
//{
//	int16_t PWM_Out;
//	
//	PWM_Out = PID_Parameter.Turn_Kp * GyroZ;
//	
//	return PWM_Out;
//}



/**
  * 函    数：主控函数，PID算法设计（直立环，速度环，转向环）
  * 参    数：无
  * 返 回 值：无
  */
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断5号线触发的中断
	{
		EXTI_ClearITPendingBit(EXTI_Line5);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
		int16_t PWM_out, Velocity_out, Vertical_out, Turn_out;		//串级PID输出
//		// 1.采集编码器数据&MPU6050角度信息
//       // 电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反
			Motor_Encoder_Left =  -Encoder_Get2();						//每隔固定时间段（MPU6050采样间隔设置的是5ms）读取一次编码器计数增量值，这个增量值为脉冲数
			Motor_Encoder_Right = Encoder_Get4();
			B = MPU6050_DMP_Get_Data(&pitc,&rol,&ya);
			MPU6050_Get_a(&AccX, &AccY, &AccZ);							//读取加速度
			MPU6050_Get_g(&GyroX, &GyroY, &GyroZ);						//读取角速度
		
		// 2.利用PID控制器得到控制输出量
		//Velocity_out = Velocity(Motor_Encoder_Left, Motor_Encoder_Right); // 速度环
		Vertical_out = Vertical(Car_Median, rol, GyroX); //直立环
		// 直立环的输入为Velocity_out + Car_Median，因为直立环的目标角度需要被调整为机械中值加上速度环的输出。
		//这样，当速度环有输出时，直立环的目标角度会改变，导致小车倾斜，从而产生加速度，达到速度控制的目的。
		//Turn_out = Turn(GyroZ);	转向环
		//PWM_out = Velocity_out;//速度环测试
	
		if (Vertical_out>0)PWM_out=Vertical_out + DeadZone_Compensator;
		if (Vertical_out<0)PWM_out=Vertical_out - DeadZone_Compensator;
		
		
		// 3.把控制输出量加载到电机上，完成最终控制
		Motor_PWM_out_Left = PWM_out ; // 左电机
		Motor_PWM_out_Right = PWM_out; // 右电机
		

		//限幅
		PWM_Limit(&Motor_PWM_out_Left,&Motor_PWM_out_Right);
		
		Serial_Printf("%.2f,%d,%d,%d,%d,%d\r\n", rol, GyroX, GyroY, Vertical_Error, Motor_PWM_out_Left, Motor_PWM_out_Right);
		//把PWM输入电机
		Motor_SetSpeed(Motor_PWM_out_Left,Motor_PWM_out_Right);

		
	}
}

