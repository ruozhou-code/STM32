#include "main.h"

/* 要循环发送的字符串 */
const uint8_t TEXT_TO_SEND[] = "wu";
uint8_t FUCK[] = "fuck\r\n";

#define SEND_BUF_SIZE (sizeof(TEXT_TO_SEND)*2 + 200)

/* 要发送的长度 */
uint8_t g_sendbuf[SEND_BUF_SIZE];

int main(void)
{
	uint32_t i = 0;
	uint16_t k;
	uint8_t mask = 0;
	int flag = 1, len = 0;
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
	//Tim2_Ic_Config(72 - 1, 65536 - 1);
	Dma_Init(DMA1_Channel4);

	len = strlen(TEXT_TO_SEND);
	k = 0;

	for (i = 0;i < SEND_BUF_SIZE; i++)
	{
		if (k >= len)
		{
			if (mask)

			{
				g_sendbuf[i] = 0x0a;
				k = 0;
				break;
			}
			else
			{
				g_sendbuf[i] = 0x0d;
				mask++;
			}
		}
		else
		{
			mask = 0;
			g_sendbuf[i] = TEXT_TO_SEND[k];
			k++;
		}
	}

	i = 0;

	while (1)
	{
		delay_ms(1000);
		HAL_UART_Transmit_DMA(&huart1, FUCK, strlen(FUCK));
		while (1)
		{
			/* 等待DMA1_Channl4传输完成 */
			if (__HAL_DMA_GET_FLAG(&hdma, DMA_FLAG_TC4))
			{
				/* 清除标志位 */
				__HAL_DMA_CLEAR_FLAG(&hdma, DMA_FLAG_TC4);
				HAL_UART_DMAStop(&huart1);
				break;
			}
		}


	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
