#include "Resource.h"


//此处定义全局变量
CmdFromUart  CmdFromUart_st; //Uart cmd处理结构体变量
CmdFlash   CmdFlash_st;      //Flash cmd处理结构体变量


/************************************************
函数名称 ： InitAll
功    能 ： All初始化
参    数 ： 无
返 回 值 ： 无
说    明 ： 分为底层与应用层
            底层：纯硬件初始化：Uart、adc、timer、rtc等
            应用层：纯逻辑初始化：Cmd处理
            尽量不让两层交叉使用
*************************************************/
void InitAll(void)
{
  HAL_Init();//系统初始化
  
  SystemClock_Config();//系统时钟初始化HSI
  SysTick_init();//滴答定时器
  /* Initialize all configured peripherals */ 
  SysStatus_LED_Init();
  
  Led_Test();//
  
  W25Q_GPIO_Init();
  
  while(W25Q_ReadID() != W25Q32_ID);
  
  W25Q_EraseBlock(0);
  Ms_Delay(200);
  
  EinkGPIO_Init();
  EPD_W21_RESET();
  EinkPower_Status(EINK_ON); 
#if 1
  //Display();
  dis_ascii(10, 0, 8, 16, ASCII_BASS + 'a' * 16); 
  dis_ascii(10, 8, 8, 16, ASCII_BASS + 'b' * 16);
  dis_ascii(10, 16, 8, 16, ASCII_BASS + 'c' * 16);
  dis_ascii(10, 24, 8, 16, ASCII_BASS + 'd' * 16);
  
  dis_ascii(50, 50, 8, 16, ASCII_BASS + 'A' * 16);
  dis_ascii(50, 58, 8, 16, ASCII_BASS + 'B' * 16);
  dis_ascii(50, 66, 8, 16, ASCII_BASS + 'C' * 16);
  
  dis_ascii(100, 20, 8, 16, ASCII_BASS + 16);
  dis_ascii(100, 40, 8, 16, ASCII_BASS + 16);
  dis_ascii(100, 60, 8, 16, ASCII_BASS + 32);
  dis_ascii(100, 80, 8, 16, ASCII_BASS + 32);
  
  dis_Hz(150,0,16,16,"张");
  
  dis_Hz(150,32,16,16,"啊");
  
  dis_Hzstring(200,0,16,16,"每当我找到成功的钥匙，就发现有人把锁给换了。");
  
  dis_Hzstring(250,0,16,16,"努力不一定有成效，不努力至少过得舒服。");
   
#else
  
  Title_Rect_Fill();
  
  Pic_Fill(100,0,400,48,DIS_TEST_PAGE2);
  
  dis_Hz(200,120+32,16,16,"啊");
  
#endif
  
  CmdFlash_st.Flash_BassAddr = DIS_RAM_BASS; 
  
  //MX_USART2_UART_Init(BAUD_RATE);
  Data_TimeOut_Init();
  
  /* Initialize all App Demo */
  Cmd_Init();

  	
}

