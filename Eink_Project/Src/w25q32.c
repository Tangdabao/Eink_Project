#include "W25q32.h"


/*****************************spi FLASH信息**********************************

W25Q32：
1.容量：32M Bit/4Mbyte（4194304）
2.存储结构：

页（page）   :256Bytes
扇区(Sector) :4Kbytes
块（Block）  ：64Kbytes


页：16384
扇区：1024
块：64

3.速度：
80Mhz

4.状态控制寄存器命令（默认0X00）

Bit 7     6      5        4       3       2       1       0
   SPR    RV    TB      BP2     BP1     BP0     WEL     BUSY

SPR：默认为0，状态寄存器保护位
TB、BP2--0,：Flash区域写保护设置
WEL;写使能锁定
BUSY：判忙标志（1：忙，0：不忙）

注意：
1、页为编程单位，可以一次性编程1个到256个字节；超过256个字节肯定要分多次写入

2、在编程之前，必须对对应的区域进行擦除操作，否则有可能写入错误。

3、擦除的最小单位是“扇区”，也可以以“块”为单位进行擦除（此时块可以为32K-bytes 或者 64K-bytes），最大可以整块擦除。

4、读操作比编程操作容易的多，没有以上的细节考虑。可以一次性读一个字节，也可以多个字节，甚至从头读到尾。
***************************************************************************/

/************************************************
函数名称 ： W25_GPIO_Init
功    能 ： W25Q32 模拟SPI的GPIO初始化
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = W25Q_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(W25Q_CS_Port, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = W25Q_CLK_Pin | W25Q_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(W25Q_CLK_Port, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = W25Q_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(W25Q_MISO_Port, &GPIO_InitStruct);
    
    W25Q_CS_DISABLE;
    W25Q_CLK_HIGH;
    W25Q_MOSI_HIGH;
}
/************************************************
函数名称 ： W25Q_WriteByte
功    能 ： W25Q32 SPI写一个字节数据
参    数 ： TxData  ：发送的字节数据
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_WriteByte(uint8_t TxData)
{
    uint8_t cnt;

    for(cnt = 0; cnt < 8; cnt++)
    {
        W25Q_CLK_LOW;                                 //时钟 - 低
        DELAY_SPI_CLK;

        if(TxData & 0x80)                            //发送数据
            W25Q_MOSI_HIGH;
        else
            W25Q_MOSI_LOW;
        TxData <<= 1;

        DELAY_SPI_CLK;
        W25Q_CLK_HIGH;                                //时钟 - 高
        DELAY_SPI_CLK;
    }
}

/************************************************
函数名称 ： W25Q_ReadByte
功    能 ： W25Q32 SPI读一个字节数据
参    数 ： 无
返 回 值 ： 读回来的数据
说    明 ： 无
*************************************************/
uint8_t W25Q_ReadByte(void)
{
    uint8_t cnt;
    uint8_t RxData = 0;

    for(cnt = 0; cnt < 8; cnt++)
    {
        W25Q_CLK_LOW;
        DELAY_SPI_CLK;

        RxData <<= 1;
        if(W25Q_MISO_READ)
        {
            RxData |= 0x01;
        }

        W25Q_CLK_HIGH;
        DELAY_SPI_CLK;
    }

    return RxData;
}

/************************************************
函数名称 ： W25Q_WriteEnable
功    能 ： W25Q32 写使能，置位WEL
参    数 ： 无
返 回 值 ：无
说    明 ： 无
*************************************************/
static void W25Q_WriteEnable(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_WRITE_ENABLE);//写使能
    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_WriteDisable
功    能 ： W25Q32 写禁止，WEL清零
参    数 ： 无
返 回 值 ：无
说    明 ： 无
*************************************************/
static void W25Q_WriteDisable(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_WRITE_DISABLE);
    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_ReadSR
功    能 ： 读取状态寄存器
参    数 ： 无
返 回 值 ：byte ：读取的字节
说    明 ： 无
*************************************************/
uint8_t W25Q_ReadSR(void)
{
    uint8_t data_tmp;
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_READ_STATUS_REG);
    data_tmp = W25Q_ReadByte();
    W25Q_CS_DISABLE;
    return data_tmp;
}

