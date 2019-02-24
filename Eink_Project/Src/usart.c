/**
******************************************************************************
* File Name          : USART.c
* Description        : This file provides code for the configuration
*                      of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/************************************************
函数名称 ： MX_USART2_UART_Init
功    能 ： UART2初始化，用于数据传输
参    数 ： BaudRate：波特率
返 回 值 ： 无
说    明 ：时钟由Pclk1提供32M支持
*************************************************/
void MX_USART2_UART_Init(uint32_t BaudRate)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  huart2.Instance = USART2;
  huart2.Init.BaudRate = BaudRate;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
  
  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART2)
  {
    /* USER CODE BEGIN USART2_MspInit 0 */
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* USER CODE END USART2_MspInit 0 */
    
    HAL_GPIO_DeInit(GPIOA, UART_RX_LED_Pin|UART_TX_BUSY_Pin);//uart被复用，先Deinit，确保安全
    
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
    
    /**USART2 GPIO Configuration    
    PA9     ------> USART2_TX
    PA10     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = UART_RX_LED_Pin|UART_TX_BUSY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* USART2 interrupt Init */
    ///HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    //HAL_NVIC_EnableIRQ(USART2_IRQn);
    /* USER CODE BEGIN USART2_MspInit 1 */
    
    __HAL_UART_DISABLE(&huart2);//失能USART
    
    USART2->RQR = 1 << 3; //clears the RXNE flag
    
    USART2->CR1 |= 1 << 5; //enable the RXNE interrupt
    
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    __HAL_UART_ENABLE(&huart2);//使能USART
    /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  
  if(uartHandle->Instance==USART2)
  {
    /* USER CODE BEGIN USART2_MspDeInit 0 */
    
    /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
    
    /**USART2 GPIO Configuration    
    PA9     ------> USART2_TX
    PA10     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, UART_RX_LED_Pin|UART_TX_BUSY_Pin);
    
    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
    /* USER CODE BEGIN USART2_MspDeInit 1 */
    
    /* USER CODE END USART2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void UART2_SendByte(uint8_t data)
{
  while((USART2->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  USART2->TDR = data;
}

void UART2_SendString(char *str)
{
  while((*str) != 0)
  {
    while((USART2->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
    USART2->TDR = *str++;
  }
}
/* USER CODE END 1 */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
