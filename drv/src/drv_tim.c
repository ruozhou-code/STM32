#include "drv_tim.h"

TIM_HandleTypeDef htim1;

void Tim_Config(uint32_t prescaler, uint32_t counter)
{
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    //basetime is no this function
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = counter;
    htim1.Init.Prescaler = prescaler;
    htim1.Instance = TIM2;
    HAL_TIM_Base_Init(&htim1);

    //Enable the tim and update interrupt;
    HAL_TIM_Base_Start_IT(&htim1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
    //使能tim2的时钟
    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM2_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
    //HAL_TIM_IRQHandler(&htim1);
    if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE) != RESET)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    }
    __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);
}
