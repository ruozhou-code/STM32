#if 0
/*
 *ADC1 �? DMA 通道�?能是: DMA1_Channel1
*/

#include "drv_adc.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc;
uint8_t adc_dma_sta;                            /* 标�?�DMA数据�?否传输完�? */


/* PA6 ADC的通道6 */
void Adc_Config(uint32_t mar)
{
    GPIO_InitTypeDef GPIO_Init;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    ADC_ChannelConfTypeDef sConfig;

    /* enable the essential peripherals */
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* enable the dma1 */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* 设置ADC的时钟，最高不能超�?14Mhz 72Mhz/6 = 12Mhz*/
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;         /* select the clock you want to config----> ADC */
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;            /* config the adc clock to 6div */
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    /* open the adc clock */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* init the PA6 */
    GPIO_Init.Mode = GPIO_MODE_ANALOG;
    GPIO_Init.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    /* Init the DMA */
    hdma_adc.Instance = DMA1_Channel1;                                       /* the adc1 used the DMA */
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;                 /* from adc to register */
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* 16bit to transmit, config to memory*/
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;                         /* memory address is inc */
    hdma_adc.Init.Mode = DMA_NORMAL;                                /* flow to conctrl */
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* 16bit to transmit, config to peripheral */
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;                     /* peripheral address keep no change */
    hdma_adc.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* config the priority to medium priority */
    HAL_DMA_Init(&hdma_adc);

    /* linked between DMA and ADC1 */
    __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc);

    hadc1.Instance = ADC1;
    hadc1.Init.ContinuousConvMode = DISABLE;     /*关闭连续�?换模�?*/
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;  /*使用右�?�齐*/
    hadc1.Init.DiscontinuousConvMode = DISABLE;  /*禁�?��?�则通道组间�?模式*/
    hadc1.Init.NbrOfConversion = 1;
    //hadc1.Init.NbrOfDiscConversion = 0;          /*组间�?�?的个�?*/
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    //hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /* config the DMA1 Priority  */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    /* start DMA and open IT */
    HAL_DMA_Start_IT(&hdma_adc, (uint32_t)&ADC1->DR, mar, 1);
    HAL_ADC_Start_DMA(&hadc1, &mar, 1);

    HAL_NVIC_SetPriority(ADC1_2_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    HAL_ADCEx_Calibration_Start(&hadc1); /* 校准 ADC */
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
#if 0
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    DMA_HandleTypeDef hdma_adc;
    if (hadc->Instance == ADC1)
    {
        /* enable the essential peripherals */
        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* enable the dma1 */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /* 设置ADC的时钟，最高不能超�?14Mhz 72Mhz/6 = 12Mhz*/
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;         /* select the clock you want to config----> ADC */
        PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;            /* config the adc clock to 6div */
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

        /* open the adc clock */
        __HAL_RCC_ADC1_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_Init;
        /* init the PA6 */
        GPIO_Init.Mode = GPIO_MODE_ANALOG;
        GPIO_Init.Pin = GPIO_PIN_6;
        GPIO_Init.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_Init);

        /* Init the DMA */
        hdma_adc.Instance = DMA1_Channel1;                                       /* the adc1 used the DMA */
        hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;                 /* from adc to register */
        hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* 16bit to transmit, config to memory*/
        hdma_adc.Init.MemInc = DMA_MINC_ENABLE;                         /* memory address is inc */
        hdma_adc.Init.Mode = DMA_NORMAL;                                /* flow to conctrl */
        hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* 16bit to transmit, config to peripheral */
        hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;                     /* peripheral address keep no change */
        hdma_adc.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* config the priority to medium priority */
        HAL_DMA_Init(&hdma_adc);

        /* linked between DMA and ADC1 */
        __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc);

        ADC_ChannelConfTypeDef sConfig;
        sConfig.Channel = ADC_CHANNEL_6;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

        /* config the DMA1 Priority  */
        HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

        /* start DMA and open IT */
        HAL_DMA_Start_IT(&hdma_adc, (uint32_t)ADC1->DR, (uint32_t)adc_dma_buf, 0);
        HAL_ADC_Start_DMA(&hadc1, adc_dma_buf, 0);
    }
#endif       
}

