#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include "stm32f1xx_hal.h"
#include "drv_delay.h"

extern ADC_HandleTypeDef hadc1;
extern uint8_t adc_dma_sta;

#if 0
// /*判断 DMA1_Channel1 传输完成标志, 是一个假函数形式, 不能当函数使用, 只能用在 if 等语句里面*/
// #define ADC_ADCX_DMACx_IS_TC() ( DMA1->ISR & (1 << 1) )
// /* 清除 DMA1_Channel1 传输完成标志 */
// #define ADC_ADCX_DMACx_CLR_TC() do{ DMA1->IFCR |= 1 << 1; }while(0)

void Adc_Config(uint32_t mar);
void adc_channel_set(ADC_HandleTypeDef* adc_handle, uint32_t channel, uint32_t rank, uint32_t cycletime);
uint32_t adc_get_result(uint32_t ch);
uint32_t adc_get_result_average(uint32_t channle, uint8_t times);
void adc_dma_enable(uint16_t cndtr);
#endif


#define ADC_ADCX_CHY_GPIO_PORT              GPIOA
#define ADC_ADCX_CHY_GPIO_PIN               GPIO_PIN_6 
#define ADC_ADCX_CHY_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) 

#define ADC_ADCX                            ADC1 
#define ADC_ADCX_CHY                        ADC_CHANNEL_6                               
#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)  

#define ADC_ADCX_DMACx                      DMA1_Channel1
#define ADC_ADCX_DMACx_IRQn                 DMA1_Channel1_IRQn
#define ADC_ADCX_DMACx_IRQHandler           DMA1_Channel1_IRQHandler

#define ADC_ADCX_DMACx_IS_TC()              ( DMA1->ISR & (1 << 1) )   


#define ADC_ADCX_DMACx_CLR_TC()             do{ DMA1->IFCR |= 1 << 1; }while(0) 

void adc_init(void);
void adc_channel_set(ADC_HandleTypeDef* adc_handle, uint32_t ch, uint32_t rank, uint32_t stime);
uint32_t adc_get_result(uint32_t ch);
uint32_t adc_get_result_average(uint32_t ch, uint8_t times);

void adc_nch_dma_init(uint32_t mar);

void adc_dma_init(uint32_t mar);
void adc_dma_enable(uint16_t cndtr);

#endif // !__DRV_ADC_H

