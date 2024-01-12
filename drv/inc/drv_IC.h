#ifndef __DRV_IC_H

#include "stm32f1xx_hal.h"

extern uint8_t IC_Flag;
extern uint16_t IC_Cnt;

void Tim2_Ic_Config(uint16_t psc, uint16_t arr);

#endif // !__DRV_IC_H
