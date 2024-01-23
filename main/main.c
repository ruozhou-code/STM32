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

	Tim1_Config(7200 - 1, 5000);

	/*两种方法可以用来改变比较值*/
#if 1
	Tim1_setcompare(500, 500);
#else
	TIM1->CCR1 = 2500;
#endif
	tim1_npwm_set(5);

	while (1)
	{
		// HAL_Delay(10000);
		// printf("nihao");

	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
