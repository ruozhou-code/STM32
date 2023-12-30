#include "main.h"

int main(void)
{

	HAL_Init();

	HAL_RCC_DeInit(); // init the system clock
	RCC_Config();
	LED_Config(); // LED init
	USART_INIT();
	printf("hello world\r\n");
	Exti_Config();
	while (1)
	{
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
