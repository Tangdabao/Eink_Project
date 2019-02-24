#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "stm32l0xx_hal.h"

#include "usart.h"
#include "led.h"
#include "timer.h"
#include "w25q32.h"
#include "Eink.h"
#include "cmd.h"
#include "osal.h"
#include "Display.h"
#include "string.h"

#define BAUD_RATE  115200
#ifndef DEBUG
#define DEBUG 0
#endif

/* Private macros ------------------------------------------------------------*/
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif



/*************************************************/
#define DATA_BUF_SIZE  128

#define	FRAME_TIMEOUT_100MS        100

#define Preamble_Data_Msb   0x88
#define Preamble_Data_Lsb   0x01

#define   ACK                0x79               //y
#define   NACK               0x6e               //n 

#define   TEST_CMD           0x01
#define   WRITE_PARA         0x02
#define   REFRESH_DIS        0x03
#define   READ_FLASH         0x04
#define   ERASER_FLASH       0x05
#define   WRITE_FLASH        0x06

/*************************************************/
#define W25Q32_ADDR_BASS	0x000000
#define BLOCK0_BASS             W25Q32_ADDR_BASS        
#define BLOCK1_BASS             BLOCK0_BASS + 0x010000    
#define BLOCK2_BASS             BLOCK1_BASS + 0x010000   
#define BLOCK3_BASS             BLOCK2_BASS + 0x010000   
#define BLOCK4_BASS             BLOCK3_BASS + 0x010000   
#define BLOCK5_BASS             BLOCK4_BASS + 0x010000   
#define BLOCK6_BASS             BLOCK5_BASS + 0x010000   
#define BLOCK7_BASS             BLOCK6_BASS + 0x010000   
#define BLOCK8_BASS             BLOCK7_BASS + 0x010000   
#define BLOCK9_BASS              BLOCK8_BASS + 0x010000   
#define BLOCK10_BASS             BLOCK9_BASS + 0x010000   
#define BLOCK11_BASS             BLOCK10_BASS + 0x010000   
#define BLOCK12_BASS             BLOCK11_BASS + 0x010000   
#define BLOCK13_BASS             BLOCK12_BASS + 0x010000   
#define BLOCK14_BASS             BLOCK13_BASS + 0x010000   
#define BLOCK15_BASS             BLOCK14_BASS + 0x010000   
#define BLOCK16_BASS             BLOCK15_BASS + 0x010000   
#define BLOCK17_BASS             BLOCK16_BASS + 0x010000   
#define BLOCK18_BASS             BLOCK17_BASS + 0x010000   
#define BLOCK19_BASS             BLOCK18_BASS + 0x010000   
#define BLOCK20_BASS             BLOCK19_BASS + 0x010000   
#define BLOCK21_BASS             BLOCK20_BASS + 0x010000   
#define BLOCK22_BASS             BLOCK21_BASS + 0x010000   
#define BLOCK23_BASS             BLOCK22_BASS + 0x010000   
#define BLOCK24_BASS             BLOCK23_BASS + 0x010000   
#define BLOCK25_BASS             BLOCK24_BASS + 0x010000   
#define BLOCK26_BASS             BLOCK25_BASS + 0x010000   
#define BLOCK27_BASS             BLOCK26_BASS + 0x010000   
#define BLOCK28_BASS             BLOCK27_BASS + 0x010000   
#define BLOCK29_BASS             BLOCK28_BASS + 0x010000   
#define BLOCK30_BASS             BLOCK29_BASS + 0x010000   
#define BLOCK31_BASS             BLOCK30_BASS + 0x010000   
#define BLOCK32_BASS             BLOCK31_BASS + 0x010000   
#define BLOCK33_BASS             BLOCK32_BASS + 0x010000   
#define BLOCK34_BASS             BLOCK33_BASS + 0x010000   
#define BLOCK35_BASS             BLOCK34_BASS + 0x010000   
#define BLOCK36_BASS             BLOCK35_BASS + 0x010000   
#define BLOCK37_BASS             BLOCK36_BASS + 0x010000   
#define BLOCK38_BASS             BLOCK37_BASS + 0x010000   
#define BLOCK39_BASS             BLOCK38_BASS + 0x010000   
#define BLOCK40_BASS             BLOCK39_BASS + 0x010000   
#define BLOCK41_BASS             BLOCK40_BASS + 0x010000   
#define BLOCK42_BASS             BLOCK41_BASS + 0x010000   
#define BLOCK43_BASS             BLOCK42_BASS + 0x010000   
#define BLOCK44_BASS             BLOCK43_BASS + 0x010000   
#define BLOCK45_BASS             BLOCK44_BASS + 0x010000   
#define BLOCK46_BASS             BLOCK45_BASS + 0x010000   
#define BLOCK47_BASS             BLOCK46_BASS + 0x010000   
#define BLOCK48_BASS             BLOCK47_BASS + 0x010000   
#define BLOCK49_BASS             BLOCK48_BASS + 0x010000   
#define BLOCK50_BASS             BLOCK49_BASS + 0x010000   
#define BLOCK51_BASS             BLOCK50_BASS + 0x010000   
#define BLOCK52_BASS             BLOCK51_BASS + 0x010000   
#define BLOCK53_BASS             BLOCK52_BASS + 0x010000   
#define BLOCK54_BASS             BLOCK53_BASS + 0x010000   
#define BLOCK55_BASS             BLOCK54_BASS + 0x010000   
#define BLOCK56_BASS             BLOCK55_BASS + 0x010000   
#define BLOCK57_BASS             BLOCK56_BASS + 0x010000   
#define BLOCK58_BASS             BLOCK57_BASS + 0x010000   
#define BLOCK59_BASS             BLOCK58_BASS + 0x010000   
#define BLOCK60_BASS             BLOCK59_BASS + 0x010000   
#define BLOCK61_BASS             BLOCK60_BASS + 0x010000   
#define BLOCK62_BASS             BLOCK61_BASS + 0x010000   
#define BLOCK63_BASS             BLOCK62_BASS + 0x010000   


