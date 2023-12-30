#include "drv_iwdg.h"
IWDG_HandleTypeDef hiwdg1;

//初始化独立看门狗
void iwdg_init(uint8_t prer, uint16_t rlr)
{
    hiwdg1.Instance = IWDG;
    hiwdg1.Init.Prescaler = prer;
    hiwdg1.Init.Reload = rlr;
    HAL_IWDG_Init(&hiwdg1);
}

/*喂狗*/
void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&hiwdg1);
}
