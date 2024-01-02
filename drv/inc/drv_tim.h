#ifndef __DRV_BASETIME_H
#define __DRV_BASETIME_H

#include "stm32f1xx_hal.h"
#include "drv_led.h"

void Tim_Config(uint32_t prescaler, uint32_t counter);

#endif // !__DRV_BASETIME_H

