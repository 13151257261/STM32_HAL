/**
  * @brief	�˹��̻���stm32f103c8t6
  * 		�ѷ�װ��delay(��ʱ) uart1(����) sys(ʱ������)����
  *		    ������Ҫ�������и��ģ������SYSTEM�ļ���
  *	@author gd
  *
  */
#include "sys.h"
#include "uart1.h"
#include "delay.h"

int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    uart1_init(115200);  //���ô��ڲ�����
    while(1)
    { 

    }
}

