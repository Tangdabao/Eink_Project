/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
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
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

/* USER CODE BEGIN 0 */
#include "Resource.h"
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef  timer2;
extern CmdFromUart        CmdFromUart_st;
extern CmdFlash           CmdFlash_st;
/******************************************************************************/
/*            Cortex-M0+ Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
void USART2_IRQHandler(void)
{
  if((USART2->ISR & UART_FLAG_ORE) != RESET) /*!< UART overrun error*/
  {
    USART2->ICR = UART_CLEAR_OREF;      /*!< Overrun Error Clear Flag*/
  }
  if(((USART2->ISR & UART_FLAG_RXNE) != RESET) &&((USART2->CR1 &USART_CR1_RXNEIE) !=RESET))
  { 
    if(CmdFromUart_st.CmdLenth == 0)//用于判断Uart 是否有数据进来
    {
      
      CmdFromUart_st.acmdStart = 1; //开始接收数据标志位
      
    }else
    {
      
      CmdFromUart_st.acmdMsCount  = 0;//计数器清零
    
    }
    
    if (CmdFlash_st.WriteFlash_Flag != 0)//判断是否需要写Flash,如果需要，把数据写入Flash
    {
      //CmdFromUart_st.CmdBuff[CmdFromUart_st.CmdLenth] = (uint8_t) (USART2->RDR & 0xFF);
      CmdFlash_st.WriteFlashBuff = (uint8_t) (USART2->RDR & 0xFF);
      
      W25Q_WritePage(&CmdFlash_st.WriteFlashBuff,CmdFlash_st.WriteAddrFlash, 1);
      
      CmdFlash_st.WriteAddrFlash++;
      
      if (CmdFlash_st.WriteAddrFlash == CmdFlash_st.WriteFlash_Len)//判断写入的数据与文件数据长度是否一致
      {
        CmdFlash_st.WriteFlash_Flag = 0;//清除写Flash标志
        UART2_SendByte(ACK);//Flash写完
        CmdFromUart_st.CmdLenth = 0;
      }//add timerout fun
       
    }else//不需要写Flash，进行指令判断
    {
     CmdFromUart_st.CmdBuff[CmdFromUart_st.CmdLenth] = (uint8_t) (USART2->RDR & 0xFF);
     CmdFromUart_st.CmdLenth++;
    }
  }
  
  USART2->RQR = 1 << 3; //clears the RXNE flag
  /* USER CODE END USART2_IRQn 1 */
}


void TIM2_IRQHandler(void)
{
 HAL_TIM_IRQHandler(&timer2);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
