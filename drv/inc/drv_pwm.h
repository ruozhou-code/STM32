#ifndef __DRV_PWM_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;

void Pwm_Init(uint32_t period, uint32_t prescaler);
void GPIOA_Set_Compare(uint16_t PA0, uint16_t PA1, uint16_t PA2);

#endif // !__DRV_PWM_H
