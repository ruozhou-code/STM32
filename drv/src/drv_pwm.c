#include "drv_pwm.h"

TIM_HandleTypeDef htim2;
TIM_OC_InitTypeDef sConfig;
TIM_ClockConfigTypeDef sClockSourceConfig;

void Pwm_Init(uint32_t period, uint32_t prescaler)
{
    htim2.Instance = TIM2;
    htim2.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = period;
    htim2.Init.Prescaler = prescaler;
    HAL_TIM_PWM_Init(&htim2);


    //sConfig.OCFastMode = TIM_OCFAST_ENABLE;
    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.Pulse = period / 2;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    //HAL_TIM_PWM_ConfigChannel(&htim2, &sConfig, TIM_CHANNEL_1 | TIM_CHANNEL_3 | TIM_CHANNEL_3);
    //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3);
    //HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2)
    {
        GPIO_InitTypeDef GPIO_Init;

        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_Init.Mode = GPIO_MODE_AF_PP;
        GPIO_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;// | GPIO_PIN_2;
        GPIO_Init.Pull = GPIO_NOPULL;
        GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_Init);

        __HAL_RCC_AFIO_CLK_ENABLE();
        __HAL_AFIO_REMAP_TIM2_DISABLE();
    }
}

void GPIOA_Set_Compare(uint16_t PA0, uint16_t PA1, uint16_t PA2)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PA0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PA1);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, PA2);
}

