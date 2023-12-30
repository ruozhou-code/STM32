#ifndef __DRV_IWDG_H
#define __DRV_IWDG_H

#include "stm32f1xx_hal.h"

void iwdg_init(uint8_t prer, uint16_t rlr);
void iwdg_feed(void);

#endif // !__DRV_IWDG_H

