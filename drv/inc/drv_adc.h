#ifndef __DRV_ADC_H

#include "stm32f1xx_hal.h"
#include "drv_delay.h"

extern ADC_HandleTypeDef hadc1;

void Adc_Config(void);
void adc_channel_set(ADC_HandleTypeDef* adc_handle, uint32_t channel, uint32_t rank, uint32_t cycletime);
uint32_t adc_get_result(uint32_t ch);
uint32_t adc_get_result_average(uint32_t channle, uint8_t times);

#endif // !__DRV_ADC_H