/************************************************
函数名称 ： W25Q_WriteSR
功    能 ： 写取状态寄存器
参    数 ： SR：写状态寄存器
返 回 值 ： 
说    明 ： 无
*************************************************/
void W25Q_WriteSR(uint8_t SR)
{
    W25Q_WriteEnable();
    W25Q_WriteByte(SFLASH_WRITE_STATUS_REG);
    W25Q_WriteByte(SR);
    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_ReadNByte
功    能 ： 从Readaddr地址开始连续读取Nbyte
参    数 ： pbuffer：数据存储区首地址
            ReadAddr：要读取的W25Q的首地址
            Nbyte：要读取的字节字节数（最大65535）
返 回 值 ： 
说    明 ： 无
*************************************************/
void W25Q_ReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_READ_DATA);
    W25Q_WriteByte((uint8_t)((ReadAddr) >> 16));
    W25Q_WriteByte((uint8_t)((ReadAddr) >> 8));
    W25Q_WriteByte((uint8_t)ReadAddr);

    while(nByte--)
    {
        *pBuffer = W25Q_ReadByte();
        pBuffer++;
    }

    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_FastReadNByte
功    能 ： 从Readaddr地址开始连续读取Nbyte
参    数 ： pbuffer：数据存储区首地址
            ReadAddr：要读取的W25Q的首地址
            Nbyte：要读取的字节字节数（最大65535）
返 回 值 ： 
说    明 ： 无
*************************************************/
void W25Q_FastReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_FAST_READ);
    W25Q_WriteByte((uint8_t)((ReadAddr) >> 16));
    W25Q_WriteByte((uint8_t)((ReadAddr) >> 8));
    W25Q_WriteByte((uint8_t)ReadAddr);
    W25Q_WriteByte(0xFF);

    while(nByte--)
    {
        *pBuffer = W25Q_ReadByte();
        pBuffer++;
    }

    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_WritePage
功    能 ： 在W25Q内写入少于1pages（256字节）的数据
参    数 ： pbuffer：数据存储区首地址
            ReadAddr：要读取的W25Q的首地址
            Nbyte：要读取的字节字节数（最大1Pages）
返 回 值 ： 
说    明 ： 无
*************************************************/
void W25Q_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    W25Q_WriteEnable();

    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_WRITE_PAGE);
    W25Q_WriteByte((WriteAddr& 0xFF0000)>> 16);
    W25Q_WriteByte((WriteAddr& 0xFF00)>> 8);
    W25Q_WriteByte( WriteAddr & 0xFF);
    while (nByte--)
    {
        W25Q_WriteByte(*pBuffer);
        pBuffer++;
    }

    W25Q_CS_DISABLE;
    W25Q_WaitForNoBusy();
}

/************************************************
函数名称 ： W25Q_WriteNoCheck
功    能 ： 无检验写Flash
          必须确保所写地址都为FFF，否则非FF处会出现错误
          具有自动换页功能
          在制定地址写入制定长度数据，确保不越界

参    数 ： pbuffer：要写入数据区首地址
            writeAddr：要写入flash地址
            Nbyte：要写入的字节数
返 回 值 ： 
说    明 ： 无
*************************************************/
void W25Q_WriteNoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    uint16_t PageRemain = 256 - WriteAddr % 256;

    if(nByte <= PageRemain)
        PageRemain = nByte;

    while(1)
    {
        W25Q_WritePage(pBuffer, WriteAddr, PageRemain);
        if(nByte == PageRemain)
            break;
        else
        {
            pBuffer += PageRemain;
            WriteAddr += PageRemain;
            nByte -= PageRemain;

            if(nByte > 256)
                PageRemain = 256;
            else
                PageRemain = nByte;
        }
    }
}
/************************************************
函数名称 ： W25Q_WriteNByte
功    能 ： 从writeaddr地址开始写，连续写入Nbytes数据到Flash中
参    数 ： pbuffer：要写入数据区首地址
            writeAddr：要写入flash地址
            Nbyte：要写入的字节数（最大65535B，64k）
返 回 值 ： 
说    明 ： 无
*************************************************/
void W25Q_WriteNByte(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    static uint8_t SectorBuf[512];
    uint32_t SecPos;
    uint16_t SecOff;
    uint16_t SecRemain;
    uint16_t i;

    SecPos = WriteAddr / 512;
    SecOff = WriteAddr % 512;
    SecRemain = 512 - SecOff;

    if(nByte <= SecRemain)
        SecRemain = nByte;

    while(1)
    {

        W25Q_ReadNByte(SectorBuf, SecPos * 512, 512);
        for(i = 0; i < SecRemain; i++)
        {
            if(SectorBuf[SecOff + i] != 0xFF)
                break;
        }
        if(i < SecRemain)
        {
            W25Q_EraseSector(SecPos);
            for(i = 0; i < SecRemain; i++)
            {
                SectorBuf[SecOff + i] = pBuffer[i];
            }
            W25Q_WriteNoCheck(SectorBuf, SecPos * 512, 512);
        }
        else
            W25Q_WriteNoCheck(pBuffer, WriteAddr, SecRemain);

        if(nByte == SecRemain)
        {
            W25Q_WriteDisable();
            break;
        }
        else
        {
            SecPos++;
            SecOff = 0;
            pBuffer += SecRemain;
            WriteAddr += SecRemain;
            nByte -= SecRemain;
            if(nByte > 512)
                SecRemain = 512;
            else
                SecRemain = nByte;
        }
    }
}

