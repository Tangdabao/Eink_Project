#ifndef __W25q32_H
#define __W25q32_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "main.h"


/* ָ��� */
#define SFLASH_WRITE_ENABLE       0x06                     //дʹ��
#define SFLASH_WRITE_DISABLE      0x04                     //дʧ��
#define SFLASH_READ_STATUS_REG    0x05                     //��״̬�Ĵ���
#define SFLASH_WRITE_STATUS_REG   0x01                     //д״̬�Ĵ���

#define SFLASH_READ_DATA          0x03                     //������
#define SFLASH_FAST_READ          0x0B                     //�������
#define SFLASH_FAST_READ_DUAL     0x3B                     //�������(˫���������)
#define SFLASH_WRITE_PAGE         0x02                     //ҳ���
#define SFLASH_ERASE_BLOCK        0xD8                     //������
#define SFLASH_ERASE_SECTOR       0x20                     //��������
#define SFLASH_ERASE_CHIP         0xC7                     //����оƬ
#define SFLASH_POWER_DOWN         0xB9                     //����
#define SFLASH_RELEASE_POWER_DOWN 0xAB                     //�ͷŵ���
#define SFLASH_DEVICE_ID          0x90                     //�豸ID
#define SFLASH_JEDEC_ID           0x9F                     //Jedec ID

#define W25Q32_ID                0XEF15

void W25Q_GPIO_Init(void);

void W25Q_WriteByte(uint8_t TxData);
uint8_t W25Q_ReadByte(void);
uint8_t W25Q_WriteReadByte(uint8_t TxData);


static void W25Q_WriteDisable(void);                     //дʧ��
static void W25Q_WriteEnable(void);                      //дʹ��

uint8_t W25Q_ReadSR(void);                               //��״̬�Ĵ���
void W25Q_WriteSR(uint8_t SR);                           //д״̬�Ĵ���

void W25Q_ReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte);         //��ȡn�ֽ�����
void W25Q_FastReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte);     //���n�ֽ�����

void W25Q_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);        //д��n(<256�ֽ�)����
void W25Q_WriteNoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);     //д��n�ֽ�����(��У��)
void W25Q_WriteNByte(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);       //д��n�ֽ�����
void W25Q_WaitForNoBusy(void);                           //�ȴ���æ

void W25Q_EraseBlock(uint32_t BlockAddr);                //������
void W25Q_EraseSector(uint32_t SectorAddr);              //��������
void W25Q_EraseChip(void);                               //��������оƬ
void W25Q_PowerDown(void);                               //����ģʽ
void W25Q_WAKEUP(void);                                  //��������
uint16_t W25Q_ReadID(void);                              //��ȡSFLASH ID
uint32_t W25Q_ReadJEDEC_ID(void);                        //��ȡJEDEC_ID
void W25Q_Write_Rect_Fill(uint8_t pBuffer, uint32_t WriteAddr, uint16_t nByte);
#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */