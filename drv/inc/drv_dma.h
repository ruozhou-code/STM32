#ifndef __DRV_DMA_H

#include "stm32f1xx_hal.h"
#include "drv_usart.h"

extern DMA_HandleTypeDef hdma;
void Dma_Init(DMA_Channel_TypeDef* DMAx_CHx);

#endif // !__DRV_DMA_H


