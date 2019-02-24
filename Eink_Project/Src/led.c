#include "Resource.h"

extern UART_HandleTypeDef huart2;
/************************************************
函数名称 ： SysStatus_LED_Init
功    能 ：LED指示灯
参    数 ： 无
返 回 值 ： 无
说    明 ：用于LED指示
*************************************************/
void SysStatus_LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  HAL_UART_MspDeInit(&huart2);//LED与UART复用，因此需要先DeinitUart
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_WritePin(UART_RX_LED_GPIO_Port,UART_RX_LED_Pin,GPIO_PIN_SET);
  GPIO_InitStruct.Pin = UART_RX_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(UART_RX_LED_GPIO_Port, &GPIO_InitStruct);
}


/************************************************
函数名称 ： SysStatusLED
功    能 ：LED开关函数
参    数 ： LED_State LedState
返 回 值 ： 无
说    明 ：LED开关函数
*************************************************/
void SysStatusLED(LED_State LedState)
{
  if (LedState != LED_ON)
  { 
    GPIOA->BSRR = UART_RX_LED_Pin;
  }
  else
  {
    GPIOA->BRR = UART_RX_LED_Pin ;
  }
}



/************************************************
函数名称 ： Led_Test
功    能 ：LED 测试
参    数 ： 
返 回 值 ： 无
说    明 ：
*************************************************/

void Led_Test(void)
{
  for(uint8_t i = 0; i <5; i++)
  {
    SysStatusLED(LED_ON);
    Ms_Delay(200);
    SysStatusLED(LED_OFF);
    Ms_Delay(200);
    
  }
}
