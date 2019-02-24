#include "Resource.h"

void mcuflash2ram(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned int addr_p)
{
  unsigned int i, j, l;
  unsigned int byte_pic_width_count;
  
  unsigned char buf[25];
  
  
  byte_pic_width_count = pic_width / 8;
  if(pic_width % 8)
    byte_pic_width_count++;
  
  for(i = 0; i < pic_heigh; i++)
  {
    W25Q_ReadNByte(buf, addr_p, byte_pic_width_count);

    l = (x + i) * EINK_WIDTH / 8 + y / 8;
    Ms_Delay(20);
    
    for(j = 0; j < byte_pic_width_count; j++)
      buf[j] = ~buf[j];
    
    W25Q_WritePage(buf, l, byte_pic_width_count);    
    addr_p += byte_pic_width_count;    
  }
  
}
void dis_ascii(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned int addr_p )//1578368
{
  
  mcuflash2ram(x, y, pic_width, pic_heigh, addr_p);
  
}

void dis_Hz(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned char *HZ )
{

  uint32_t i,j;
  uint32_t pos;
  //取得字在字库里面的索引地址
  i = *HZ++ - 0xa0;
  j = *HZ++ - 0xa0;   
  
  // -6 操作是因为本终端对字库做了处理，汉字是由16区开始，而不是从11区开始
  pos = HZK8_16_SONG_BASS+((i-1)*94+(j-1))*32; 
  //str = (unsigned char *)480;
  //在相应的地方写上汉字，一个汉字地址占16像素，宽度字节数*8    
  mcuflash2ram(x, y, pic_width, pic_heigh, pos);

}

void dis_Hzstring(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned char *HZ)
{ 
  char i,j;
  
  i = strlen(HZ);
  
  for(j=0; j < i; j++)
  {
    dis_Hz(x,y + j * pic_width/2,pic_width,pic_heigh,HZ + j );
    j++;
  }
}



void Pic_Fill(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh,  unsigned int addr_p)
{
  unsigned int i,pos;
  unsigned int byte_pic_width_count;
  
  unsigned char buf[50];
  
  
  byte_pic_width_count = pic_width / 8;
  if(pic_width % 8)
    byte_pic_width_count++;
  
  for(i = 0; i < pic_heigh; i++)
  {
    W25Q_ReadNByte(buf, addr_p, byte_pic_width_count);

    pos = (x + i) * EINK_WIDTH / 8 + y / 8;

    
    W25Q_WritePage(buf, pos, byte_pic_width_count);    
    addr_p += byte_pic_width_count;    
  }
}


void Title_Rect_Fill(void)
{
 
  //SFLASH_Write_Rect_Fill(0x00, 200, 200);
  //SFLASH_Write_Rect_Fill(0x00, 400, 200);
  //SFLASH_Write_Rect_Fill(0x00, 600, 200);
  
  /*
  SFLASH_Write_Rect_Fill(0x00, 1600, 200);
  SFLASH_Write_Rect_Fill(0x00, 1800, 200);
  SFLASH_Write_Rect_Fill(0x00, 2000, 200);
  SFLASH_Write_Rect_Fill(0x00, 2200, 200);
  */
  //SFLASH_Write_Rect_Fill(0x00, 1600, 800);
  for(uint8_t i=0;i<4;i++)
  {
    //Delay_ms(20);
    W25Q_Write_Rect_Fill(0x00, i*200, 200);
  }
  
 // SFLASH_WritePage(buf_dis_ram, 200, 200); 
 //SFLASH_WritePage(buf_dis_ram, 400, 200); 
 // SFLASH_WritePage(buf_dis_ram, 600, 200); 

  //SFLASH_WritePage(buf_dis_ram, 1600, 200);
 //SFLASH_WritePage(buf_dis_ram, 1800, 200);
  //SFLASH_WritePage(buf_dis_ram, 2000, 200);
 // SFLASH_WritePage(buf_dis_ram, 2200, 200);
}

void EINK_Putc(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, unsigned int addr_p )
{
    mcuflash2ram(x, y, pic_width, pic_heigh, addr_p);
}

void EINK_PutString_ascii(unsigned int x, unsigned int y, unsigned int pic_width, unsigned int pic_heigh, uint8_t *str )
{
    int32_t i;

    for ( i = 0; i < strlen((const char *)&str[0]); i++ )
    {
        EINK_Putc(x, y, pic_width, pic_heigh, ASCII_BASS + str[i]*16);
        y += 8;
    }
}
