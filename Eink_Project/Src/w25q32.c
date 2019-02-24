#include "W25q32.h"


/*****************************spi FLASH��Ϣ**********************************

W25Q32��
1.������32M Bit/4Mbyte��4194304��
2.�洢�ṹ��

ҳ��page��   :256Bytes
����(Sector) :4Kbytes
�飨Block��  ��64Kbytes


ҳ��16384
������1024
�飺64

3.�ٶȣ�
80Mhz

4.״̬���ƼĴ������Ĭ��0X00��

Bit 7     6      5        4       3       2       1       0
   SPR    RV    TB      BP2     BP1     BP0     WEL     BUSY

SPR��Ĭ��Ϊ0��״̬�Ĵ�������λ
TB��BP2--0,��Flash����д��������
WEL;дʹ������
BUSY����æ��־��1��æ��0����æ��

ע�⣺
1��ҳΪ��̵�λ������һ���Ա��1����256���ֽڣ�����256���ֽڿ϶�Ҫ�ֶ��д��

2���ڱ��֮ǰ������Զ�Ӧ��������в��������������п���д�����

3����������С��λ�ǡ���������Ҳ�����ԡ��顱Ϊ��λ���в�������ʱ�����Ϊ32K-bytes ���� 64K-bytes�������������������

4���������ȱ�̲������׵Ķ࣬û�����ϵ�ϸ�ڿ��ǡ�����һ���Զ�һ���ֽڣ�Ҳ���Զ���ֽڣ�������ͷ����β��
***************************************************************************/

/************************************************
�������� �� W25_GPIO_Init
��    �� �� W25Q32 ģ��SPI��GPIO��ʼ��
��    �� �� ��
�� �� ֵ �� ��
˵    �� �� ��
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
�������� �� W25Q_WriteByte
��    �� �� W25Q32 SPIдһ���ֽ�����
��    �� �� TxData  �����͵��ֽ�����
�� �� ֵ �� ��
˵    �� �� ��
*************************************************/
void W25Q_WriteByte(uint8_t TxData)
{
    uint8_t cnt;

    for(cnt = 0; cnt < 8; cnt++)
    {
        W25Q_CLK_LOW;                                 //ʱ�� - ��
        DELAY_SPI_CLK;

        if(TxData & 0x80)                            //��������
            W25Q_MOSI_HIGH;
        else
            W25Q_MOSI_LOW;
        TxData <<= 1;

        DELAY_SPI_CLK;
        W25Q_CLK_HIGH;                                //ʱ�� - ��
        DELAY_SPI_CLK;
    }
}

/************************************************
�������� �� W25Q_ReadByte
��    �� �� W25Q32 SPI��һ���ֽ�����
��    �� �� ��
�� �� ֵ �� ������������
˵    �� �� ��
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
�������� �� W25Q_WriteEnable
��    �� �� W25Q32 дʹ�ܣ���λWEL
��    �� �� ��
�� �� ֵ ����
˵    �� �� ��
*************************************************/
static void W25Q_WriteEnable(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_WRITE_ENABLE);//дʹ��
    W25Q_CS_DISABLE;
}

/************************************************
�������� �� W25Q_WriteDisable
��    �� �� W25Q32 д��ֹ��WEL����
��    �� �� ��
�� �� ֵ ����
˵    �� �� ��
*************************************************/
static void W25Q_WriteDisable(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_WRITE_DISABLE);
    W25Q_CS_DISABLE;
}

/************************************************
�������� �� W25Q_ReadSR
��    �� �� ��ȡ״̬�Ĵ���
��    �� �� ��
�� �� ֵ ��byte ����ȡ���ֽ�
˵    �� �� ��
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
�������� �� W25Q_WriteSR
��    �� �� дȡ״̬�Ĵ���
��    �� �� SR��д״̬�Ĵ���
�� �� ֵ �� 
˵    �� �� ��
*************************************************/
void W25Q_WriteSR(uint8_t SR)
{
    W25Q_WriteEnable();
    W25Q_WriteByte(SFLASH_WRITE_STATUS_REG);
    W25Q_WriteByte(SR);
    W25Q_CS_DISABLE;
}