/**
* @brief 设置 ADC 通道采样时间
* @param adcx : adc 句柄指针,ADC_HandleTypeDef
* @param ch : 通道�?, ADC_CHANNEL_0~ADC_CHANNEL_17
* @param stime: 采样时间 0~7, 对应关系�?:
* @arg ADC_SAMPLETIME_1CYCLE_5, 1.5 �? ADC 时钟周期
* @arg ADC_SAMPLETIME_7CYCLES_5, 7.5 �? ADC 时钟周期
* @arg ADC_SAMPLETIME_13CYCLES_5, 13.5 �? ADC 时钟周期 ADC_SAMPLETIME_28CYCLES_5, 28.5 �? ADC 时钟周期
* @arg ADC_SAMPLETIME_41CYCLES_5, 41.5 �? ADC 时钟周期 ADC_SAMPLETIME_55CYCLES_5, 55.5 �? ADC 时钟周期
* @arg ADC_SAMPLETIME_71CYCLES_5, 71.5 �? ADC 时钟周期 ADC_SAMPLETIME_239CYCLES_5, 239.5 �? ADC 时钟周期
* @param rank: 多通道采集时需要�?�置的采集编�?,
假�?�你定义 channle1 �? rank=1�? channle2 �? rank=2�?
那么对应你在 DMA 缓存空间的变量数�? AdcDMA[0] �? i �? channle1 的转换结果，
AdcDMA[1]就是通道 2 的转换结果�?
单通道 DMA 设置�? ADC_REGULAR_RANK_1
* @arg 编号 1~16�? ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
* @retval �?
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
* @brief 获得 ADC �?换后的结�?
* @param ch: 通道�? 0~17，取值范围为�? ADC_CHANNEL_0~ADC_CHANNEL_17
* @retval �?
*/
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&hadc1, ch, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5); /* 设置通道/序列和采样时�? */
    HAL_ADC_Start(&hadc1); /* 开�? ADC */
    HAL_ADC_PollForConversion(&hadc1, 10); /* 等待�?换结�? */
    /* 返回最近一�? ADC1 规则组的�?换结�? */
    return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

/**
* @brief 获取通道 ch 的转换值，�? times �?,然后平均
* @param ch : 通道�?, 0~17
* @param times : 获取次数
* @retval 通道 ch �? times 次转换结果平均�?
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

/**
* @brief 使能一�? ADC DMA 传输
* @note 该函数用寄存器来操作，防止用 HAL 库操作�?�其他参数有�?�?,也为了兼容�?
* @param ndtr: DMA 传输的�?�数
* @retval �?
*/
void adc_dma_enable(uint16_t cndtr)
{
    ADC1->CR2 &= ~(1 << 0); /* 先关�? ADC */
    DMA1_Channel1->CCR &= ~(1 << 0); /* 关闭 DMA 传输 */
    while (DMA1_Channel1->CCR & (1 << 0)); /* �?�? DMA �?以�??设置 */
    DMA1_Channel1->CNDTR = cndtr; /* DMA 传输数据�? */
    DMA1_Channel1->CCR |= 1 << 0; /* 开�? DMA 传输 */
    ADC1->CR2 |= 1 << 0; /* 重新�?�? ADC */
    ADC1->CR2 |= 1 << 22; /* �?动�?�则�?换通道 */
}

void DMA1_Channel1_IRQHandler(void)
{
#if 0
    if (ADC_ADCX_DMACx_IS_TC())
    {
        adc_dma_sta = 1;
        ADC_ADCX_DMACx_CLR_TC();    /*clear the it flag*/
    }
#else
    HAL_DMA_IRQHandler(&hdma_adc);
#endif
}

void ADC1_2_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&hadc1);
}

/* 回调函数 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc_dma_sta = 1;
    }
}
#endif
#include "drv_adc.h"

ADC_HandleTypeDef g_adc_handle;

void adc_init(void)
{
    g_adc_handle.Instance = ADC_ADCX;
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_handle.Init.ContinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfConversion = 1;
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfDiscConversion = 0;
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_handle);

    HAL_ADCEx_Calibration_Start(&g_adc_handle);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC_ADCX)
    {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = { 0 };

        ADC_ADCX_CHY_CLK_ENABLE();
        ADC_ADCX_CHY_GPIO_CLK_ENABLE();

        /* ÉèÖÃADCÊ±ÖÓ */
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);

        /* ÉèÖÃAD²É¼¯Í¨µÀ¶ÔÓ¦IOÒý½Å¹¤×÷Ä£Ê½ */
        gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);
    }
}

void adc_channel_set(ADC_HandleTypeDef* adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf;

    adc_ch_conf.Channel = ch;
    adc_ch_conf.Rank = rank;
    adc_ch_conf.SamplingTime = stime;
    HAL_ADC_ConfigChannel(adc_handle, &adc_ch_conf);
}

uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_handle, ch, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);

    HAL_ADC_Start(&g_adc_handle);
    HAL_ADC_PollForConversion(&g_adc_handle, 10);
    return (uint16_t)HAL_ADC_GetValue(&g_adc_handle);
}

uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;
}

DMA_HandleTypeDef g_dma_adc_handle = { 0 };
ADC_HandleTypeDef g_adc_dma_handle = { 0 };
uint8_t g_adc_dma_sta = 0;

