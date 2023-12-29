#ifndef __DRV_USART_H
#define __DRV_USART_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f1xx_hal.h"

#define REC_LENGTH  1
#define MAX_REC_LENGTH  1024 

extern unsigned char UART1_Rx_Buf[MAX_REC_LENGTH];
extern unsigned char UART1_Rx_flg;
extern unsigned int  UART1_Rx_cnt;
extern unsigned char UART1_temp[REC_LENGTH];

extern unsigned char LED_flag;

#define usart_flag 0        /*这个是用来调试用的*/

#if usart_flag
extern USART_HandleTypeDef husart;
#else
extern UART_HandleTypeDef huart1;
#endif

void USART_INIT(void);

#endif
