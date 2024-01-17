#include "drv_adc.h"

ADC_HandleTypeDef hadc1;

/* PA6 ADC的通道6 */
void Adc_Config(void)
{
    hadc1.Instance = ADC1;
    hadc1.Init.ContinuousConvMode = DISABLE;     /*关闭连续转换模式*/
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;  /*使用右对齐*/
    hadc1.Init.DiscontinuousConvMode = DISABLE;  /*禁止规则通道组间断模式*/
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.NbrOfDiscConversion = 0;          /*组间中断的个数*/
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    //hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    HAL_ADC_Init(&hadc1);



    HAL_ADCEx_Calibration_Start(&hadc1);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    if (hadc->Instance == ADC1)
    {
        /* 打开时钟 */
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* 设置ADC的时钟，最高不能超过14Mhz */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;         /* select the clock you want to config----> ADC */
        PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;            /* config the adc clock to 6div */
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

        /* open the adc clock */
        __HAL_RCC_ADC1_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_Init;
        /* 初始化PA6 */
        GPIO_Init.Mode = GPIO_MODE_ANALOG;
        GPIO_Init.Pin = GPIO_PIN_6;
        GPIO_Init.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_Init);
    }
}
/**
* @brief 设置 ADC 通道采样时间
* @param adcx : adc 句柄指针,ADC_HandleTypeDef
* @param ch : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
* @param stime: 采样时间 0~7, 对应关系为:
* @arg ADC_SAMPLETIME_1CYCLE_5, 1.5 个 ADC 时钟周期
* @arg ADC_SAMPLETIME_7CYCLES_5, 7.5 个 ADC 时钟周期
* @arg ADC_SAMPLETIME_13CYCLES_5, 13.5 个 ADC 时钟周期 ADC_SAMPLETIME_28CYCLES_5, 28.5 个 ADC 时钟周期
* @arg ADC_SAMPLETIME_41CYCLES_5, 41.5 个 ADC 时钟周期 ADC_SAMPLETIME_55CYCLES_5, 55.5 个 ADC 时钟周期
* @arg ADC_SAMPLETIME_71CYCLES_5, 71.5 个 ADC 时钟周期 ADC_SAMPLETIME_239CYCLES_5, 239.5 个 ADC 时钟周期
* @param rank: 多通道采集时需要设置的采集编号,
假设你定义 channle1 的 rank=1， channle2 的 rank=2，
那么对应你在 DMA 缓存空间的变量数组 AdcDMA[0] 就 i 是 channle1 的转换结果，
AdcDMA[1]就是通道 2 的转换结果。
单通道 DMA 设置为 ADC_REGULAR_RANK_1
* @arg 编号 1~16： ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
* @retval 无
*/
void adc_channel_set(ADC_HandleTypeDef* adc_handle, uint32_t channel, uint32_t rank, uint32_t cycletime)
{
    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = channel;
    sConfig.Rank = rank;
    sConfig.SamplingTime = cycletime;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/**
* @brief 获得 ADC 转换后的结果
* @param ch: 通道值 0~17，取值范围为： ADC_CHANNEL_0~ADC_CHANNEL_17
* @retval 无
*/
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&hadc1, ch, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5); /* 设置通道/序列和采样时间 */
    HAL_ADC_Start(&hadc1); /* 开启 ADC */
    HAL_ADC_PollForConversion(&hadc1, 10); /* 等待转换结束 */
    /* 返回最近一次 ADC1 规则组的转换结果 */
    return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

/**
* @brief 获取通道 ch 的转换值，取 times 次,然后平均
* @param ch : 通道号, 0~17
* @param times : 获取次数
* @retval 通道 ch 的 times 次转换结果平均值
*/
uint32_t adc_get_result_average(uint32_t channle, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t i;
    for (i = 0;i < times;i++)
    {
        temp_val += adc_get_result(channle);
        delay_ms(5);
    }
    return temp_val / times;        /*return the average*/
}