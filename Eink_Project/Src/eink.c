#include "Resource.h"

extern UART_HandleTypeDef huart2;

const unsigned char lut_vcom0[] =
{
    0x00	, 0x17	, 0x00	, 0x00	, 0x00	, 0x02,
    0x00	, 0x17	, 0x17	, 0x00	, 0x00	, 0x02,
    0x00	, 0x0A	, 0x01	, 0x00	, 0x00	, 0x01,
    0x00	, 0x0E	, 0x0E	, 0x00	, 0x00	, 0x02,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,

};
const unsigned char lut_ww[] =
{
    0x40	, 0x17	, 0x00	, 0x00	, 0x00	, 0x02,
    0x90	, 0x17	, 0x17	, 0x00	, 0x00	, 0x02,
    0x40	, 0x0A	, 0x01	, 0x00	, 0x00	, 0x01,
    0xA0	, 0x0E	, 0x0E	, 0x00	, 0x00	, 0x02,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,

};
const unsigned char lut_bw[] =
{
    0x40	, 0x17	, 0x00	, 0x00	, 0x00	, 0x02,
    0x90	, 0x17	, 0x17	, 0x00	, 0x00	, 0x02,
    0x40	, 0x0A	, 0x01	, 0x00	, 0x00	, 0x01,
    0xA0	, 0x0E	, 0x0E	, 0x00	, 0x00	, 0x02,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,

};

const unsigned char lut_bb[] =
{
    0x80	, 0x17	, 0x00	, 0x00	, 0x00	, 0x02,
    0x90	, 0x17	, 0x17	, 0x00	, 0x00	, 0x02,
    0x80	, 0x0A	, 0x01	, 0x00	, 0x00	, 0x01,
    0x50	, 0x0E	, 0x0E	, 0x00	, 0x00	, 0x02,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,

};

const unsigned char lut_wb[] =
{
    0x80	, 0x17	, 0x00	, 0x00	, 0x00	, 0x02,
    0x90	, 0x17	, 0x17	, 0x00	, 0x00	, 0x02,
    0x80	, 0x0A	, 0x01	, 0x00	, 0x00	, 0x01,
    0x50	, 0x0E	, 0x0E	, 0x00	, 0x00	, 0x02,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,
    0x00	, 0x00	, 0x00	, 0x00	, 0x00	, 0x00,

};


const unsigned char gImage_Test[74] =
{
    0X00, 0X01, 0X1E, 0X00, 0X11,
    0X00, 0X7F, 0XC0, 0X1F, 0XE0,
    0XFF, 0XF0, 0X7F, 0XF0, 0XFF,
    0XF0, 0XFF, 0XF8, 0XFF, 0XF8,
    0XFF, 0XF8, 0XC3, 0XF8, 0XFC,
    0XF8, 0X01, 0XF8, 0XFC, 0XF8,
    0X07, 0XF0, 0XFF, 0XF0, 0X0F,
    0XF0, 0X7F, 0XE0, 0X3F, 0XE0,
    0X3F, 0XF8, 0X7F, 0X80, 0XFF,
    0XFC, 0XFF, 0X01, 0XFC, 0XFC,
    0XFC, 0X01, 0XF8, 0XFC, 0XF8,
    0X01, 0XF8, 0XFC, 0XFF, 0XF9,
    0XFD, 0XFC, 0XFF, 0XF8, 0XFF,
    0XFC, 0XFF, 0XF8, 0X7F, 0XF8,
    0XFF, 0XF8, 0X1F, 0XE0,
}; //5*17


/************************************************
函数名称 ： EinkGPIO_Init
功    能 ： Eink模拟SPI的GPIO初始化
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void EinkGPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(EINK_RES_Port, EINK_RES_Pin | EINK_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EINK_CLK_Port, EINK_DC_Pin | EINK_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EINK_CS_Port, EINK_CS_Pin|EINK_SDI_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : PC14 PC15 */
    GPIO_InitStruct.Pin = EINK_RES_Pin | EINK_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(EINK_RES_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PA1 PA2 PA3 */
    GPIO_InitStruct.Pin = EINK_DC_Pin | EINK_CS_Pin | EINK_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(EINK_CS_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =  EINK_SDI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(EINK_SDI_Port, &GPIO_InitStruct);

    /*Configure GPIO pins :PA10 */
    HAL_UART_MspDeInit(&huart2);//BUSY与uart复用，因此需要Deinit
    
    GPIO_InitStruct.Pin = EINK_BUSY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(EINK_BUSY_Port, &GPIO_InitStruct);
    
}

void EinkPower_Status(EINK_State EinkState)
{
  if (EinkState != EINK_ON)
  { 
    EINK_EN_Port->BRR  = EINK_EN_Pin;
  }
  else
  {
    EINK_EN_Port->BSRR  = EINK_EN_Pin;
  }
}


/************************************************
函数名称 ： EPD_W21_RESET
功    能 ： Eink RESET
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
*************************************************/
void EPD_W21_RESET(void)
{
    nRST_L;
    Ms_Delay(200);
    nRST_H;
    Ms_Delay(200);
}



void SPI4W_WRITECOM(unsigned char INIT_COM)
{
    unsigned char TEMPCOM;
    unsigned char scnt;
    TEMPCOM = INIT_COM;
    nCS_H;
    nCS_L;
    SCLK_L;
    nDC_L;
    for(scnt = 0; scnt < 8; scnt++)
    {
        if(TEMPCOM & 0x80)
            SDA_H;
        else
            SDA_L;
        Us_Delay(1);
        SCLK_H;
        Us_Delay(1);
        SCLK_L;
        TEMPCOM = TEMPCOM << 1;
        Us_Delay(1);
    }
    nCS_H;
}

void SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
    unsigned char TEMPCOM;
    unsigned char scnt;
    TEMPCOM = INIT_DATA;
    nCS_H;
    nCS_L;
    SCLK_L;
    nDC_H;
    for(scnt = 0; scnt < 8; scnt++)
    {
        if(TEMPCOM & 0x80)
            SDA_H;
        else
            SDA_L;
        Us_Delay(1);
        SCLK_H;
        Us_Delay(1);
        SCLK_L;
        TEMPCOM = TEMPCOM << 1;
        Us_Delay(1);
    }
    nCS_H;
}
void lcd_chkstatus(void)
{
    unsigned char busy;
    do
    {
        SPI4W_WRITECOM(0x71);
        Ms_Delay(200);
        busy = nBUSY;
        busy = !(busy & 0x01);
    }
    while(busy);
}

