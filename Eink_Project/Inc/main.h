/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32l0xx_hal.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/*****uart*****/
#define UART_RX_LED_Pin GPIO_PIN_9
#define UART_RX_LED_GPIO_Port GPIOA
#define UART_TX_BUSY_Pin GPIO_PIN_10
#define UART_TX_BUSY_GPIO_Port GPIOA

/*****w25q32*****/
#define W25Q_CS_Pin             GPIO_PIN_1
#define W25Q_CS_Port            GPIOB
#define W25Q_MISO_Pin           GPIO_PIN_7
#define W25Q_MISO_Port          GPIOA
#define W25Q_CLK_Pin            GPIO_PIN_5
#define W25Q_CLK_Port           GPIOA
#define W25Q_MOSI_Pin           GPIO_PIN_6
#define W25Q_MOSI_Port          GPIOA

#define W25Q_CS_ENABLE             (W25Q_CS_Port->BRR  = W25Q_CS_Pin)
#define W25Q_CS_DISABLE            (W25Q_CS_Port->BSRR = W25Q_CS_Pin)

#define W25Q_CLK_LOW               (W25Q_CLK_Port->BRR  = W25Q_CLK_Pin)
#define W25Q_CLK_HIGH              (W25Q_CLK_Port->BSRR = W25Q_CLK_Pin)

#define W25Q_MISO_READ             HAL_GPIO_ReadPin(W25Q_MISO_Port,W25Q_MISO_Pin)///(W25Q_MISO_Port->IDR & W25Q_MISO_Pin)

#define W25Q_MOSI_LOW              (W25Q_MOSI_Port->BRR  = W25Q_MOSI_Pin)
#define W25Q_MOSI_HIGH             (W25Q_MOSI_Port->BSRR = W25Q_MOSI_Pin)
    
#define DELAY_SPI_CLK 	           __NOP();///__NOP();//__NOP();__NOP();__NOP();__NOP();__NOP();

/*****Eink*****/
#define EINK_RES_Pin    GPIO_PIN_14
#define EINK_RES_Port   GPIOC

#define EINK_EN_Pin    GPIO_PIN_15
#define EINK_EN_Port   GPIOC 
    
#define EINK_DC_Pin    GPIO_PIN_1
#define EINK_DC_Port   GPIOA

#define EINK_CS_Pin    GPIO_PIN_2
#define EINK_CS_Port   GPIOA

#define EINK_CLK_Pin    GPIO_PIN_3
#define EINK_CLK_Port   GPIOA

#define EINK_SDI_Pin    GPIO_PIN_4
#define EINK_SDI_Port   GPIOA

#define EINK_BUSY_Pin    GPIO_PIN_10
#define EINK_BUSY_Port   GPIOA


#define EINK_EN  (EINK_EN_Port->BSRR = EINK_EN_Pin)
#define EINK_DIS (EINK_EN_Port->BRR  = EINK_EN_Pin)

#define SDA_H    (EINK_SDI_Port->BSRR = EINK_SDI_Pin)
#define SDA_L    (EINK_SDI_Port->BRR  = EINK_SDI_Pin)

#define SCLK_H   (EINK_CLK_Port->BSRR = EINK_CLK_Pin)
#define SCLK_L   (EINK_CLK_Port->BRR  = EINK_CLK_Pin)

#define nCS_H    (EINK_CS_Port->BSRR = EINK_CS_Pin)//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define nCS_L    (EINK_CS_Port->BRR  = EINK_CS_Pin)//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)

#define nDC_H    (EINK_DC_Port->BSRR = EINK_DC_Pin)//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define nDC_L    (EINK_DC_Port->BRR  = EINK_DC_Pin)//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)

#define nRST_H   (EINK_RES_Port->BSRR = EINK_RES_Pin)//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET)
#define nRST_L   (EINK_RES_Port->BRR  = EINK_RES_Pin)//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET)

#define nBUSY    HAL_GPIO_ReadPin(EINK_BUSY_Port,EINK_BUSY_Pin)//(EINK_BUSY_Port->IDR & EINK_BUSY_Pin)//HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)



/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */



/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
