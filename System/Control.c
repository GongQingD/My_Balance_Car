#include "Control.h"
#include "Sys_Init.h"
float pitc=0, rol=0, ya=0;
int8_t B=0, sum_test = 0;
int16_t Motor_Encoder_Left, Motor_Encoder_Right;
int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;//MPU6050�ļ��ٶȺͽǼ��ٶ�
/***********************///��Ҫ���õ�ֵ
struct PID_param {
	int8_t Velocity_Kp;//�ٶȻ�p
	int8_t Velocity_Ki;//�ٶȻ�i
	int8_t Vertical_Kp;//ֱ����p
	int8_t Vertical_Kd;//ֱ����d
	int8_t Turn_Kp;    //ת��p
}PID_Parameter;
/***********************///��Ҫ���õ�ֵ
struct PID_param PID_Parameter = {1, 1, 1, 1, 1};//����PID�Ĳ���
int8_t Car_Median = 0;//С���Ļ�е��ֵ��С��ƽ��ʱ��pitch


/**
  * ��    �����ٶȻ�PID
  * ��    ����Target��Motor_Encoder_Left������������ֵ����Motor_Encoder_Right���ҵ��������ֵ��
  * �� �� ֵ��С���������Ƕȣ�ֱ���������룩
  */
int16_t Velocity(int16_t Motor_Encoder_Left, int16_t Motor_Encoder_Right)
{
	int16_t PWM_Out, Motor_Encoder_Err, Motor_Encoder_Lowout;
	static int16_t Motor_Encoder_S, Motor_EnC_Err_Lowout_last;//Encoder_S�����ۻ�ֵ����Ҫ�������棬EnC_Err_Lowout_last��ͨ�˲���ʽҪ�õ�
	float a = 0.7; //��ͨ�˲�ϵ��
	
	// 1.�����ٶ�ƫ��
	Motor_Encoder_Err = (Motor_Encoder_Left + Motor_Encoder_Right) - 0;//ֱ��ʱ���ٶ�Ϊ0����������ٶ���0
	// 2.���ٶ�ƫ����е�ͨ�˲�
	Motor_Encoder_Lowout = (1 - a) * Motor_Encoder_Err + a * Motor_EnC_Err_Lowout_last; // ʹ�ò��θ���ƽ�����˳���Ƶ���ţ������ٶ�ͻ��
    Motor_EnC_Err_Lowout_last = Motor_Encoder_Lowout;   // ��ֹ�ٶȹ���Ӱ��ֱ��������������
	// 3.���ٶ�ƫ�����
	Motor_Encoder_S += Motor_Encoder_Lowout;
	// 4.�����޷�
	Motor_Encoder_S = 
			Motor_Encoder_S > 10000 ? 10000:			// ���ֵ > 10000����Ϊ10000
			(Motor_Encoder_S < (-10000) ? (-10000):		// ���ֵ < -10000����Ϊ-10000
			Motor_Encoder_S);							// ���򱣳�ԭֵ
	 // 5.�ٶȻ��������
	PWM_Out = PID_Parameter.Velocity_Kp * Motor_Encoder_Lowout+PID_Parameter.Velocity_Ki * Motor_Encoder_S;	
	
	return PWM_Out;
}

/**
  * ��    ����ֱ����PID
  * ��    ����Car_MedianС���Ļ�е��ֵ��pitch��ǣ�GyroY��Y����ٶ�
  * �� �� ֵ�������PWM����
  */
int16_t Vertical(uint16_t Car_Median,float pitch, int16_t GyroY) 
{
	int16_t PWM_Out;
	
	PWM_Out = -PID_Parameter.Vertical_Kp * (pitch - Car_Median) + PID_Parameter.Vertical_Kd * (GyroY - 0);
	
	return PWM_Out;
}

/**
  * ��    ����ת��PID
  * ��    ����Car_MedianС���Ļ�е��ֵ��pitch��ǣ�GyroY��Y����ٶ�
  * �� �� ֵ��
  */
int16_t Turn(int16_t GyroZ)
{
	int16_t PWM_Out;
	
	PWM_Out = PID_Parameter.Turn_Kp * GyroZ;
	
	return PWM_Out;
}





















/**
  * ��    �������غ�����PID�㷨��ƣ�ֱ�������ٶȻ���ת�򻷣�
  * ��    ������
  * �� �� ֵ����
  */
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)		//�ж��Ƿ����ⲿ�ж�5���ߴ������ж�
	{
		int16_t PWM_out, Velocity_out, Vertical_out, Turn_out, Motor_PWM_out_Left, Motor_PWM_out_Right;		//����PID���
		// 1.�ɼ�����������&MPU6050�Ƕ���Ϣ
       // �������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��
		Motor_Encoder_Left = -Encoder_Get2();						//ÿ���̶�ʱ��Σ�MPU6050����������õ���5ms����ȡһ�α�������������ֵ���������ֵΪ������
		Motor_Encoder_Right = Encoder_Get4();
		B = MPU6050_DMP_Get_Data(&pitc,&rol,&ya);
		MPU6050_Get_a(&AccX, &AccY, &AccZ);							//��ȡ���ٶ�
		MPU6050_Get_g(&GyroX, &GyroY, &GyroZ);						//��ȡ���ٶ�
		
		// 2.����PID�������õ����������
		Velocity_out = Velocity(Motor_Encoder_Left, Motor_Encoder_Right); // �ٶȻ�
		Vertical_out = Vertical(Velocity_out + Car_Median, pitc, GyroY); 
		// ֱ����������ΪVelocity_out + Car_Median����Ϊֱ������Ŀ��Ƕ���Ҫ������Ϊ��е��ֵ�����ٶȻ��������
		//���������ٶȻ������ʱ��ֱ������Ŀ��ǶȻ�ı䣬����С����б���Ӷ��������ٶȣ��ﵽ�ٶȿ��Ƶ�Ŀ�ġ�
		Turn_out = Turn(GyroZ);	

		PWM_out = Vertical_out;//�������
		
		// 3.�ѿ�����������ص�����ϣ�������տ���
		Motor_PWM_out_Left = PWM_out - Turn_out; // ����
		Motor_PWM_out_Right = PWM_out + Turn_out; // �ҵ��
		
		//�޷�
		PWM_Limit(&Motor_PWM_out_Left,&Motor_PWM_out_Right);
		
		//��PWM������
		Motor_SetSpeed(Motor_PWM_out_Left,Motor_PWM_out_Right);
		
		
		
		
		
		
		
		
		
		EXTI_ClearITPendingBit(EXTI_Line5);		//����ⲿ�ж�14���ߵ��жϱ�־λ
													//�жϱ�־λ�������
													//�����жϽ��������ϵش�����������������
	}
}

