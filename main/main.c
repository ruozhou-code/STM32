#include "main.h"

int main(void)
{
	uint32_t i = 0;
	int flag = 1;
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
	//Pwm_Init(100, 7200);
	Tim2_Ic_Config(72 - 1, 65536 - 1);
	while (1)
	{
		if ((IC_Flag & 0x80))
		{
			i = (IC_Flag & 0x3f) * 65536 + IC_Cnt;
			printf("the high time is %u\r\n", i);
			IC_Flag = 0;
		}
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