/************************************************
�������� �� W25Q_ReadNByte
��    �� �� ��Readaddr��ַ��ʼ������ȡNbyte
��    �� �� pbuffer�����ݴ洢���׵�ַ
            ReadAddr��Ҫ��ȡ��W25Q���׵�ַ
            Nbyte��Ҫ��ȡ���ֽ��ֽ��������65535��
�� �� ֵ �� 
˵    �� �� ��
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
�������� �� W25Q_FastReadNByte
��    �� �� ��Readaddr��ַ��ʼ������ȡNbyte
��    �� �� pbuffer�����ݴ洢���׵�ַ
            ReadAddr��Ҫ��ȡ��W25Q���׵�ַ
            Nbyte��Ҫ��ȡ���ֽ��ֽ��������65535��
�� �� ֵ �� 
˵    �� �� ��
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
�������� �� W25Q_WritePage
��    �� �� ��W25Q��д������1pages��256�ֽڣ�������
��    �� �� pbuffer�����ݴ洢���׵�ַ
            ReadAddr��Ҫ��ȡ��W25Q���׵�ַ
            Nbyte��Ҫ��ȡ���ֽ��ֽ��������1Pages��
�� �� ֵ �� 
˵    �� �� ��
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
�������� �� W25Q_WriteNoCheck
��    �� �� �޼���дFlash
          ����ȷ����д��ַ��ΪFFF�������FF������ִ���
          �����Զ���ҳ����
          ���ƶ���ַд���ƶ��������ݣ�ȷ����Խ��

��    �� �� pbuffer��Ҫд���������׵�ַ
            writeAddr��Ҫд��flash��ַ
            Nbyte��Ҫд����ֽ���
�� �� ֵ �� 
˵    �� �� ��
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
�������� �� W25Q_WriteNByte
��    �� �� ��writeaddr��ַ��ʼд������д��Nbytes���ݵ�Flash��
��    �� �� pbuffer��Ҫд���������׵�ַ
            writeAddr��Ҫд��flash��ַ
            Nbyte��Ҫд����ֽ��������65535B��64k��
�� �� ֵ �� 
˵    �� �� ��
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
�������� �� W25Q_WaitForNoBusy
��    �� �� �ȴ���æ
��    �� �� ��
�� �� ֵ �� ��
˵    �� �� ��
*************************************************/
void W25Q_WaitForNoBusy(void)
{
    while((W25Q_ReadSR() & 0x01) == 0x01);
}

/************************************************
�������� �� W25Q_EraseBlock
��    �� �� �����
           ��������Ҫһ��ʱ��
   
��    �� �� BlockAddr�����ַ0--63
�� �� ֵ �� ��
˵    �� �� ��
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
�������� �� W25Q_EraseSector
��    �� �� ��������  
��    �� �� SectorAddr��������ַ��0--1023
�� �� ֵ �� ��
˵    �� �� ��
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
�������� �� W25Q_EraseChip
��    �� �� ��Ƭ���� 
��    �� �� ��
�� �� ֵ �� ��
˵    �� �� ��
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
�������� �� W25Q_PowerDown
��    �� �� �������ģʽ
��    �� �� ��
�� �� ֵ �� ��
˵    �� �� ��
*************************************************/
void W25Q_PowerDown(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_POWER_DOWN);
    W25Q_CS_DISABLE;
}

/************************************************
�������� �� W25Q_PowerDown
��    �� �� �˳�����ģʽ
��    �� �� ��
�� �� ֵ �� ��
˵    �� �� ��
*************************************************/
void W25Q_WAKEUP(void)
{
    W25Q_CS_ENABLE;
    W25Q_WriteByte(SFLASH_RELEASE_POWER_DOWN);
    W25Q_CS_DISABLE;
}

/************************************************
�������� �� W25Q_ReadID
��    �� �� ��ȡоƬID
��    �� �� ��
�� �� ֵ �� ID��16λID��
˵    �� �� ��
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
�������� �� W25Q_ReadJEDEC_ID
��    �� �� ��ȡоƬJEDEC__ID
��    �� �� ��
�� �� ֵ �� ID��24λID��
˵    �� �� ��
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
�������� �� W25Q_Write_Rect_Fill
��    �� �� ��W25q�����ָ��λ��ָ������
��    �� �� 
�� �� ֵ �� 
˵    �� �� 
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
