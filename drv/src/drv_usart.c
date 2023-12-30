#include "drv_usart.h"

unsigned char UART1_Rx_Buf[MAX_REC_LENGTH] = { 0 }; // USART1存储接收数据
unsigned char UART1_Rx_flg = 0;					  // USART1接收完成标志
unsigned int UART1_Rx_cnt = 0;					  // USART1接受数据计数器
unsigned char UART1_temp[REC_LENGTH] = { 0 };		  // USART1接收数据缓存

unsigned char LED_flag = 0;

#if usart_flag
USART_HandleTypeDef husart1;
#else
UART_HandleTypeDef huart1;
#endif

void USART_INIT(void)
{
#if usart_flag
	husart.Init.BaudRate = 115200;
	husart.Init.Mode = USART_MODE_TX_RX;
	husart.Init.Parity = USART_PARITY_NONE;
	husart.Init.StopBits = USART_STOPBITS_1;
	husart.Init.WordLength = USART_WORDLENGTH_8B;
	husart.Init.Instance = USART1;
	// while(HAL_USART_Init(&husart)!=HAL_OK);
	while (HAL_USART_Init(&husart) != HAL_OK);
#else
	huart1.Init.BaudRate = 115200;
	huart1.Init.Mode = USART_MODE_TX_RX;
	huart1.Init.Parity = USART_PARITY_NONE;
	huart1.Init.StopBits = USART_STOPBITS_1;
	huart1.Init.WordLength = USART_WORDLENGTH_8B;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Instance = USART1;
	HAL_UART_Init(&huart1);

	HAL_UART_Receive_IT(&huart1, (uint8_t*)UART1_temp, 1);
#endif
}
#if usart_flag

void HAL_USART_MspInit(USART_HandleTypeDef* husart)
{
	GPIO_InitTypeDef GPIO_Init;
	if (husart->Instance == USART1)
	{
		/* Enable the PA9 & PA10 clock */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/* Enable the USART Clock*/
		__HAL_RCC_USART1_CLK_ENABLE();

		/* Init the Pin of PA9 and PA10 */
		GPIO_Init.Mode = GPIO_MODE_AF_PP; // PA9 TX
		GPIO_Init.Pin = GPIO_PIN_9;
		GPIO_Init.Pull = GPIO_PULLUP;
		GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_Init);

		GPIO_Init.Mode = GPIO_MODE_AF_INPUT; // PA10 RX
		GPIO_Init.Pin = GPIO_PIN_10;
		HAL_GPIO_Init(GPIOA, &GPIO_Init);
	}
}
#else

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_Init;
	if (huart->Instance == USART1)
	{
		/* Enable the PA9 & PA10 clock */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/* Enable the USART Clock*/
		__HAL_RCC_USART1_CLK_ENABLE();

		/* Init the Pin of PA9 and PA10 */
		GPIO_Init.Mode = GPIO_MODE_AF_PP; // PA9 TX
		GPIO_Init.Pin = GPIO_PIN_9;
		GPIO_Init.Pull = GPIO_PULLUP;
		GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_Init);

		GPIO_Init.Mode = GPIO_MODE_AF_INPUT; // PA10 RX
		GPIO_Init.Pin = GPIO_PIN_10;
		GPIO_Init.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_Init);

		HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}
#endif

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1); /*调用这个函数会关闭中断*/
	HAL_UART_Receive_IT(&huart1, (uint8_t*)UART1_temp, REC_LENGTH);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{

	if (huart->Instance == USART1)
	{
		UART1_Rx_Buf[UART1_Rx_cnt] = UART1_temp[0];
		UART1_Rx_cnt++;

		if (0x0a == UART1_temp[0])
		{
			UART1_Rx_flg = 1;
			UART1_Rx_Buf[UART1_Rx_cnt] = '\0';
			if (strstr(UART1_Rx_Buf, "open") != NULL)
			{
				LED_flag = 1;
			}
			else if (strstr(UART1_Rx_Buf, "close") != NULL)
			{
				LED_flag = 0;
			}
		}
		
		if (UART1_Rx_flg)
		{
			HAL_UART_Transmit(&huart1, UART1_Rx_Buf, UART1_Rx_cnt, 0x10); // 发送接收到的数据
			for (int i = 0; i < UART1_Rx_cnt; i++)
				UART1_Rx_Buf[i] = 0;
			UART1_Rx_cnt = 0;
			UART1_Rx_flg = 0;
		}
		
		HAL_UART_Receive_IT(&huart1, (uint8_t*)UART1_temp, REC_LENGTH);
	}
}

int fputc(int ch, FILE* f)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 10);
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != SET);
	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);
	return ch;
}

int fgetc(FILE* f)
{
	uint8_t ch = 0;
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != SET)
		;
	HAL_UART_Receive(&huart1, (uint8_t*)&ch, 1, 10);
	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	return (int)ch;
}