/************************************************
函数名称 ： W25Q_WaitForNoBusy
功    能 ： 等待不忙
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_WaitForNoBusy(void)
{
    while((W25Q_ReadSR() & 0x01) == 0x01);
}

/************************************************
函数名称 ： W25Q_EraseBlock
功    能 ： 块擦除
           擦除块需要一定时间
   
参    数 ： BlockAddr：块地址0--63
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_EraseBlock(uint32_t BlockAddr)
{
    BlockAddr *= 65536;
    W25Q_WriteEnable();
    W25Q_WaitForNoBusy();
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_ERASE_BLOCK);
    W25Q_WriteByte((uint8_t)((BlockAddr) >> 16));
    W25Q_WriteByte((uint8_t)((BlockAddr) >> 8));
    W25Q_WriteByte((uint8_t)BlockAddr);
    W25Q_CS_DISABLE;

    W25Q_WaitForNoBusy();
}

/************************************************
函数名称 ： W25Q_EraseSector
功    能 ： 扇区擦除  
参    数 ： SectorAddr：扇区地址：0--1023
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_EraseSector(uint32_t SectorAddr)
{
    SectorAddr *= 4096;
    W25Q_WriteEnable();
    W25Q_WaitForNoBusy();
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_ERASE_SECTOR);
    
    
    W25Q_WriteByte((uint8_t)((SectorAddr) >> 16));
    W25Q_WriteByte((uint8_t)((SectorAddr) >> 8));
    W25Q_WriteByte((uint8_t)SectorAddr);
    
    W25Q_WriteByte((SectorAddr& 0xFF0000)>> 16);
    W25Q_WriteByte((SectorAddr& 0xFF00)>> 8);
    W25Q_WriteByte( SectorAddr & 0xFF);

    W25Q_CS_DISABLE;

    W25Q_WaitForNoBusy();
}

/************************************************
函数名称 ： W25Q_EraseChip
功    能 ： 整片擦除 
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_EraseChip(void)
{
    W25Q_WriteEnable();
    W25Q_WaitForNoBusy();
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_ERASE_CHIP);
    W25Q_CS_DISABLE;

    W25Q_WaitForNoBusy();
}

/************************************************
函数名称 ： W25Q_PowerDown
功    能 ： 进入掉电模式
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_PowerDown(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_POWER_DOWN);
    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_PowerDown
功    能 ： 退出掉电模式
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void W25Q_WAKEUP(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_RELEASE_POWER_DOWN);
    W25Q_CS_DISABLE;
}

/************************************************
函数名称 ： W25Q_ReadID
功    能 ： 读取芯片ID
参    数 ： 无
返 回 值 ： ID：16位ID号
说    明 ： 无
*************************************************/
uint16_t W25Q_ReadID(void)
{
    uint16_t ID = 0;
    W25Q_CS_ENABLE;

    W25Q_WriteByte(SFLASH_DEVICE_ID);
    W25Q_WriteByte(0x00);
    W25Q_WriteByte(0x00);
    W25Q_WriteByte(0x00);

    ID |= W25Q_ReadByte() << 8;
    ID |= W25Q_ReadByte();
    W25Q_CS_DISABLE;
    return ID;
}

/************************************************
函数名称 ： W25Q_ReadJEDEC_ID
功    能 ： 读取芯片JEDEC__ID
参    数 ： 无
返 回 值 ： ID：24位ID号
说    明 ： 无
*************************************************/
uint32_t W25Q_ReadJEDEC_ID(void)
{
    uint32_t ID = 0;
    W25Q_CS_ENABLE;

    W25Q_WriteByte(SFLASH_JEDEC_ID);

    ID |= W25Q_ReadByte() << 16;
    ID |= W25Q_ReadByte() << 8;
    ID |= W25Q_ReadByte();
    W25Q_CS_DISABLE;
    return ID;
}

/************************************************
函数名称 ： W25Q_Write_Rect_Fill
功    能 ： 在W25q中添加指定位置指定数据
参    数 ： 
返 回 值 ： 
说    明 ： 
*************************************************/
void W25Q_Write_Rect_Fill(uint8_t pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
    W25Q_WriteEnable();

    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_WRITE_PAGE);

    W25Q_WriteByte((WriteAddr& 0xFF0000)>> 16);
    W25Q_WriteByte((WriteAddr& 0xFF00)>> 8);
    W25Q_WriteByte( WriteAddr & 0xFF);

    while (nByte--)
    {
        W25Q_WriteByte(pBuffer);
    }

    W25Q_CS_DISABLE;
    W25Q_WaitForNoBusy();
}
