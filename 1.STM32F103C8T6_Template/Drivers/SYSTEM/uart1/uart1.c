#include "sys.h"
#include "uart1.h"
#include "string.h"

UART_HandleTypeDef uart1_handle;                                            /* UART1��� */

uint8_t uart1_rx_buf[UART1_RX_BUF_SIZE];                                    /* UART1���ջ����� */
uint16_t uart1_rx_len = 0;                                                  /* UART1�����ַ����� */

/**
 * @brief       �ض���fputc����
 * @note        printf�������ջ�ͨ������fputc����ַ���������
 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);                                       /* �ȴ���һ���ַ�������� */

    USART1->DR = (uint8_t)ch;                                               /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}

/**
 * @brief       ����1��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void uart1_init(uint32_t baudrate)
{
    /*UART1 ��ʼ������*/
    uart1_handle.Instance = USART1;                                         /* USART1 */
    uart1_handle.Init.BaudRate = baudrate;                                  /* ������ */
    uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* �ֳ�Ϊ8λ���ݸ�ʽ */
    uart1_handle.Init.StopBits = UART_STOPBITS_1;                           /* һ��ֹͣλ */
    uart1_handle.Init.Parity = UART_PARITY_NONE;                            /* ����żУ��λ */
    uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* ��Ӳ������ */
    uart1_handle.Init.Mode = UART_MODE_TX_RX;                               /* �շ�ģʽ */
    HAL_UART_Init(&uart1_handle);                                           /* HAL_UART_Init()��ʹ��UART1 */
}

/**
 * @brief       UART�ײ��ʼ������
 * @param       huart: UART�������ָ��
 * @note        �˺����ᱻHAL_UART_Init()����
 *              ���ʱ��ʹ�ܣ��������ã��ж�����
 * @retval      ��
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (huart->Instance == USART1)                                          /* ����Ǵ���1�����д���1 MSP��ʼ�� */
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();                                       /* ʹ�ܴ���TX��ʱ�� */
        __HAL_RCC_USART1_CLK_ENABLE();                                      /* ʹ�ܴ���ʱ�� */

        gpio_init_struct.Pin = GPIO_PIN_9;                                  /* ���ڷ������ź� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                            /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLUP;                                /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                      /* IO�ٶ�����Ϊ���� */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
                
        gpio_init_struct.Pin = GPIO_PIN_10;                                 /* ����RX�� ģʽ���� */
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;    
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);                            /* ����RX�� �������ó�����ģʽ */
        
        HAL_NVIC_EnableIRQ(USART1_IRQn);                                    /* ʹ��USART1�ж�ͨ�� */
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);                            /* ��2��������ȼ�:��ռ���ȼ�3�������ȼ�3 */

        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);                          /* ʹ��UART1�����ж� */
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);                          /* ʹ��UART1���߿����ж� */
    }
}

/**
 * @brief       UART1���ջ��������
 * @param       ��
 * @retval      ��
 */
void uart1_rx_clear(void)
{
    memset(uart1_rx_buf, 0, sizeof(uart1_rx_buf));                          /* ��ս��ջ����� */
    uart1_rx_len = 0;                                                       /* ���ռ��������� */
}

/**
 * @brief       ����1�жϷ�����
 * @note        �ڴ�ʹ�ý����жϼ������жϣ�ʵ�ֲ����������շ�
 * @param       ��
 * @retval      ��
 */
void USART1_IRQHandler(void)
{
    uint8_t receive_data = 0;   
    if(__HAL_UART_GET_FLAG(&uart1_handle, UART_FLAG_RXNE) != RESET){        /* ��ȡ����RXNE��־λ�Ƿ���λ */
        if(uart1_rx_len >= sizeof(uart1_rx_buf))                            /* ������յ��ַ������ڽ��ջ�������С�� */
            uart1_rx_len = 0;                                               /* �򽫽��ռ��������� */
        HAL_UART_Receive(&uart1_handle, &receive_data, 1, 1000);            /* ����һ���ַ� */
        uart1_rx_buf[uart1_rx_len++] = receive_data;                        /* �����յ����ַ������ڽ��ջ����� */
    }

    if (__HAL_UART_GET_FLAG(&uart1_handle, UART_FLAG_IDLE) != RESET)        /* ��ȡ���տ����жϱ�־λ�Ƿ���λ */
    {
        printf("recv: %s\r\n", uart1_rx_buf);                               /* �����յ������ݴ�ӡ���� */
        uart1_rx_clear();
        __HAL_UART_CLEAR_IDLEFLAG(&uart1_handle);                           /* ���UART���߿����ж� */
    }
}
