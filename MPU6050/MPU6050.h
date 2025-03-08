#ifndef __MPU6050_H
#define __MPU6050_H


void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
u8 mpu6050_read(u8 addr, u8 reg, u8 len, u8 *buf);
u8 mpu6050_write(u8 addr, u8 reg, u8 len, u8* buf);

u8 MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_Get_a(int16_t *AccX, int16_t *AccY, int16_t *AccZ);
void MPU6050_Get_g(int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
void Temp(int16_t *GyroX);	
	
u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
	
#endif
