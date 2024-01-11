#ifndef __DRV_PWM_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;

void Pwm_Init(uint32_t period, uint32_t prescaler);

#endif // !__DRV_PWM_H
