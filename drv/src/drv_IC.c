#include "drv_IC.h"

TIM_HandleTypeDef htim2;
TIM_IC_InitTypeDef sConfig;

void Tim2_Ic_Config(uint16_t psc, uint16_t arr)
{
    htim2.Instance = TIM2;
    htim2.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = arr;
    htim2.Init.Prescaler = psc;
    HAL_TIM_IC_Init(&htim2);

    sConfig.ICFilter = 0;
    sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
    sConfig.ICPrescaler = TIM_ICPSC_DIV1;
    sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    HAL_TIM_IC_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1);

    /* 使能定时器溢出中断 */
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);

    /* 使能通道输入以及使能捕获中断 */
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim)
{
    GPIO_InitTypeDef GPIO_Init;
    if (htim->Instance == TIM2)
    {

        /* 使能TIM2的时钟 */
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_Init.Mode = GPIO_MODE_AF_PP;
        GPIO_Init.Pin = GPIO_PIN_0;
        GPIO_Init.Pull = GPIO_PULLDOWN;
        GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_Init);

        HAL_NVIC_SetPriority(TIM2_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
}

uint8_t IC_Flag = 0;/*输入捕获状态*/
uint16_t IC_Cnt = 0;/*输入捕获的值*/

/* 定时器捕获中断回调函数 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
    if ((IC_Flag & 0x80) == 0)
    {       /*还没有成功捕获*/
        if ((IC_Flag & 0x40))
        {   /*标记为捕获到一个下降沿*/
            IC_Flag |= 0x80;/*标记成功捕获到一次高电平脉冲*/
            IC_Cnt = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1); /*获取当前的捕获值*/
            TIM_RESET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1);//清除原来的设置
            TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);//设置为上升沿触发
        }
        else
        {
            /* 标记为捕获到一个上升沿*/
            IC_Flag = 0;
            IC_Cnt = 0;

            IC_Flag |= 0x40;
            /*计数器清0*/
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            /*清除原来的设置*/
            TIM_RESET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1);
            /*设置为下降沿触发*/
            TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);
        }
    }
}

/* 定时器溢出中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2)
    {
        if ((IC_Flag & 0x80) == 0)
        {
            /*还没有完成捕获*/
            if (IC_Flag & 0x40)
            {
                /*已经捕获到了高电平*/
                if ((IC_Flag & 0x3f) == 0x3f)/*高电平的时间太长了*/
                {
                    TIM_RESET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1);
                    TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);//设置为上升沿触发
                    IC_Flag |= 0x80;
                    IC_Cnt = 0xffff;
                }
                else
                {
                    IC_Flag++;
                }
            }
        }
    }
}



void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}