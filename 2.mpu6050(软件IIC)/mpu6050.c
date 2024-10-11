#include "mpu6050.h"
#include "delay.h"

void i2c_gpio_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;

    I2C_SCL_CLK();
    I2C_SDA_CLK();
    
    gpio_initstruct.Pin = I2C_SCL_PIN;          
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_OD;  //开漏输出                      
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;           
    HAL_GPIO_Init(I2C_SCL_PORT, &gpio_initstruct);
    
    gpio_initstruct.Pin = I2C_SDA_PIN;          
    HAL_GPIO_Init(I2C_SDA_PORT, &gpio_initstruct);
}

void i2c_start(void) //起始信号
{
	I2C_SDA_SET();
    I2C_SCL_SET();
    I2C_SDA_RESET(); //SDA产生下降沿
    I2C_SCL_RESET();
}

void i2c_stop(void) //终止信号
{
	I2C_SDA_RESET();
    I2C_SCL_SET();
    I2C_SDA_SET(); //SDA产生上升沿
}

void i2c_send_ack(uint8_t ack) //发送应答信号
{
	if(ack == 1)
		I2C_SDA_SET();
	else
		I2C_SDA_RESET();
	
    I2C_SCL_SET(); //从机在SCL为高电平时读取SDA上的信号
    I2C_SCL_RESET();
}

uint8_t i2c_receive_ack(void) //接受应答信号
{
	uint8_t ack;
	I2C_SDA_SET(); //释放sda线
	I2C_SCL_SET();
	ack = I2C_SDA_VALUE(); //获取sda的值
	I2C_SCL_RESET();
	return ack;
}


void i2c_send_byte(uint8_t data) //写一个字节数据
{
    uint8_t i, tmp;
    tmp = data;
    
    for(i = 0; i < 8; i++) 
    {
        if((tmp & 0x80) == 0x80)  //取出最高位，高位先行
            I2C_SDA_SET();
        else
            I2C_SDA_RESET();
        tmp = tmp << 1;     //左移
        I2C_SCL_SET();
        I2C_SCL_RESET();
    }
}

uint8_t i2c_read_byte(void) //读一个字节的数据
{
	uint8_t i = 0, data = 0;
	I2C_SDA_SET(); //释放sda线
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

/***********************************以上为软件IIC代码*******************************************/

void mpu6050_init(void)
{
	i2c_gpio_init();
	
	mpu6050_write_byte(MPU6050_PWR_MGMT_1, 0x01);           //电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
    mpu6050_write_byte(MPU6050_PWR_MGMT_2, 0x00);           //电源管理寄存器2，保持默认值0，所有轴均不待机
    mpu6050_write_byte(MPU6050_SMPLRT_DIV, 0x09);           //采样率分频寄存器，配置采样率
    mpu6050_write_byte(MPU6050_CONFIG, 0x06);               //配置寄存器，配置DLPF
    mpu6050_write_byte(MPU6050_GYRO_CONFIG, 0x18);          //陀螺仪配置寄存器，选择满量程为±2000°/s
    mpu6050_write_byte(MPU6050_ACCEL_CONFIG, 0x18);         //加速度计配置寄存器，选择满量程为±16g
}

void mpu6050_get_data(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                      int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;
    
    DataH = mpu6050_read_byte(MPU6050_ACCEL_XOUT_H);    //读取加速度计X轴的高8位数据
    DataL = mpu6050_read_byte(MPU6050_ACCEL_XOUT_L);    //读取加速度计X轴的低8位数据
    *AccX = (DataH << 8) | DataL;                           //数据拼接，通过输出参数返回
    
    DataH = mpu6050_read_byte(MPU6050_ACCEL_YOUT_H);    //读取加速度计Y轴的高8位数据
    DataL = mpu6050_read_byte(MPU6050_ACCEL_YOUT_L);    //读取加速度计Y轴的低8位数据
    *AccY = (DataH << 8) | DataL;                           //数据拼接，通过输出参数返回
    
    DataH = mpu6050_read_byte(MPU6050_ACCEL_ZOUT_H);    //读取加速度计Z轴的高8位数据
    DataL = mpu6050_read_byte(MPU6050_ACCEL_ZOUT_L);    //读取加速度计Z轴的低8位数据
    *AccZ = (DataH << 8) | DataL;                           //数据拼接，通过输出参数返回
    
    DataH = mpu6050_read_byte(MPU6050_GYRO_XOUT_H);     //读取陀螺仪X轴的高8位数据
    DataL = mpu6050_read_byte(MPU6050_GYRO_XOUT_L);     //读取陀螺仪X轴的低8位数据
    *GyroX = (DataH << 8) | DataL;                          //数据拼接，通过输出参数返回
    
    DataH = mpu6050_read_byte(MPU6050_GYRO_YOUT_H);     //读取陀螺仪Y轴的高8位数据
    DataL = mpu6050_read_byte(MPU6050_GYRO_YOUT_L);     //读取陀螺仪Y轴的低8位数据
    *GyroY = (DataH << 8) | DataL;                          //数据拼接，通过输出参数返回
    
    DataH = mpu6050_read_byte(MPU6050_GYRO_ZOUT_H);     //读取陀螺仪Z轴的高8位数据
    DataL = mpu6050_read_byte(MPU6050_GYRO_ZOUT_L);     //读取陀螺仪Z轴的低8位数据
    *GyroZ = (DataH << 8) | DataL;                          //数据拼接，通过输出参数返回
}

void mpu6050_write_byte(uint8_t address, uint8_t data)
{
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //写入mpu6050地址
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
	i2c_send_byte((0x68 << 1) | 0x00); //写入mpu6050地址
	i2c_receive_ack();
	i2c_send_byte(address); 
	i2c_receive_ack();
	
	i2c_start(); //上面读写位为写，下面改成读
	i2c_send_byte((0x68 << 1) | 0x01);
	i2c_receive_ack();
	data = i2c_read_byte();
	i2c_send_ack(1); //非应答，停止读取
	i2c_stop();
	return data;
}

void mpu6050_write_len(uint8_t address, uint8_t len, uint8_t *data) //连续写多个字节
{
	int i = 0;
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //写入mpu6050地址
	i2c_receive_ack();
	i2c_send_byte(address);  //指定寄存器
	i2c_receive_ack();
	for(i = 0; i < len; i++) //读取多个字节数据
	{
		i2c_send_byte(data[i]); 
		i2c_receive_ack();
	}
	i2c_stop();
}

void mpu6050_read_len(uint8_t address, uint8_t len, uint8_t *data)//连续读多个字节
{
	int i = 0; //指定读取的寄存器位置
	i2c_start();
	i2c_send_byte((0x68 << 1) | 0x00); //写入mpu6050地址
	i2c_receive_ack();
	i2c_send_byte(address); 
	i2c_receive_ack();
	
	i2c_start(); //读取寄存器的值
	i2c_send_byte((0x68 << 1) | 0x01);
	i2c_receive_ack();
	for(i = 0; i < len; i++)
	{
		data[i] = i2c_read_byte();
		if(i == len - 1)
			i2c_send_ack(1); //非应答，停止读取
		else
			i2c_send_ack(0); //应答，继续读取
	    
	}
	i2c_stop();
	
}
