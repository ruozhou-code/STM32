#ifndef __DRV_KEY_H
#define __DRV_KEY_H

#include "stm32f1xx.h"

extern uint8_t key_PressRelease_flag;

#define KEY_IS_PRESS 1
#define KEY_NO_PRESS 0

#define KEY_READ HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)

void key_init(void);
char IsKeyPressing(void);
void IsKeyPressThenRelease(void);



#endif // !__DRV_KEY_H

