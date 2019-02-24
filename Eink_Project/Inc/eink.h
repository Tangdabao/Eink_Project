/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Eink_H
#define __Eink_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "main.h"
typedef enum
{
   EINK_ON= 0U,
   EINK_OFF
}EINK_State;
void EPD_W21_RESET(void);
void EinkGPIO_Init(void);
void EinkPower_Status(EINK_State EinkState);
void Display(void);
#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */