#include "mpu6050.h"
#include "delay.h"

I2C_HandleTypeDef hi2c1;
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_I2C1_CLK_ENABLE();

  }
}

/***********************************����ΪӲ��IIC��ʼ�����ô���*******************************************/

void mpu6050_init(void)
{
	
	mpu6050_write_byte(MPU6050_PWR_MGMT_1, 0x01);           //��Դ����Ĵ���1��ȡ��˯��ģʽ��ѡ��ʱ��ԴΪX��������
    mpu6050_write_byte(MPU6050_PWR_MGMT_2, 0x00);           //��Դ����Ĵ���2������Ĭ��ֵ0���������������
    mpu6050_write_byte(MPU6050_SMPLRT_DIV, 0x09);           //�����ʷ�Ƶ�Ĵ��������ò�����
    mpu6050_write_byte(MPU6050_CONFIG, 0x06);               //���üĴ���������DLPF
    mpu6050_write_byte(MPU6050_GYRO_CONFIG, 0x18);          //���������üĴ�����ѡ��������Ϊ��2000��/s
    mpu6050_write_byte(MPU6050_ACCEL_CONFIG, 0x18);         //���ٶȼ����üĴ�����ѡ��������Ϊ��16g
}

void mpu6050_get_data(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                      int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;
    
    DataH = mpu6050_read_byte(MPU6050_ACCEL_XOUT_H);    //��ȡ���ٶȼ�X��ĸ�8λ����
    DataL = mpu6050_read_byte(MPU6050_ACCEL_XOUT_L);    //��ȡ���ٶȼ�X��ĵ�8λ����
    *AccX = (DataH << 8) | DataL;                           //����ƴ�ӣ�ͨ�������������
    
    DataH = mpu6050_read_byte(MPU6050_ACCEL_YOUT_H);    //��ȡ���ٶȼ�Y��ĸ�8λ����
    DataL = mpu6050_read_byte(MPU6050_ACCEL_YOUT_L);    //��ȡ���ٶȼ�Y��ĵ�8λ����
    *AccY = (DataH << 8) | DataL;                           //����ƴ�ӣ�ͨ�������������
    
    DataH = mpu6050_read_byte(MPU6050_ACCEL_ZOUT_H);    //��ȡ���ٶȼ�Z��ĸ�8λ����
    DataL = mpu6050_read_byte(MPU6050_ACCEL_ZOUT_L);    //��ȡ���ٶȼ�Z��ĵ�8λ����
    *AccZ = (DataH << 8) | DataL;                           //����ƴ�ӣ�ͨ�������������
    
    DataH = mpu6050_read_byte(MPU6050_GYRO_XOUT_H);     //��ȡ������X��ĸ�8λ����
    DataL = mpu6050_read_byte(MPU6050_GYRO_XOUT_L);     //��ȡ������X��ĵ�8λ����
    *GyroX = (DataH << 8) | DataL;                          //����ƴ�ӣ�ͨ�������������
    
    DataH = mpu6050_read_byte(MPU6050_GYRO_YOUT_H);     //��ȡ������Y��ĸ�8λ����
    DataL = mpu6050_read_byte(MPU6050_GYRO_YOUT_L);     //��ȡ������Y��ĵ�8λ����
    *GyroY = (DataH << 8) | DataL;                          //����ƴ�ӣ�ͨ�������������
    
    DataH = mpu6050_read_byte(MPU6050_GYRO_ZOUT_H);     //��ȡ������Z��ĸ�8λ����
    DataL = mpu6050_read_byte(MPU6050_GYRO_ZOUT_L);     //��ȡ������Z��ĵ�8λ����
    *GyroZ = (DataH << 8) | DataL;                          //����ƴ�ӣ�ͨ�������������
}

void mpu6050_write_byte(uint8_t address, uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1, 0x68 << 1, address, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

uint8_t mpu6050_read_byte(uint8_t address)
{
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c1, 0x68 << 1, address, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	return data;
}

void mpu6050_write_len(uint8_t address, uint8_t len, uint8_t *data) //����д����ֽ�
{

	HAL_I2C_Mem_Write(&hi2c1, 0x68 << 1, address, I2C_MEMADD_SIZE_8BIT, data, len, 100);
}

void mpu6050_read_len(uint8_t address, uint8_t len, uint8_t *data)//����������ֽ�
{
	HAL_I2C_Mem_Read(&hi2c1, 0x68 << 1, address, I2C_MEMADD_SIZE_8BIT, data, len, 100);
}
