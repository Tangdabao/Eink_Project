#include "Resource.h"

extern CmdFlash   CmdFlash_st;
extern CmdFromUart  CmdFromUart_st;

typedef int (*pfnCmdLine)(int argc, uint8_t *argv);

typedef struct
{
    const char pcCmd;
    pfnCmdLine  pfnCmd;
    const char *pcHelp;
} tCmdLineEntry;

tCmdLineEntry *pCmdEntry;

/************************************************
函数名称 ： Test_parameters
功    能 ： CMD 测试指令，用于测试Uart与CMD是否正常
参    数 ： 无
返 回 值 ： 无
说    明 ： 无 
*************************************************/
int Test_parameters(int argc, uint8_t *argv)
{
    uint8_t txd_len = 3;
    uint8_t sendbuf[16] = {Preamble_Data_Msb, Preamble_Data_Lsb, TEST_CMD};
    for(int i = 0; i < txd_len; i++)
        UART2_SendByte(sendbuf[i]);

    return 0;
}

/************************************************
函数名称 ： Write_parameters
功    能 ： 暂未开放
参    数 ： 无
返 回 值 ： 无
说    明 ： 无 
*************************************************/
int Write_parameters(int argc, uint8_t *argv)
{
    return 0;
}


/************************************************
函数名称 ： Refresh_Display
功    能 ： 刷新显示指令
参    数 ： 无
返 回 值 ： 无
说    明 ： 显示不同的Pages
*************************************************/
int Refresh_Display(int argc, uint8_t *argv)
{
    uint8_t sendbuf[16] = {Preamble_Data_Msb, Preamble_Data_Lsb, REFRESH_DIS};
    uint8_t txd_len = 4;
    switch(argv[0])
    {
    case 1:
        CmdFlash_st.Flash_BassAddr = DIS_RAM_BASS;
        break;
    case 2:
        CmdFlash_st.Flash_BassAddr = DIS_TEST_PAGE1;
        break;
    case 3:
        CmdFlash_st.Flash_BassAddr = DIS_TEST_PAGE2;
        break;
    case 4:
        CmdFlash_st.Flash_BassAddr = DIS_TEST_PAGE3;
        break;
    case 5:
        CmdFlash_st.Flash_BassAddr = ASCII_BASS;
        break;
    case 6:
        CmdFlash_st.Flash_BassAddr = HZK8_16_SONG_BASS;
        break;    
    default :
        UART2_SendByte(NACK);
    }
    for(int i = 0; i < txd_len; i++)
        UART2_SendByte(sendbuf[i]);
    
    CmdFromUart_st.acmdDisplay_Start = 1;

    return 0;
}

/************************************************
函数名称 ： Read_Flash
功    能 ：读Flash指令
参    数 ： 无
返 回 值 ： 无
说    明 ：无
*************************************************/
int Read_Flash(int argc, uint8_t *argv)
{
    uint8_t rxd_temp=0;
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_READ_DATA);
    W25Q_WriteByte((uint8_t)argv[0]);
    W25Q_WriteByte((uint8_t)argv[1]);
    W25Q_WriteByte((uint8_t)argv[2]);
    CmdFlash_st.WriteFlash_Len = argv[3];
    CmdFlash_st.WriteFlash_Len <<= 8;
    CmdFlash_st.WriteFlash_Len |= argv[4];
    CmdFlash_st.WriteFlash_Len <<= 8;
    CmdFlash_st.WriteFlash_Len |= argv[5];

    while(CmdFlash_st.WriteFlash_Len--)
    {
        rxd_temp = W25Q_ReadByte();
        UART2_SendByte(rxd_temp);
    }
    W25Q_CS_DISABLE;
    return 0;
}

