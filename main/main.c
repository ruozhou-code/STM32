#include "main.h"




#define ADC_DMA_BUF_SIZE 100			/* 定义采集的的ADC_DMA的数组的大小 */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];         /*存放ADC读取的变量值*/

extern uint8_t g_adc_dma_sta;               /* DMA´«Êä×´Ì¬±êÖ¾, 0,Î´Íê³É; 1, ÒÑÍê³É */


/* 要循环发送的字符串 */
const uint8_t TEXT_TO_SEND[] = "wu";
uint8_t FUCK[] = "fuck\r\n";
#define SEND_BUF_SIZE (sizeof(TEXT_TO_SEND)*2 + 200)
/* 要发送的长度 */
uint8_t g_sendbuf[SEND_BUF_SIZE];

uint32_t temp = 0;
float ave = 0;
uint32_t sum;
uint16_t adcx;
int main(void)
{
	uint16_t i;


	float temp;
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
	adc_dma_init((uint32_t)&g_adc_dma_buf); /* ³õÊ¼»¯ADC DMA²É¼¯ */
	adc_dma_enable(ADC_DMA_BUF_SIZE);   /* Æô¶¯ADC DMA²É¼¯ */

	while (1)
	{
#if 0
		temp = adc_get_result_average(ADC_CHANNEL_6, 10); /* 读取到的数字量 */
		ave = 3.3 * (temp / 4096.0);
		printf("%.2f\r\n", ave);
		delay_ms(1000);
#else
		if (g_adc_dma_sta == 1)
		{
			sum = 0;
			printf("transmit is ok");
			for (i = 0;i < ADC_DMA_BUF_SIZE;i++)
			{
				sum += g_adc_dma_buf[i];
			}
			adcx = sum / ADC_DMA_BUF_SIZE;
			temp = (float)adcx * (3.3 / 4096);
			printf("%d\r\n", adcx);
			g_adc_dma_sta = 0;
			adc_dma_enable(ADC_DMA_BUF_SIZE);
		}
		delay_ms(1000);
#endif
	}
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{

	while (1);
}
#endif
