#ifndef __MPU6050_H__
#define __MPU6050_H__	

#include "stm32f1xx.h"

#define I2C_SCL_CLK()      __HAL_RCC_GPIOB_CLK_ENABLE();
#define I2C_SCL_PORT       GPIOB
#define I2C_SCL_PIN        GPIO_PIN_8

#define I2C_SDA_CLK()      __HAL_RCC_GPIOB_CLK_ENABLE();
#define I2C_SDA_PORT       GPIOB
#define I2C_SDA_PIN        GPIO_PIN_9

#define I2C_SCL_RESET()        HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_RESET)
#define I2C_SCL_SET()          HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET)

#define I2C_SDA_RESET()        HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET)
#define I2C_SDA_SET()          HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET)

#define I2C_SDA_VALUE()        HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN)

#define MPU6050_SMPLRT_DIV      0x19
#define MPU6050_CONFIG          0x1A
#define MPU6050_GYRO_CONFIG     0x1B
#define MPU6050_ACCEL_CONFIG    0x1C
    
#define MPU6050_ACCEL_XOUT_H    0x3B
#define MPU6050_ACCEL_XOUT_L    0x3C
#define MPU6050_ACCEL_YOUT_H    0x3D
#define MPU6050_ACCEL_YOUT_L    0x3E
#define MPU6050_ACCEL_ZOUT_H    0x3F
#define MPU6050_ACCEL_ZOUT_L    0x40
#define MPU6050_TEMP_OUT_H      0x41
#define MPU6050_TEMP_OUT_L      0x42
#define MPU6050_GYRO_XOUT_H     0x43
#define MPU6050_GYRO_XOUT_L     0x44
#define MPU6050_GYRO_YOUT_H     0x45
#define MPU6050_GYRO_YOUT_L     0x46
#define MPU6050_GYRO_ZOUT_H     0x47
#define MPU6050_GYRO_ZOUT_L     0x48
    
#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_PWR_MGMT_2      0x6C
#define MPU6050_WHO_AM_I        0x75

void i2c_gpio_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_send_ack(uint8_t ack);
uint8_t i2c_receive_ack(void);
void i2c_send_byte(uint8_t data);
uint8_t i2c_read_byte(void);


void mpu6050_init(void);
void mpu6050_get_data(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                      int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
void mpu6050_write_byte(uint8_t address, uint8_t data);
uint8_t mpu6050_read_byte(uint8_t address);
void mpu6050_write_len(uint8_t address, uint8_t len, uint8_t *data);
void mpu6050_read_len(uint8_t address, uint8_t len, uint8_t *data);
#endif