/*************************************************/
#define DIS_RAM_BASS            BLOCK0_BASS
#define DIS_RAM_BYTES           15000

#define DIS_TEST_PAGE1          DIS_RAM_BASS + 0x004000
#define DIS_TEST_PAGE2          DIS_TEST_PAGE1 + 0x004000
#define DIS_TEST_PAGE3          DIS_TEST_PAGE2 + 0x004000
#define ASCII_BASS                BLOCK1_BASS
#define HZK8_16_SONG_BASS         BLOCK6_BASS
#define HZK16_16_SONG_BASS        BLOCK2_BASS

/*************************************************/
#define EINK_WIDTH      400
#define EINK_HEIGH	300
#define EINK_DOT        EINK_WIDE * EINK_HEIGH


typedef struct
{
  uint8_t CmdBuff[DATA_BUF_SIZE];//uart 数据buff
  uint8_t CmdLenth;   //uart数据长度
  uint8_t acmdStart;  //开始接收Cmd 标志
  uint8_t acmdEnd;    //uart数据接收完毕标志
  uint8_t acmdMsCount; //用于Uart的Ms定时器
  uint8_t acmdDisplay_Start;//uart命令，开始刷新标志
}CmdFromUart;

typedef struct
{
  uint8_t WriteFlash_Flag;//开始写Flash标志,当需要写W25q32的时候有效
  uint8_t WriteFlashBuff;//写Flash内容Buff
  uint32_t WriteAddrFlash;//写Flash地址
  uint32_t WriteFlash_Len;//写Flash内容长度
  uint32_t Flash_BassAddr;//刷新基地址
}CmdFlash;  
  
extern CmdFlash   CmdFlash_st;
extern CmdFromUart  CmdFromUart_st;


void InitAll(void);
#endif

