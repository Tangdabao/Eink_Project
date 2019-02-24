#ifndef __Timer_H
#define __Timer_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "main.h"

#define TIMx                           TIM2
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()
/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM2_IRQn
#define TIMx_IRQHandler                TIM2_IRQHandler

   
void SystemClock_Config(void);
void Us_Delay(uint32_t Delay);
void SysTick_init(void);
void Ms_Delay(uint32_t Delay);
void Data_TimeOut_Init(void);
#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
