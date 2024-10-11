/**
  * @brief	此工程基于stm32f103c8t6
  * 		已封装好delay(延时) uart1(串口) sys(时钟配置)函数
  *		    如有需要，请自行更改，详情见SYSTEM文件夹
  *	@author gd
  *
  */
#include "sys.h"
#include "uart1.h"
#include "delay.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    uart1_init(115200);  //设置串口波特率
    while(1)
    { 

    }
}

