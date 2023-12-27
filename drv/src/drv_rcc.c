#include "drv_rcc.h"

void RCC_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	//init the outdoor clock source
	RCC_OscInitStruct.HSEPredivValue			= RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSEState					= RCC_HSE_ON;
	RCC_OscInitStruct.HSIState					= RCC_HSI_ON;
	RCC_OscInitStruct.LSEState					= RCC_LSE_ON;
	RCC_OscInitStruct.LSIState					= RCC_LSI_ON;
	RCC_OscInitStruct.OscillatorType			= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL				= RCC_PLL_MUL9;
	RCC_OscInitStruct.PLL.PLLSource				= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLState				= RCC_PLL_ON;
	while(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK);

	//init the peripherals closk source
	RCC_ClkInitStruct.AHBCLKDivider				= RCC_CFGR_HPRE_1;
	RCC_ClkInitStruct.APB1CLKDivider			= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider			= RCC_HCLK_DIV1;
	RCC_ClkInitStruct.ClockType					= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource				= RCC_SYSCLKSOURCE_PLLCLK;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	
}

