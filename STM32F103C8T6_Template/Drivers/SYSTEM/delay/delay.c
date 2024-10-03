#include "delay.h"

/**
  * @brief  ΢�뼶��ʱ
  * @param  nus ��ʱʱ������Χ��0~233015
  * @retval ��
  */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = 72 * nus;                           /* ���ö�ʱ����װֵ */
    SysTick->VAL = 0x00;                                /* ��յ�ǰ����ֵ */
    SysTick->CTRL |= 1 << 2;                            /* ���÷�Ƶϵ��Ϊ1��Ƶ */
    SysTick->CTRL |= 1 << 0;                            /* ������ʱ�� */
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));     /* �ȴ�������0 */
    SysTick->CTRL &= ~(1 << 0);                         /* �رն�ʱ�� */
}

/**
  * @brief  ���뼶��ʱ
  * @param  nms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void delay_ms(uint32_t nms)
{
    while(nms--)
        delay_us(1000);
}
 
/**
  * @brief  �뼶��ʱ
  * @param  ns ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void delay_s(uint32_t ns)
{
    while(ns--)
        delay_ms(1000);
}

/**
  * @brief  ��дHAL_Delay����
  * @param  nms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void HAL_Delay(uint32_t nms)
{
    delay_ms(nms);
}
