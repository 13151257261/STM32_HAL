#include "sys.h"

/**
 * @brief       ϵͳʱ�ӳ�ʼ������
 * @param       plln: PLL��Ƶϵ��(PLL��Ƶ), ȡֵ��Χ: 2~16
 * @retval      ��
 */
void stm32_clock_init(uint32_t plln)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    RCC_OscInitTypeDef rcc_osc_init = {0};
    RCC_ClkInitTypeDef rcc_clk_init = {0};

    rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;       /* ѡ��Ҫ����HSE */
    rcc_osc_init.HSEState = RCC_HSE_ON;                         /* ��HSE */
    rcc_osc_init.HSEPredivValue = RCC_HSE_PREDIV_DIV1;          /* HSEԤ��Ƶϵ�� */
    rcc_osc_init.PLL.PLLState = RCC_PLL_ON;                     /* ��PLL */
    rcc_osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;             /* PLLʱ��Դѡ��HSE */
    rcc_osc_init.PLL.PLLMUL = plln;                             /* PLL��Ƶϵ�� */
    ret = HAL_RCC_OscConfig(&rcc_osc_init);                     /* ��ʼ�� */

    if (ret != HAL_OK)
    {
        while (1);                                              /* ʱ�ӳ�ʼ��ʧ�ܣ�֮��ĳ��򽫿����޷�����ִ�У���������������Լ��Ĵ��� */
    }

    /* ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2*/
    rcc_clk_init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;        /* ����ϵͳʱ������PLL */
    rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;               /* AHB��Ƶϵ��Ϊ1 */
    rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;                /* APB1��Ƶϵ��Ϊ2 */
    rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;                /* APB2��Ƶϵ��Ϊ1 */
    ret = HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_2);  /* ͬʱ����FLASH��ʱ����Ϊ2WS��Ҳ����3��CPU���ڡ� */

    if (ret != HAL_OK)
    {
        while (1);                                              /* ʱ�ӳ�ʼ��ʧ�ܣ�֮��ĳ��򽫿����޷�����ִ�У���������������Լ��Ĵ��� */
    }
}


