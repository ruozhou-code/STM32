#include "main.h"

/* 要循环发送的字符串 */
const uint8_t TEXT_TO_SEND[] = "wu";
uint8_t FUCK[] = "fuck\r\n";

#define SEND_BUF_SIZE (sizeof(TEXT_TO_SEND)*2 + 200)

/* 要发送的长度 */
uint8_t g_sendbuf[SEND_BUF_SIZE];

 uint32_t temp = 0;
 float ave = 0;

int main(void)
{
	uint32_t i = 0;
	
	uint8_t mask = 0;
	int flag = 1, len = 0;
	HAL_Init();

	HAL_RCC_DeInit(); // init the system clock
	RCC_Config();
	LED_Config(); // LED init
	USART_INIT();
	printf("xi hai qing ge\r\n");
	printf("ri bu luo\r\n");
	//Exti_Config();
	//key_init();
	//Wwdg_Config(WWDG_PRESCALER_8, 0x7F, 0x5F);
	//iwdg_init(IWDG_PRESCALER_64, 625);
	//Tim_Config(7200, 10000);
	//Pwm_Init(100, 7200);
	//Tim2_Ic_Config(72 - 1, 65536 - 1);
	//Dma_Init(DMA1_Channel4);
	Adc_Config();

	while (1)
	{
		temp = adc_get_result_average(ADC_CHANNEL_6, 10); /* 读取到的数字量 */
		ave = 3.3*(temp/4096.0);
		printf("%.2f\r\n", ave);
		delay_ms(1000);
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