void adc_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = { 0 };
    ADC_ChannelConfTypeDef adc_ch_conf = { 0 };

    ADC_ADCX_CHY_CLK_ENABLE();
    ADC_ADCX_CHY_GPIO_CLK_ENABLE();

    if ((uint32_t)ADC_ADCX_DMACx > (uint32_t)DMA1_Channel7)
    {
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();
    }

    /* ÉèÖÃADCÊ±ÖÓ */
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);

    /* ÉèÖÃAD²É¼¯Í¨µÀ¶ÔÓ¦IOÒý½Å¹¤×÷Ä£Ê½ */
    gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);

    /* ³õÊ¼»¯DMA */
    g_dma_adc_handle.Instance = ADC_ADCX_DMACx;
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&g_dma_adc_handle);

    __HAL_LINKDMA(&g_adc_dma_handle, DMA_Handle, g_dma_adc_handle);

    g_adc_dma_handle.Instance = ADC_ADCX;
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_dma_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;
    g_adc_dma_handle.Init.NbrOfConversion = 1;
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_dma_handle);

    HAL_ADCEx_Calibration_Start(&g_adc_dma_handle);

    /* ÅäÖÃADCÍ¨µÀ */
    adc_ch_conf.Channel = ADC_ADCX_CHY;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_dma_handle, &adc_ch_conf);

    /* ÅäÖÃDMAÊý¾ÝÁ÷ÇëÇóÖÐ¶ÏÓÅÏÈ¼¶ */
    HAL_NVIC_SetPriority(ADC_ADCX_DMACx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMACx_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0);
    HAL_ADC_Start_DMA(&g_adc_dma_handle, &mar, 0);
}

DMA_HandleTypeDef gdma_nch_adc_handle;
ADC_HandleTypeDef g_adc_nch_dma_handle;

void adc_nch_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = { 0 };
    ADC_ChannelConfTypeDef adc_ch_conf = { 0 };

    /* 使能adcx的时钟 */
    ADC_ADCX_CHY_CLK_ENABLE();
    /* 使能GPIOA的时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /*开启DMA1和DMA2的时钟*/
    __HAL_RCC_DMA1_CLK_ENABLE();

    /*设置ADC的时钟*/
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);

    /*设置ADC采集通道ADC的GPIO*/
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    /* 初始化DMA */
    gdma_nch_adc_handle.Instance = ADC_ADCX_DMACx;
    gdma_nch_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    gdma_nch_adc_handle.Init.MemInc = DMA_MINC_ENABLE;
    gdma_nch_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    gdma_nch_adc_handle.Init.Mode = DMA_NORMAL;
    gdma_nch_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    gdma_nch_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    gdma_nch_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&gdma_nch_adc_handle);

    /*将DMA和ADC连接起来*/
    __HAL_LINKDMA(&g_adc_nch_dma_handle, DMA_Handle, gdma_nch_adc_handle);

    /*初始化ADC*/
    g_adc_nch_dma_handle.Instance = ADC_ADCX;
    g_adc_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_nch_dma_handle.Init.ScanConvMode = ADC_SCAN_ENABLE;               /*使能扫描模式*/
    g_adc_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                  /*使能连续模式*/
    g_adc_nch_dma_handle.Init.NbrOfConversion = 8;                          /*有8个通道*/
    g_adc_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;              /*禁止规则组间中断模式*/
    g_adc_nch_dma_handle.Init.NbrOfDiscConversion = 0;
    g_adc_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;        /*软件触发*/
    HAL_ADC_Init(&g_adc_nch_dma_handle);

    /*校准ADC*/
    HAL_ADCEx_Calibration_Start(&g_adc_nch_dma_handle);

    /*设置通道*/
    adc_ch_conf.Channel = ADC_CHANNEL_0;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA0*/

    adc_ch_conf.Channel = ADC_CHANNEL_1;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA1*/

    adc_ch_conf.Channel = ADC_CHANNEL_2;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_3;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA2*/

    adc_ch_conf.Channel = ADC_CHANNEL_3;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_4;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA3*/

    adc_ch_conf.Channel = ADC_CHANNEL_4;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_5;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA4*/

    adc_ch_conf.Channel = ADC_CHANNEL_5;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_6;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA5*/

    adc_ch_conf.Channel = ADC_CHANNEL_6;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_7;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA6*/

    adc_ch_conf.Channel = ADC_CHANNEL_7;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_8;
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);/*PA7*/

    /*配置DMA数据量请求中断优先级*/
    HAL_NVIC_SetPriority(ADC_ADCX_DMACx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMACx_IRQn);

    /*启动DMA并开启中断*/
    HAL_DMA_Start_IT(&gdma_nch_adc_handle, (uint32_t)&ADC1->DR, mar, 0);

    /*启动DMA的ADC传输*/
    HAL_ADC_Start_DMA(&g_adc_nch_dma_handle, &mar, 0);
}

void adc_dma_enable(uint16_t cndtr)
{
    ADC_ADCX->CR2 &= ~(1 << 0);

    ADC_ADCX_DMACx->CCR &= ~(1 << 0);
    while (ADC_ADCX_DMACx->CCR & (1 << 0));
    ADC_ADCX_DMACx->CNDTR = cndtr;
    ADC_ADCX_DMACx->CCR |= 1 << 0;

    ADC_ADCX->CR2 |= 1 << 0;
    ADC_ADCX->CR2 |= 1 << 22;
}


void ADC_ADCX_DMACx_IRQHandler(void)
{
    if (ADC_ADCX_DMACx_IS_TC())
    {
        g_adc_dma_sta = 1;
        ADC_ADCX_DMACx_CLR_TC();
    }
}