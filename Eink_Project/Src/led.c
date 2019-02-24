#include "Resource.h"

extern UART_HandleTypeDef huart2;
/************************************************
�������� �� SysStatus_LED_Init
��    �� ��LEDָʾ��
��    �� �� ��
�� �� ֵ �� ��
˵    �� ������LEDָʾ
*************************************************/
void SysStatus_LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  HAL_UART_MspDeInit(&huart2);//LED��UART���ã������Ҫ��DeinitUart
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_WritePin(UART_RX_LED_GPIO_Port,UART_RX_LED_Pin,GPIO_PIN_SET);
  GPIO_InitStruct.Pin = UART_RX_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(UART_RX_LED_GPIO_Port, &GPIO_InitStruct);
}


/************************************************
�������� �� SysStatusLED
��    �� ��LED���غ���
��    �� �� LED_State LedState
�� �� ֵ �� ��
˵    �� ��LED���غ���
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
�������� �� Led_Test
��    �� ��LED ����
��    �� �� 
�� �� ֵ �� ��
˵    �� ��
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
