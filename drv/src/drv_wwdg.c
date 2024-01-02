#include "drv_wwdg.h"

WWDG_HandleTypeDef hwwdg1;

void Wwdg_Config(uint32_t prescale, uint32_t counter, uint32_t window)
{
    hwwdg1.Instance = WWDG;
    hwwdg1.Init.Counter = counter;
    hwwdg1.Init.EWIMode = WWDG_EWI_ENABLE;
    hwwdg1.Init.Prescaler = prescale;
    hwwdg1.Init.Window = window;
    HAL_WWDG_Init(&hwwdg1);
}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{
    //enable the clock peripheral of WWDG
    __HAL_RCC_WWDG_CLK_ENABLE();

    HAL_NVIC_EnableIRQ(WWDG_IRQn);
    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 2);
}

void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&hwwdg1);
}

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    if (hwwdg->Instance == WWDG)
    {
        HAL_WWDG_Refresh(&hwwdg1);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
        //__HAL_WWDG_CLEAR_IT(hwwdg, WWDG_FLAG_EWIF);
    }
}

