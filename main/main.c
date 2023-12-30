#include "main.h"

int main(void)
{

	HAL_Init();

	HAL_RCC_DeInit(); // init the system clock
	RCC_Config();
	LED_Config(); // LED init
	USART_INIT();
	printf("hello world\r\n");
	iwdg_init(IWDG_PRESCALER_64, 625);
	//Exti_Config();
	key_init();
	while (1)
	{
		if (IsKeyPressing() == KEY_IS_PRESS)
		{
			iwdg_feed();
		}
		HAL_Delay(10);
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
