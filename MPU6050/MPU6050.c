#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "Delay.h"



/**************************
指定地址写
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
指定地址读
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
设置MPU6050的数字低通滤波器
lpf:数字低通滤波频率(Hz)
返回值:0,设置成功
    其他,设置失败 
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
	return MPU6050_WriteReg(MPU_CFG_REG, data);//设置数字低通滤波器  
}


/*************************
设置MPU6050的采样率(假定Fs=1KHz)
rate:4~1000(Hz)
返回值:0,设置成功
    其他,设置失败 
***************************/
u8 MPU_Set_Rate(int16_t Rate)
{
	u8 data;
	if(Rate > 1000) Rate = 1000;
	if(Rate < 4)Rate = 4;
	data = 1000 / (Rate - 1);
	data = MPU6050_WriteReg(MPU_SAMPLE_RATE_REG, data);	//设置数字低通滤波器
 	return MPU_Set_LPF(Rate / 2);	//自动设置LPF为采样率的一半
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU6050_WriteReg(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}


/*************************
MPU6050初始化
***************************/
u8 MPU6050_Init(void)
{
//	u8 res; 
	MyI2C_Init();
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG, 0X80);	//复位MPU6050
  Delay_ms(100);
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG, 0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(200);						//设置采样率50Hz
	MPU6050_WriteReg(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU6050_WriteReg(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU6050_WriteReg(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU6050_WriteReg(MPU_INTBP_CFG_REG,0X00);	//INT引脚高电平有效
	MPU6050_WriteReg(MPU_INT_EN_REG, 0x01);//使能数据就绪中断
//	res = MPU6050_ReadReg(MPU_DEVICE_ID_REG); 
//	if(res==MPU_ADDR)//器件ID正确
//	{
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0X02);	//设置CLKSEL,PLL X轴为参考
	MPU6050_WriteReg(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
	MPU_Set_Rate(100);						//设置采样率为50Hz
// 	}else return 1;
	return 0;
}




/*************************
获取MPU6050的ID
***************************/
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU_DEVICE_ID_REG);
}




/*************************
获取加速度计数据
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
获取陀螺仪数据
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
以下两个函数对接dmp的读写
**************************/
u8 mpu6050_write(u8 addr, u8 reg, u8 len, u8* buf)//返回值 0：读成功  -1：读失败
{ 
		 unsigned char i;
		 MyI2C_Start();              //启动总线
		 MyI2C_SendByte(addr<<1|0);      //发送器件地址        
		MyI2C_ReceiveAck();
		 MyI2C_SendByte(reg);       //发送器件子地址
		MyI2C_ReceiveAck();
		 for(i=0;i<len;i++)
{
	 MyI2C_SendByte(*buf++);  //发送数据
	MyI2C_ReceiveAck();
}	
		
		 MyI2C_Stop();               //结束总线

		 return 0;
}

u8 mpu6050_read(u8 addr, u8 reg, u8 len, u8 *buf)//返回值 0：读成功  -1：读失败
{
		 unsigned char i;
		 MyI2C_Start();              //启动总线           
		 MyI2C_SendByte(addr<<1|0);      //发送器件地址 
		MyI2C_ReceiveAck();
		 MyI2C_SendByte(reg);       //发送器件子地址
		MyI2C_ReceiveAck();
	
		 MyI2C_Start();              //重新启动总线
		 MyI2C_SendByte(addr<<1|1);
		MyI2C_ReceiveAck();
		 for(i=0;i<len-1;i++) 
	{
		 *buf++=MyI2C_ReceiveByte(0);//发送数据
	}
			
		 *buf=MyI2C_ReceiveByte(1);
		 MyI2C_Stop();               //结束总线
		 return 0;
}


