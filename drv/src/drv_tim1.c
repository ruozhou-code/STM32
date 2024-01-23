#include "drv_tim1.h"

TIM_HandleTypeDef tim1;

/**
 * @brief 这个是用来配置高级定时器输出PWM的设置
 * @author Wan Sir
 * @note 高级定时器的时钟来源子APB2=72Mhz
 * PB0---- > 通道2            PB1---- > 通道3
 * @param psc:预分频系数
 * @param per:自动重装值
 * @return none return
 * **/
void Tim1_Config(uint16_t psc, uint16_t per)
{
    GPIO_InitTypeDef GPIO_Init;
    TIM_OC_InitTypeDef sConfig;
    /*使能时钟*/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_TIM1_PARTIAL();

    /*初始化定时器的PWM*/
    tim1.Instance = TIM1;
    tim1.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE;
    tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1.Init.Period = per;
    tim1.Init.Prescaler = psc;
    tim1.Init.RepetitionCounter = 0;                                /*重复计数初始值*/
    HAL_TIM_PWM_Init(&tim1);

    /*初始化PB0和PB1*/
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = GPIO_PIN_8 | GPIO_PIN_11;
    GPIO_Init.Pull = GPIO_PULLUP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    /*设置PWM输出通道*/
    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.Pulse = 2500;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&tim1, &sConfig, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&tim1, &sConfig, TIM_CHANNEL_4);

    /*设置中断优先级*/
    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);                       /*开启TIM1中断*/

    __HAL_TIM_ENABLE_IT(&tim1, TIM_IT_UPDATE);              /*允许更新中断*/
    HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_1);        /*使能通道2和通道3的中断*/
    HAL_TIM_PWM_Start(&tim1, TIM_CHANNEL_4);
    //__HAL_TIM_ENABLE(&tim1);
}

static uint32_t g_npwm_remain = 0;                          /*用来保存需要发送的脉冲个数*/

/**
 * @brief 高级定时器TIM1设置的脉冲个数
 * @param npwm：保存的脉冲的个数
 * @retval none
*/
void tim1_npwm_set(uint32_t cnt)
{
    if (cnt == 0) return;
    g_npwm_remain = cnt;
    /*产生一次更新事件，在中断里面处理脉冲输出*/
    HAL_TIM_GenerateEvent(&tim1, TIM_EVENTSOURCE_UPDATE);
    __HAL_TIM_ENABLE(&tim1);
}

void Tim1_setcompare(uint16_t i, uint16_t j)
{
    __HAL_TIM_SET_COMPARE(&tim1, TIM_CHANNEL_1, i);
    __HAL_TIM_SET_COMPARE(&tim1, TIM_CHANNEL_4, j);
}

/*定时器中断回调函数*/
void TIM1_UP_IRQHandler(void)
{
    uint16_t npwm = 0;
    if (__HAL_TIM_GET_FLAG(&tim1, TIM_FLAG_UPDATE) != RESET)
    {
        if (g_npwm_remain >= 256)       /*因为寄存器最大可重复计数位8位，所以要先判断*/
        {
            g_npwm_remain -= 256;
            npwm = 256;
        }
        else if (g_npwm_remain % 256)
        {
            npwm = g_npwm_remain;
            g_npwm_remain = 0;
        }
        if (npwm)
        {
            TIM1->RCR = npwm - 1;
            HAL_TIM_GenerateEvent(&tim1, TIM_EVENTSOURCE_UPDATE);
            __HAL_TIM_ENABLE(&tim1);
        }
        else
        {
            /* 关闭定时器 TIMX,使用__HAL_TIM_DISABLE 需要失能通道输出，所以不用 */
            TIM1->CR1 &= ~(1 << 0);
        }
        __HAL_TIM_CLEAR_FLAG(&tim1, TIM_FLAG_UPDATE);
    }
}