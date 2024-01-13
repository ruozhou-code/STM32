#include "main.h"

int main(void)
{
	int i = 500, flag = 1;
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
	//Tim_Config(7200, 10000);
	Pwm_Init(10000 - 1, 7200 - 1);
	while (1)
	{
		delay_ms(10);
		GPIOA_Set_Compare(i, i, i);
		if (i >= 100)
			flag = 0;
		else if (i <= 0)
			flag = 1;
		if (flag == 1)
			i = i + 1;
		else if (flag == 0)
			i = i - 1;
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
