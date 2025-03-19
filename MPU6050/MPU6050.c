#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "Delay.h"



/**************************
ָ����ַд
***************************/
uint8_t MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU_ADDR<<1|0);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	if(MyI2C_ReceiveAck())
		{
			MyI2C_Stop();
			return 1;	
		}
	MyI2C_SendByte(Data);
	if(MyI2C_ReceiveAck())
		{
			MyI2C_Stop();
			return 1;	
		}
	MyI2C_Stop();
	return 0;
}


/*************************
ָ����ַ��
***************************/
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU_ADDR<<1|0);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU_ADDR<<1 | 1);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte(1);
	//MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

/*************************
����MPU6050�����ֵ�ͨ�˲���
lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
����ֵ:0,���óɹ�
    ����,����ʧ�� 
***************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU6050_WriteReg(MPU_CFG_REG, data);//�������ֵ�ͨ�˲���  
}


/*************************
����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
rate:4~1000(Hz)
����ֵ:0,���óɹ�
    ����,����ʧ�� 
***************************/
u8 MPU_Set_Rate(int16_t Rate)
{
	u8 data;
	if(Rate > 1000) Rate = 1000;
	if(Rate < 4)Rate = 4;
	data = 1000 / (Rate - 1);
	data = MPU6050_WriteReg(MPU_SAMPLE_RATE_REG, data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(Rate / 2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}


/*************************
MPU6050��ʼ��
***************************/
u8 MPU6050_Init(void)
{
//	u8 res; 
	MyI2C_Init();
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG, 0X80);	//��λMPU6050
  Delay_ms(100);
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG, 0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(200);						//���ò�����50Hz
	MPU6050_WriteReg(MPU_INT_EN_REG,0X00);	//�ر������ж�
	MPU6050_WriteReg(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU6050_WriteReg(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU6050_WriteReg(MPU_INTBP_CFG_REG,0X00);	//INT���Ÿߵ�ƽ��Ч
	MPU6050_WriteReg(MPU_INT_EN_REG, 0x01);//ʹ�����ݾ����ж�
//	res = MPU6050_ReadReg(MPU_DEVICE_ID_REG); 
//	if(res==MPU_ADDR)//����ID��ȷ
//	{
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0X02);	//����CLKSEL,PLL X��Ϊ�ο�
	MPU6050_WriteReg(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
	MPU_Set_Rate(100);						//���ò�����Ϊ50Hz
// 	}else return 1;
	return 0;
}




/*************************
��ȡMPU6050��ID
***************************/
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU_DEVICE_ID_REG);
}




/*************************
��ȡ���ٶȼ�����
***************************/
void MPU6050_Get_a(int16_t *AccX, int16_t *AccY, int16_t *AccZ 	)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU_ACCEL_XOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_ACCEL_XOUTL_REG);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU_ACCEL_YOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_ACCEL_YOUTL_REG);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU_ACCEL_ZOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_ACCEL_ZOUTL_REG);
	*AccZ = (DataH << 8) | DataL;
}


/*************************
��ȡ����������
***************************/
void MPU6050_Get_g(int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	DataH = MPU6050_ReadReg(MPU_GYRO_XOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_GYRO_XOUTL_REG);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU_GYRO_YOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_GYRO_YOUTL_REG);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU_GYRO_ZOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_GYRO_ZOUTL_REG);
	*GyroZ = (DataH << 8) | DataL;

}


/***********************
�������������Խ�dmp�Ķ�д
**************************/
u8 mpu6050_write(u8 addr, u8 reg, u8 len, u8* buf)//����ֵ 0�����ɹ�  -1����ʧ��
{ 
		 unsigned char i;
		 MyI2C_Start();              //��������
		 MyI2C_SendByte(addr<<1|0);      //����������ַ        
		MyI2C_ReceiveAck();
		 MyI2C_SendByte(reg);       //���������ӵ�ַ
		MyI2C_ReceiveAck();
		 for(i=0;i<len;i++)
{
	 MyI2C_SendByte(*buf++);  //��������
	MyI2C_ReceiveAck();
}	
		
		 MyI2C_Stop();               //��������

		 return 0;
}

u8 mpu6050_read(u8 addr, u8 reg, u8 len, u8 *buf)//����ֵ 0�����ɹ�  -1����ʧ��
{
		 unsigned char i;
		 MyI2C_Start();              //��������           
		 MyI2C_SendByte(addr<<1|0);      //����������ַ 
		MyI2C_ReceiveAck();
		 MyI2C_SendByte(reg);       //���������ӵ�ַ
		MyI2C_ReceiveAck();
	
		 MyI2C_Start();              //������������
		 MyI2C_SendByte(addr<<1|1);
		MyI2C_ReceiveAck();
		 for(i=0;i<len-1;i++) 
	{
		 *buf++=MyI2C_ReceiveByte(0);//��������
	}
			
		 *buf=MyI2C_ReceiveByte(1);
		 MyI2C_Stop();               //��������
		 return 0;
}


