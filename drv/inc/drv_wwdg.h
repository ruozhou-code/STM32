#ifndef __DRV_WWDG_H

#include "stm32f1xx_hal.h"
#include "drv_led.h"

void Wwdg_Config(uint32_t prescale, uint32_t counter, uint32_t window);

#endif // !__DRV_WWDG_H
