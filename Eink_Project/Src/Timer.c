#include "timer.h"
static uint32_t fac_us = 0;

TIM_HandleTypeDef timer2;

/************************************************
函数名称 ： SystemClock_Config
功    能 ： 系统时钟初始化
参    数 ： 无
返 回 值 ： 无
说    明 ：HSI：ON （16M）
SYSCLK:ON（32M）
HCLK:ON(32M)
PCLK:ON(32M)
*************************************************/
void SystemClock_Config(void)
{
  
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  
  
  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
}

/************************************************
函数名称 ： SysTick_init
功    能 ： 滴答定时器初始化
参    数 ： 无
返 回 值 ： 无
说    明 ： Us中断，中断优先级0，0
*************************************************/
void SysTick_init(void)
{
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  fac_us = 32;
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  
}
/************************************************
函数名称 ： Us_Delay
功    能 ： 延时函数Us
参    数 ： 无
返 回 值 ： 无
说    明 ：勿使用，Hal_Delay
*************************************************/
void Us_Delay(uint32_t Delay)
{
  uint32_t ticks;
  uint32_t told, tnow, tcnt = 0;
  uint32_t reload = SysTick->LOAD;
  ticks = Delay * fac_us;
  told = SysTick->VAL;
  while(1)
  {
    tnow = SysTick->VAL;
    if(tnow != told)
    {
      if(tnow < told)
        tcnt += told - tnow;
      else
        tcnt += reload - tnow + told;
      told = tnow;
      if(tcnt >= ticks)
        break;
    }
  };
}

/************************************************
函数名称 ： Ms_Delay
功    能 ： 延时函数Ms
参    数 ： 无
返 回 值 ： 无
说    明 ：勿使用，Hal_Delay
*************************************************/
void Ms_Delay(uint32_t Delay)
{
  uint32_t i;
  for(i = 0; i < Delay; i++)
    Us_Delay(1000);
}

/************************************************
函数名称 ： Data_TimeOut_Init
功    能 ： Uart超时计数器
参    数 ： 无
返 回 值 ： 无
说    明 ：用于Uart超时计数
*************************************************/
void Data_TimeOut_Init(void)
{
  TIMx_CLK_ENABLE();
  timer2.Instance = TIMx;
  timer2.Init.Period = 2 - 1;//计数值，1Khz  1*1/1khz=1ms
  timer2.Init.Prescaler = 32000 - 1;//预分频寄存器，系统时钟32M，因此32M/32000==1kHz
  timer2.Init.ClockDivision = 0;
  timer2.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&timer2) != HAL_OK)
  {   
  }
  
  __HAL_TIM_ENABLE_IT(&timer2, TIM_IT_UPDATE);
  __HAL_TIM_CLEAR_IT(&timer2, TIM_IT_UPDATE);

  HAL_NVIC_SetPriority(TIMx_IRQn, 3, 0);

  HAL_NVIC_EnableIRQ(TIMx_IRQn);

  __HAL_TIM_ENABLE(&timer2);
}