/************************************************
函数名称 ： Eraser_Flash
功    能 ： 擦除Flash指令
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
int Eraser_Flash(int argc, uint8_t *argv)
{
    uint16_t EraseFlash_Addr;
    uint16_t  EraseFlash_Num;
    switch(argv[0])
    {
    //Erase chip
    case 1:
        if((argv[1] == 0xAA) & (argv[2] == 0xAA))
            W25Q_EraseChip();
        UART2_SendByte(ACK);
        break;

    //Erase block
    case 2:
        EraseFlash_Addr = argv[1];
        EraseFlash_Num  = argv[2];
        //flash最大64块
        if ((EraseFlash_Addr < 64)&&(EraseFlash_Num < 64))
        {
         for(uint8_t i = 0; i < EraseFlash_Num; i++)
            W25Q_EraseBlock(EraseFlash_Addr++);
            UART2_SendByte(ACK);
        }else
        {
            UART2_SendByte(NACK);//参数错误，Block不能大于64
        }
        break;

    //Erase sec.
    case 3:
        EraseFlash_Addr = argv[1];
        EraseFlash_Addr <<= 8;
        EraseFlash_Addr |= argv[2];

        EraseFlash_Num = argv[3];
        EraseFlash_Num <<= 8;
        EraseFlash_Num |= argv[4];
       //flash最大1024Sector
        if ((EraseFlash_Addr < 1024)&&(EraseFlash_Num < 1024))
        {
         for(uint16_t i = 0; i < EraseFlash_Num; i++)
            W25Q_EraseSector(EraseFlash_Addr++);
         UART2_SendByte(ACK); 
        }else
        {
         UART2_SendByte(NACK);//参数错误，Sector不能大于1024
        }
        break;
    default:
        UART2_SendByte(NACK);
    }
    return 0;
}

/************************************************
函数名称 ： Write_Flash
功    能 ： 写Flash指令
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
int Write_Flash(int argc, uint8_t *argv)
{

    CmdFlash_st.WriteFlash_Flag = 1;
    CmdFlash_st.WriteAddrFlash = 0;
    //写地址
    CmdFlash_st.WriteAddrFlash = argv[0];
    CmdFlash_st.WriteAddrFlash <<= 8;
    CmdFlash_st.WriteAddrFlash |= argv[1];
    CmdFlash_st.WriteAddrFlash <<= 8;
    CmdFlash_st.WriteAddrFlash |= argv[2];
    //写长度
    CmdFlash_st.WriteFlash_Len = argv[3];
    CmdFlash_st.WriteFlash_Len <<= 8;
    CmdFlash_st.WriteFlash_Len |= argv[4];
    CmdFlash_st.WriteFlash_Len <<= 8;
    CmdFlash_st.WriteFlash_Len |= argv[5];
    
    CmdFlash_st.WriteFlash_Len = CmdFlash_st.WriteFlash_Len + CmdFlash_st.WriteAddrFlash;//此处注意，，WriteFlash_Len用于判断数据是否接受完毕（Uart服务函数）
      
    //回ACK
    UART2_SendByte(ACK);
    return 0;
}

/************************************************
函数名称 ： ErrorReport
功    能 ： CMD错误指令报告
参    数 ： 无
返 回 值 ： 无
说    明 ： 无 
*************************************************/
int ErrorReport(int argc, uint8_t *argv)
{
  char *Error_Report="This is Error Cmd\r\n";
  UART2_SendString(Error_Report);
  return 0;
}

/************************************************
功    能 ： CMD指令表
参    数 ： 无
返 回 值 ： 无
说    明 ： 添加指令请在下表添加
*************************************************/
tCmdLineEntry g_sCmdTable[] =
{
    { TEST_CMD,   Test_parameters,  " Test_parameters "}, //01
    { WRITE_PARA,  Write_parameters, " Write_parameters "},//02
    { REFRESH_DIS, Refresh_Display,  " Refresh_Display "}, //03
    { READ_FLASH,  Read_Flash,       " Read_Flash "},      //04
    { ERASER_FLASH,Eraser_Flash,     " Eraser_Flash "},    //05
    { WRITE_FLASH, Write_Flash,      " Write_Flash "},    //06
    { 0 ,ErrorReport,"Error" }
};

/************************************************
函数名称 ： CmdLineProcess
功    能 ： CMD指令处理函数
参    数 ： 无
返 回 值 ： 无
说    明 ： 无 
*************************************************/
uint8_t uCmdStrLength, uFunStrLength;
int CmdLineProcess(void)
{
    uint8_t *pFunStr;
    pFunStr = (uint8_t *)&CmdFromUart_st.CmdBuff[3];
    uFunStrLength = CmdFromUart_st.CmdLenth - 3;
    pCmdEntry = &g_sCmdTable[0];
    while(pCmdEntry->pcCmd)
    {
        if(CmdFromUart_st.CmdBuff[2] == pCmdEntry->pcCmd)
        {
            return(pCmdEntry->pfnCmd(uFunStrLength, pFunStr));
        }
        pCmdEntry++;
    }
    return 0;
}

/************************************************
函数名称 ： Cmd_Init
功    能 ： CMD相关变量初始化
参    数 ： 无
返 回 值 ： 无
说    明 ： 无 
*************************************************/
void Cmd_Init(void)
{
 CmdFromUart_st.CmdLenth = 0;
 CmdFromUart_st.acmdStart = 0;
 CmdFromUart_st.acmdEnd = 0;
 CmdFromUart_st.acmdDisplay_Start = 1;//默认开启第一幅图像
   
 CmdFlash_st.WriteAddrFlash = 0;
 CmdFlash_st.WriteFlashBuff = 0;
 CmdFlash_st.WriteFlash_Flag = 0;
 CmdFlash_st.WriteFlash_Len = 0;
 
}

/************************************************
函数名称 ： HAL_TIM_PeriodElapsedCallback
功    能 ： Uart超时计数器,服务函数
参    数 ： 无
返 回 值 ： 无
说    明 ：用于Uart超时数据处理
*************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
   if(htim->Instance ==TIMx)
  {
    //add you code
    if( CmdFromUart_st.acmdStart != 0)//开始准备Uart超时
    {
      if (CmdFromUart_st.acmdMsCount > FRAME_TIMEOUT_100MS)//接收数据超时，串口接收完毕
      {
        CmdFromUart_st.acmdMsCount = 0;//清除计数器
        CmdFromUart_st.acmdStart  = 0;//清除开始标志
        CmdFromUart_st.acmdEnd = 1;//置位结束标志
      }else
      {
        CmdFromUart_st.acmdMsCount++;//计数器未超时，继续计数
      }
    }else
    {
      CmdFromUart_st.acmdMsCount = 0;//清除计数器
      CmdFromUart_st.acmdStart  = 0;//清除开始标志
    }
   }
  
}