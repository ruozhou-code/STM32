#include "main.h"

void delay_us(uint16_t us)
{
	int i = 0;
	for (i = 0; i < us; i++)
	{
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

void delay_ms(uint16_t ms)
{
	int i = 0;
	for (i = 0; i < ms; i++)
	{
		delay_us(1000);
	}
}

int main(void)
{

	HAL_Init();

	HAL_RCC_DeInit(); // init the system clock
	RCC_Config();
	LED_Config(); // LED init
	USART_INIT();
	printf("hello world\r\n");
	key_init();
	while (1)
	{
#if 0
		if (IsKeyPressing() == KEY_IS_PRESS)
			printf("key is press\r\n");
		else if (IsKeyPressing() == KEY_NO_PRESS)
			printf("kee is no press\r\n");
		HAL_Delay(1000);
#else
		IsKeyPressThenRelease();
		if (key_PressRelease_flag == 255)
			printf("key pressing");

#endif

	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
