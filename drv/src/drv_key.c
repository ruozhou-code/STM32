#include "drv_key.h"

uint8_t key_PressRelease_flag = 0;

void key_init(void)
{
    GPIO_InitTypeDef GPIO_Init;
    //the key is using pa12
    //open the gpioa clock source
    __HAL_RCC_GPIOA_CLK_ENABLE();

    //init the pa12 to input module
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Pin = GPIO_PIN_12;
    GPIO_Init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_Init);
}

//when the key is press?
char IsKeyPressing(void)
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
    {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
            return KEY_IS_PRESS;
    }
    else
    {
        return KEY_NO_PRESS;
    }
    return KEY_NO_PRESS;
}

//the key was press then release
void IsKeyPressThenRelease(void)
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
    {
        HAL_Delay(30);
        if (KEY_READ == GPIO_PIN_RESET)
        {
            HAL_Delay(30);
            while (KEY_READ != GPIO_PIN_SET);
            HAL_Delay(30);
            if (KEY_READ == GPIO_PIN_SET)
            {
                key_PressRelease_flag = (~key_PressRelease_flag);
            }
        }
    }
}
