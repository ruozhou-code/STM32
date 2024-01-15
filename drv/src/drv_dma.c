#include "drv_dma.h"

DMA_HandleTypeDef hdma;

/* 使用通道4通道5  TX——>channel4 RX——>channel5 */

void Dma_Init(DMA_Channel_TypeDef* DMAx_CHx)
{
    /* enable the clock of DMA1 */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /*link the dma with usart1*/
    __HAL_LINKDMA(&huart1, hdmatx, hdma);

    hdma.Instance = DMAx_CHx;
    hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.Mode = DMA_NORMAL;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma);

}
