#include "mpu6050.h"
#include "delay.h"

void i2c_gpio_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;

    I2C_SCL_CLK();
    I2C_SDA_CLK();
    
    gpio_initstruct.Pin = I2C_SCL_PIN;          
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_OD;  //��©���                      
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;           
    HAL_GPIO_Init(I2C_SCL_PORT, &gpio_initstruct);
    
    gpio_initstruct.Pin = I2C_SDA_PIN;          
    HAL_GPIO_Init(I2C_SDA_PORT, &gpio_initstruct);
}

void i2c_start(void) //��ʼ�ź�
{
	I2C_SDA_SET();
    I2C_SCL_SET();
    I2C_SDA_RESET(); //SDA�����½���
    I2C_SCL_RESET();
}

void i2c_stop(void) //��ֹ�ź�
{
	I2C_SDA_RESET();
    I2C_SCL_SET();
    I2C_SDA_SET(); //SDA����������
}

void i2c_send_ack(uint8_t ack) //����Ӧ���ź�
{
	if(ack == 1)
		I2C_SDA_SET();
	else
		I2C_SDA_RESET();
	
    I2C_SCL_SET(); //�ӻ���SCLΪ�ߵ�ƽʱ��ȡSDA�ϵ��ź�
    I2C_SCL_RESET();
}

uint8_t i2c_receive_ack(void) //����Ӧ���ź�
{
	uint8_t ack;
	I2C_SDA_SET(); //�ͷ�sda��
	I2C_SCL_SET();
	ack = I2C_SDA_VALUE(); //��ȡsda��ֵ
	I2C_SCL_RESET();
	return ack;
}


void i2c_send_byte(uint8_t data) //дһ���ֽ�����
{
    uint8_t i, tmp;
    tmp = data;
    
    for(i = 0; i < 8; i++) 
    {
        if((tmp & 0x80) == 0x80)  //ȡ�����λ����λ����
            I2C_SDA_SET();
        else
            I2C_SDA_RESET();
        tmp = tmp << 1;     //����
        I2C_SCL_SET();
        I2C_SCL_RESET();
    }
}

uint8_t i2c_read_byte(void) //��һ���ֽڵ�����
{
	uint8_t i = 0, data = 0;
	I2C_SDA_SET(); //�ͷ�sda��
	for(i = 0; i < 8;i++)
	{
		I2C_SCL_SET();
	    if(I2C_SDA_VALUE() == 1)
		{
			data |= 0x80 >> i;
		}
	    I2C_SCL_RESET();
	}
	return data;
}

/***********************************����Ϊ���IIC����*******************************************/

void mpu6050_init(void)
{
	i2c_gpio_init();
	
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
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //д��mpu6050��ַ
	i2c_receive_ack();
	i2c_send_byte(address); 
	i2c_receive_ack();
	i2c_send_byte(data); 
	i2c_receive_ack();
	i2c_stop();
	
}

uint8_t mpu6050_read_byte(uint8_t address)
{
	uint8_t data = 0;
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //д��mpu6050��ַ
	i2c_receive_ack();
	i2c_send_byte(address); 
	i2c_receive_ack();
	
	i2c_start(); //�����дλΪд������ĳɶ�
	i2c_send_byte((0x68 << 1) | 0x01);
	i2c_receive_ack();
	data = i2c_read_byte();
	i2c_send_ack(1); //��Ӧ��ֹͣ��ȡ
	i2c_stop();
	return data;
}

void mpu6050_write_len(uint8_t address, uint8_t len, uint8_t *data) //����д����ֽ�
{
	int i = 0;
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //д��mpu6050��ַ
	i2c_receive_ack();
	i2c_send_byte(address);  //ָ���Ĵ���
	i2c_receive_ack();
	for(i = 0; i < len; i++) //��ȡ����ֽ�����
	{
		i2c_send_byte(data[i]); 
		i2c_receive_ack();
	}
	i2c_stop();
}

void mpu6050_read_len(uint8_t address, uint8_t len, uint8_t *data)//����������ֽ�
{
	int i = 0; //ָ����ȡ�ļĴ���λ��
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //д��mpu6050��ַ
	i2c_receive_ack();
	i2c_send_byte(address); 
	i2c_receive_ack();
	
	i2c_start(); //��ȡ�Ĵ�����ֵ
	i2c_send_byte((0x68 << 1) | 0x01);
	i2c_receive_ack();
	for(i = 0; i < len; i++)
	{
		data[i] = i2c_read_byte();
		if(i == len - 1)
			i2c_send_ack(1); //��Ӧ��ֹͣ��ȡ
		else
			i2c_send_ack(0); //Ӧ�𣬼�����ȡ
	    
	}
	i2c_stop();
	
}
