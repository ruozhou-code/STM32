#include "drv_exti.h"

void Exti_Config(void)
{
    GPIO_InitTypeDef GPIO_Init;

    //init PA12,open gpioa peripherals source clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    //init the PA12 to mode IT Input
    GPIO_Init.Mode = GPIO_MODE_IT_FALLING;
    GPIO_Init.Pin = GPIO_PIN_12;
    GPIO_Init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    // enable the exti
    HAL_NVIC_SetPendingIRQ(EXTI15_10_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    //HAL库默认先清理中断再处理回调函数，退出时再进行一次中断，避免按键抖动误触发
    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_12);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(30);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

