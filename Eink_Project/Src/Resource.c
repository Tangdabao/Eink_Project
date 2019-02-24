#include "Resource.h"


//�˴�����ȫ�ֱ���
CmdFromUart  CmdFromUart_st; //Uart cmd����ṹ�����
CmdFlash   CmdFlash_st;      //Flash cmd����ṹ�����


/************************************************
�������� �� InitAll
��    �� �� All��ʼ��
��    �� �� ��
�� �� ֵ �� ��
˵    �� �� ��Ϊ�ײ���Ӧ�ò�
            �ײ㣺��Ӳ����ʼ����Uart��adc��timer��rtc��
            Ӧ�ò㣺���߼���ʼ����Cmd����
            �����������㽻��ʹ��
*************************************************/
void InitAll(void)
{
  HAL_Init();//ϵͳ��ʼ��
  
  SystemClock_Config();//ϵͳʱ�ӳ�ʼ��HSI
  SysTick_init();//�δ�ʱ��
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
  
  dis_Hz(150,0,16,16,"��");
  
  dis_Hz(150,32,16,16,"��");
  
  dis_Hzstring(200,0,16,16,"ÿ�����ҵ��ɹ���Կ�ף��ͷ������˰��������ˡ�");
  
  dis_Hzstring(250,0,16,16,"Ŭ����һ���г�Ч����Ŭ�����ٹ��������");
   
#else
  
  Title_Rect_Fill();
  
  Pic_Fill(100,0,400,48,DIS_TEST_PAGE2);
  
  dis_Hz(200,120+32,16,16,"��");
  
#endif
  
  CmdFlash_st.Flash_BassAddr = DIS_RAM_BASS; 
  
  //MX_USART2_UART_Init(BAUD_RATE);
  Data_TimeOut_Init();
  
  /* Initialize all App Demo */
  Cmd_Init();

  	
}