void pic_display_42(void)
{
    unsigned int i;
    uint8_t rxd_temp;
    SPI4W_WRITECOM(0x10);   // modify 2/9 Data Start
    for(i = 0; i < 15000; i++)
    {
        SPI4W_WRITEDATA(0x00);
    }
    Ms_Delay(1);
    SPI4W_WRITECOM(0x13);  //modify 2/9 Image Process

    for(i = 0; i < 15000; i++)
    {
        SPI4W_WRITEDATA(0x55);
    }
    Ms_Delay(1);
    SPI4W_WRITECOM(0x13);


    for(i = 0; i < 15000; i++)
    {
        W25Q_ReadNByte(&rxd_temp, CmdFlash_st.Flash_BassAddr ++, 1);
        SPI4W_WRITEDATA(rxd_temp); // 输入实际的图像
    }

    Ms_Delay(1);
}

void lut(void)
{
    unsigned int count;
    SPI4W_WRITECOM(0x20);
    for(count = 0; count < 44; count++)
    {
        SPI4W_WRITEDATA(lut_vcom0[count]);  //VCOM  LUT(LUTC)
    }

    SPI4W_WRITECOM(0x21);
    for(count = 0; count < 42; count++)
    {
        SPI4W_WRITEDATA(lut_ww[count]);
    }

    SPI4W_WRITECOM(0x22);
    for(count = 0; count < 42; count++)
    {
        SPI4W_WRITEDATA(lut_bw[count]);
    }

    SPI4W_WRITECOM(0x23);
    for(count = 0; count < 42; count++)
    {
        SPI4W_WRITEDATA(lut_wb[count]);
    }

    SPI4W_WRITECOM(0x24);
    for(count = 0; count < 42; count++)
    {
        SPI4W_WRITEDATA(lut_bb[count]);
    }
}

void Display(void)
{
    EPD_W21_RESET();
    
    SPI4W_WRITECOM(0x01);
    SPI4W_WRITEDATA (0x03);	     //???????
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0x2b);
    SPI4W_WRITEDATA (0x2b);
    SPI4W_WRITEDATA (0xff);

    SPI4W_WRITECOM(0x06);         //boost??
    SPI4W_WRITEDATA (0x17);
    SPI4W_WRITEDATA (0x17);
    SPI4W_WRITEDATA (0x17);       //07 0f 17 1f 27 2F 37 2f

    SPI4W_WRITECOM(0x04);
    lcd_chkstatus();

    SPI4W_WRITECOM(0x00);
    SPI4W_WRITEDATA(0xbf);		// KW-BF   KWR-AF	BWROTP 0f
    SPI4W_WRITEDATA(0x0b);

    SPI4W_WRITECOM(0x30);			//PLL??
    SPI4W_WRITEDATA (0x3c);      	// 3A 100HZ   29 150Hz 39 200HZ	31 171HZ

    SPI4W_WRITECOM(0x61);			//??????
    SPI4W_WRITEDATA (0x01);
    SPI4W_WRITEDATA (0x90);       //128
    SPI4W_WRITEDATA (0x01);		//
    SPI4W_WRITEDATA (0x2c);

    SPI4W_WRITECOM(0x82);			//vcom??
    SPI4W_WRITEDATA (0x12);

    SPI4W_WRITECOM(0X50);
    SPI4W_WRITEDATA(0x97);		//VBDF 17|D7 VBDW 97 VBDB 57		VBDF F7 VBDW 77 VBDB 37  VBDR B7
    pic_display_42();
    lut();
    SPI4W_WRITECOM(0x12);
    Ms_Delay(50);

    lcd_chkstatus();

    /************************power off Sequence************************/
    SPI4W_WRITECOM(0X50);
    SPI4W_WRITEDATA(0x17);		//border floating

    SPI4W_WRITECOM(0X82);  		//VCOM to 0V
    SPI4W_WRITECOM(0X00);  		//

    Ms_Delay(50);

    SPI4W_WRITECOM(0x01);   		//VG&VS to 0V fast
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0x00);
    SPI4W_WRITEDATA (0x00);

    Ms_Delay(50);

    SPI4W_WRITECOM(0X02);  		//power off
    lcd_chkstatus();
    SPI4W_WRITECOM(0X07);		  	//deep sleep
    SPI4W_WRITEDATA(0xA5);
    /************************power off Sequence************************/

}