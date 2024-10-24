#ifndef __W25Q64_H__
#define __W25Q64_H__

#include "stm32f1xx.h"
#define W25Q64_CS(x)   x ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET)

/* 指令表 */
#define FLASH_ManufactDeviceID                  0x90
#define FLASH_WriteEnable                       0x06
#define FLASH_ReadStatusReg1                    0x05
#define FLASH_ReadData                          0x03
#define FLASH_PageProgram                       0x02
#define FLASH_SectorErase                       0x20
#define FLASH_DummyByte                         0xFF

void w25q64_init(void);
uint16_t w25q64_read_id(void);
void w25q64_send_address(uint32_t address);
void w25q64_read_data(uint32_t address, uint8_t *data, uint8_t size); //读取数据
void w25q64_send_address(uint32_t address); //发送地址
void w25q64_write_page(uint32_t address, uint8_t *data, uint8_t size); //页写
void w25q64_erase_sector(uint32_t address); //扇区擦除
#endif

