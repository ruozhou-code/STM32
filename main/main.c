#include "main.h"

int main(void)
{

	HAL_Init();

	HAL_RCC_DeInit(); // init the system clock
	RCC_Config();
	LED_Config(); // LED init
	USART_INIT();
	printf("hello world\r\n");
	//Exti_Config();
	//key_init();
	//Wwdg_Config(WWDG_PRESCALER_8, 0x7F, 0x5F);
	//iwdg_init(IWDG_PRESCALER_64, 625);
	Tim_Config(7200, 10000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_Delay(300);

	while (1)
	{
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
