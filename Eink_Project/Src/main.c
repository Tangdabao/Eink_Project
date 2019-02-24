
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
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
#include "main.h"
#include "Resource.h"
int main(void)
{
  InitAll();
  
  while (1)
  {
    if((CmdFromUart_st.acmdEnd != 0) && (CmdFlash_st.WriteFlash_Flag == 0))//Uart time out & No Write Flash
    {
      if( ( CmdFromUart_st.CmdBuff[0] == Preamble_Data_Msb) && ( CmdFromUart_st.CmdBuff[1] == Preamble_Data_Lsb) )
      {
        CmdLineProcess();
        //clear
        Osal_MemSet(CmdFromUart_st.CmdBuff,0,CmdFromUart_st.CmdLenth);
        CmdFromUart_st.CmdLenth = 0;
        CmdFromUart_st.acmdEnd = 0;
      }
      else
      {
        Osal_MemSet(CmdFromUart_st.CmdBuff,0,CmdFromUart_st.CmdLenth);
        CmdFromUart_st.CmdLenth = 0;
        CmdFromUart_st.acmdEnd= 0;
      }
    }else
    { 
       Ms_Delay(10);
    }
    
    while(CmdFlash_st.WriteFlash_Flag);
    
    if(CmdFromUart_st.acmdDisplay_Start == 1)
    {
      //HAL_UART_MspDeInit(&huart2);
      HAL_GPIO_DeInit(GPIOA, UART_RX_LED_Pin|UART_TX_BUSY_Pin);
      EinkGPIO_Init();
      EPD_W21_RESET();
      EinkPower_Status(EINK_ON); 
  
      Display();
      
      MX_USART2_UART_Init(BAUD_RATE);
      CmdFromUart_st.acmdDisplay_Start = 0;
    }
  }
}

/**
* @brief  This function is executed in case of error occurrence.
* @param  file: The file name as string.
* @param  line: The line in file as a number.
* @retval None
*/
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
