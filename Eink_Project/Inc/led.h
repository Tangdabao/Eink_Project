/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Led_H
#define __Led_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef enum
{
   LED_ON= 0U,
   LED_OFF
}LED_State;
/* USER CODE END Private defines */

void SysStatus_LED_Init(void);
void SysStatusLED(LED_State LedState);
void Led_Test(void);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */
