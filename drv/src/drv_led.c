#include "drv_led.h"

void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_Init;
	//open the PC13 clock
	__HAL_RCC_GPIOC_CLK_ENABLE();

	//init the PC13 to PP
	GPIO_Init.Mode					= GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pin					= GPIO_PIN_13;
	GPIO_Init.Pull					= GPIO_PULLUP;
	GPIO_Init.Speed					= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
}
