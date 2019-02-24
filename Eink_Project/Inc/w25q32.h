#ifndef __W25q32_H
#define __W25q32_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "main.h"


/* 指令表 */
#define SFLASH_WRITE_ENABLE       0x06                     //写使能
#define SFLASH_WRITE_DISABLE      0x04                     //写失能
#define SFLASH_READ_STATUS_REG    0x05                     //读状态寄存器
#define SFLASH_WRITE_STATUS_REG   0x01                     //写状态寄存器

#define SFLASH_READ_DATA          0x03                     //读数据
#define SFLASH_FAST_READ          0x0B                     //快读数据
#define SFLASH_FAST_READ_DUAL     0x3B                     //快读数据(双数据线输出)
#define SFLASH_WRITE_PAGE         0x02                     //页编程
#define SFLASH_ERASE_BLOCK        0xD8                     //擦除块
#define SFLASH_ERASE_SECTOR       0x20                     //擦除扇区
#define SFLASH_ERASE_CHIP         0xC7                     //擦除芯片
#define SFLASH_POWER_DOWN         0xB9                     //掉电
#define SFLASH_RELEASE_POWER_DOWN 0xAB                     //释放掉电
#define SFLASH_DEVICE_ID          0x90                     //设备ID
#define SFLASH_JEDEC_ID           0x9F                     //Jedec ID

#define W25Q32_ID                0XEF15

void W25Q_GPIO_Init(void);

void W25Q_WriteByte(uint8_t TxData);
uint8_t W25Q_ReadByte(void);
uint8_t W25Q_WriteReadByte(uint8_t TxData);


static void W25Q_WriteDisable(void);                     //写失能
static void W25Q_WriteEnable(void);                      //写使能

uint8_t W25Q_ReadSR(void);                               //读状态寄存器
void W25Q_WriteSR(uint8_t SR);                           //写状态寄存器

void W25Q_ReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte);         //读取n字节数据
void W25Q_FastReadNByte(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t nByte);     //快读n字节数据

void W25Q_WritePage(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);        //写入n(<256字节)数据
void W25Q_WriteNoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);     //写入n字节数据(无校验)
void W25Q_WriteNByte(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t nByte);       //写入n字节数据
void W25Q_WaitForNoBusy(void);                           //等待不忙

void W25Q_EraseBlock(uint32_t BlockAddr);                //擦除块
void W25Q_EraseSector(uint32_t SectorAddr);              //擦除扇区
void W25Q_EraseChip(void);                               //擦除整个芯片
void W25Q_PowerDown(void);                               //掉电模式
void W25Q_WAKEUP(void);                                  //唤醒器件
uint16_t W25Q_ReadID(void);                              //读取SFLASH ID
uint32_t W25Q_ReadJEDEC_ID(void);                        //读取JEDEC_ID
void W25Q_Write_Rect_Fill(uint8_t pBuffer, uint32_t WriteAddr, uint16_t nByte);
#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */