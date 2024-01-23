#ifndef __DRV_TIM1_H

#include "stm32f1xx_hal.h"

void Tim1_Config(uint16_t psc, uint16_t per);
void tim1_npwm_set(uint32_t cnt);
void Tim1_setcompare(uint16_t i, uint16_t j);

#endif // !__DRV_TIM1_H
