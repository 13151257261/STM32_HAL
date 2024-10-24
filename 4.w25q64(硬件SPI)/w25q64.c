#include "w25q64.h"

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_4;  //gpio pin 4为软件配置cs片选引脚
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}
/******************************以上为硬件SPI配置代码**********************************/

uint8_t spi_swap_byte(uint8_t data)
{
	uint8_t recv_data = 0;
	HAL_SPI_TransmitReceive(&hspi1, &data, &recv_data, 1, 100);
	return recv_data;
}

void w25q64_init(void)
{
	MX_SPI1_Init();
}

uint16_t w25q64_read_id(void)
{
	uint16_t device_id = 0;
	
	W25Q64_CS(0);
	spi_swap_byte(FLASH_ManufactDeviceID);
	spi_swap_byte(0x00);
	spi_swap_byte(0x00);
	spi_swap_byte(0x00);
	
	device_id = spi_swap_byte(FLASH_DummyByte);
	device_id = device_id << 8;
	device_id = spi_swap_byte(FLASH_DummyByte);
	
	W25Q64_CS(1);
	return device_id;
}

void w25q64_write_enable(void) //写使能
{
	W25Q64_CS(0);
	spi_swap_byte(FLASH_WriteEnable);
	W25Q64_CS(1);
}

uint8_t w25q64_read_sr1(void) //读取sr1寄存器
{
	uint8_t sr1_value = 0;
	W25Q64_CS(0);
	spi_swap_byte(FLASH_ReadStatusReg1);
	sr1_value = spi_swap_byte(FLASH_DummyByte);
	W25Q64_CS(1);
	return sr1_value;
}

void w25q64_wait_busy(void) //sr1最后一位，判断是否是忙等状态
{
	while((w25q64_read_sr1() & 0x01) == 0x01);
}

void w25q64_read_data(uint32_t address, uint8_t *data, uint8_t size) //读取数据
{
	int i = 0;
	W25Q64_CS(0);
	spi_swap_byte(FLASH_ReadData);
	w25q64_send_address(address);
	for(i = 0; i < size; i++)
	{
		data[i] = spi_swap_byte(0xFF);
	}
	W25Q64_CS(1);
}

void w25q64_send_address(uint32_t address) //发送地址
{
	spi_swap_byte(address >> 16); //共24位地址
	spi_swap_byte(address >> 8);
	spi_swap_byte(address);
}

void w25q64_write_page(uint32_t address, uint8_t *data, uint8_t size) //页写
{
	w25q64_write_enable();
	int i = 0;
	W25Q64_CS(0);
	spi_swap_byte(FLASH_PageProgram);
	w25q64_send_address(address);
	for(i = 0; i < size; i++)
	{
		spi_swap_byte(data[i]);
	}
	W25Q64_CS(1);
	
	w25q64_wait_busy(); //检测是否处于忙等状态
}

void w25q64_erase_sector(uint32_t address) //扇区擦除
{
	w25q64_wait_busy(); //检测是否处于忙等状态
	w25q64_write_enable();
	W25Q64_CS(0);
	spi_swap_byte(FLASH_SectorErase);
	w25q64_send_address(address);
	W25Q64_CS(1);
	w25q64_wait_busy(); //检测是否处于忙等状态
}



