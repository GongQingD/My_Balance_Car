#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// �޸����Ŷ��岿��
void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)BitValue); // PB4��ΪSCL
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)BitValue); // PB3��ΪSDA
    Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3); // ��ȡPB3
    return BitValue;
}

void MyI2C_Init(void)
{
    // ʹ��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    
    // ����JTAG���ܣ��ͷ�PB3��PB4
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ����SCL (PB4) �� SDA (PB3) Ϊ��©���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // ��ʼ��������
    GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4);
}

/* ����I2CЭ�麯�����ֲ��� */
void MyI2C_Start(void) 
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t i;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    while(MyI2C_R_SDA())
    {
        i++;
        if(i>250)
        {
            MyI2C_Stop();
            return 1;
        }
    }
    MyI2C_W_SCL(0);
    return 0;
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(u8 Ack)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i ++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA()){Byte |= (0x80 >> i);}
        MyI2C_W_SCL(0);
    }
    MyI2C_SendAck(Ack);
    return Byte;
}
