#ifndef __Display_H
#define __Display_H
#include "stm32l0xx_hal.h"
 
void mcuflash2ram(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned int addr_p);
void dis_ascii(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned int addr_p );
void dis_Hz(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned char *HZ );
void Title_Rect_Fill(void);
void Pic_Fill(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh,  unsigned int addr_p);
void EINK_Putc(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned int addr_p );
void dis_Hzstring(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned char *HZ);
#endif