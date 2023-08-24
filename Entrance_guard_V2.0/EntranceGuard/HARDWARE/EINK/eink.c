#include "eink.h"
#include "stdlib.h"
#include "font.h"
u8 Canvas[Display_buff_len];
// 初始化GPIO
void EINK_GPIOInit(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  /**/
  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /*Configure GPIO pins : PA0 PA1 PA2 PA15 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pin Output Level */
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7);
}

// 模拟SPI时序
void EINK_WR_Bus(u8 dat)
{
  u8 i;
  EINK_CS_Clr();
  for (i = 0; i < 8; i++)
  {
    EINK_SCL_Clr();
    if (dat & 0x80)
    {
      EINK_SDA_Set();
    }
    else
    {
      EINK_SDA_Clr();
    }
    EINK_SCL_Set();
    dat <<= 1;
  }
  EINK_CS_Set();
}

// 写入一个命令
void EINK_WR_REG(u8 reg)
{
  EINK_DC_Clr();
  EINK_WR_Bus(reg);
  EINK_DC_Set();
}

// 写入一个字节
void EINK_WR_DATA8(u8 dat)
{
  EINK_WR_Bus(dat);
}

PAINT Paint;

void Epaper_READBUSY()
{
  while (1)
  {
    // if(EINK_BUSY()==0)
    // {
    msDelay(1500);
    break;
    // }
  }
}

void EPD_Update(void)
{
  EINK_WR_REG(0x22); // Display Update Control
  EINK_WR_DATA8(0xF7);
  // EINK_WR_DATA8(0xFF);
  EINK_WR_REG(0x20); // Activate Display Update Sequence
  Epaper_READBUSY();
}

// 初始化屏幕
void EINK_GUIInit(void)
{
  EINK_GPIOInit();
  EINK_RES_Clr(); // Module reset
  msDelay(20);    // At least 10ms delay
  EINK_RES_Set();
  msDelay(20); // At least 10ms delay

  Epaper_READBUSY();
  EINK_WR_REG(0x12); // SWRESET
  Epaper_READBUSY();

  EINK_WR_REG(0x01); // Driver output control
  EINK_WR_DATA8(0xC7);
  EINK_WR_DATA8(0x00);
  EINK_WR_DATA8(0x01);

  EINK_WR_REG(0x11); // data entry mode
  EINK_WR_DATA8(0x01);

  EINK_WR_REG(0x44); // set Ram-X address start/end position
  EINK_WR_DATA8(0x00);
  EINK_WR_DATA8(0x18); // 0x0F-->(15+1)*8=128

  EINK_WR_REG(0x45);   // set Ram-Y address start/end position
  EINK_WR_DATA8(0xC7); // 0xF9-->(249+1)=250
  EINK_WR_DATA8(0x00);
  EINK_WR_DATA8(0x00);
  EINK_WR_DATA8(0x00);

  EINK_WR_REG(0x3C); // BorderWavefrom
  EINK_WR_DATA8(0x05);

  //  EINK_WR_REG(0x21); //  Display update control
  //  EINK_WR_DATA8(0x00);
  //  EINK_WR_DATA8(0x80);

  EINK_WR_REG(0x18); // Read built-in temperature sensor
  EINK_WR_DATA8(0x80);

  EINK_WR_REG(0x4E); // set RAM x address count to 0;
  EINK_WR_DATA8(0x00);
  EINK_WR_REG(0x4F); // set RAM y address count to 0X199;
  EINK_WR_DATA8(0xC7);
  EINK_WR_DATA8(0x00);

  Epaper_READBUSY();
}

void Paint_NewImage(u8 *image, u16 Width, u16 Height, u16 Rotate, u16 Color)
{
  Paint.Image = 0x00;
  Paint.Image = image;

  Paint.WidthMemory = Width;
  Paint.HeightMemory = Height;
  Paint.Color = Color;
  Paint.WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
  Paint.HeightByte = Height;
  Paint.Rotate = Rotate;
  if (Rotate == ROTATE_0 || Rotate == ROTATE_180)
  {
    Paint.Width = Width;
    Paint.Height = Height;
  }
  else
  {
    Paint.Width = Height;
    Paint.Height = Width;
  }
}

void Paint_SetPixel(u16 Xpoint, u16 Ypoint, u16 Color)
{
  u16 X, Y;
  u32 Addr;
  u8 Rdata;
  switch (Paint.Rotate)
  {
  case 0:
    X = Xpoint;
    Y = Ypoint;
    break;
  case 90:
    X = Paint.WidthMemory - Ypoint - 1;
    Y = Xpoint;
    break;
  case 180:
    X = Paint.WidthMemory - Xpoint - 1;
    Y = Paint.HeightMemory - Ypoint - 1;
    break;
  case 270:
    X = Ypoint;
    Y = Paint.HeightMemory - Xpoint - 1;
    break;
  default:
    return;
  }
  Addr = X / 8 + Y * Paint.WidthByte;
  Rdata = Paint.Image[Addr];
  if (Color == BLACK)
  {
    Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8)); // 将对应数据位置0
  }
  else
    Paint.Image[Addr] = Rdata | (0x80 >> (X % 8)); // 将对应数据位置1
}

// 清屏函数
void EINK_Clear(u16 Color)
{
  u16 X, Y;
  u32 Addr;
  for (Y = 0; Y < Paint.HeightByte; Y++)
  {
    for (X = 0; X < Paint.WidthByte; X++)
    { // 8 pixel =  1 byte
      Addr = X + Y * Paint.WidthByte;
      Paint.Image[Addr] = Color;
    }
  }
}

// 画点函数
void EINK_DrawPoint(u16 Xpoint, u16 Ypoint, u16 Color)
{
  Paint_SetPixel(Xpoint - 1, Ypoint - 1, Color);
}

// 更新到显存
void EINK_Display(unsigned char *Image)
{
  unsigned int Width, Height, i, j;
  u32 k = 0;
  Width = 25;
  Height = 200;
  EINK_WR_REG(0x24);
  for (j = 0; j < Height; j++)
  {
    for (i = 0; i < Width; i++)
    {
      EINK_WR_DATA8(Image[k]);
      k++;
    }
  }
  EPD_Update();
}

// 画直线
void EINK_DrawLine(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, u16 Color)
{
  u16 Xpoint, Ypoint;
  int dx, dy;
  int XAddway, YAddway;
  int Esp;
  char Dotted_Len;
  Xpoint = Xstart;
  Ypoint = Ystart;
  dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
  dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

  XAddway = Xstart < Xend ? 1 : -1;
  YAddway = Ystart < Yend ? 1 : -1;

  Esp = dx + dy;
  Dotted_Len = 0;

  for (;;)
  {
    Dotted_Len++;
    EINK_DrawPoint(Xpoint, Ypoint, Color);
    if (2 * Esp >= dy)
    {
      if (Xpoint == Xend)
        break;
      Esp += dy;
      Xpoint += XAddway;
    }
    if (2 * Esp <= dx)
    {
      if (Ypoint == Yend)
        break;
      Esp += dx;
      Ypoint += YAddway;
    }
  }
}

// 画矩形
void EINK_DrawRectangle(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, u16 Color, u8 mode)
{
  u16 i;
  if (mode)
  {
    for (i = Ystart; i < Yend; i++)
    {
      EINK_DrawLine(Xstart, i, Xend, i, Color);
    }
  }
  else
  {
    EINK_DrawLine(Xstart, Ystart, Xend, Ystart, Color);
    EINK_DrawLine(Xstart, Ystart, Xstart, Yend, Color);
    EINK_DrawLine(Xend, Yend, Xend, Ystart, Color);
    EINK_DrawLine(Xend, Yend, Xstart, Yend, Color);
  }
}

// 画圆形
void EINK_DrawCircle(u16 X_Center, u16 Y_Center, u16 Radius, u16 Color, u8 mode)
{
  u16 Esp, sCountY;
  u16 XCurrent, YCurrent;
  XCurrent = 0;
  YCurrent = Radius;
  Esp = 3 - (Radius << 1);
  if (mode)
  {
    while (XCurrent <= YCurrent)
    { // Realistic circles
      for (sCountY = XCurrent; sCountY <= YCurrent; sCountY++)
      {
        EINK_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color); // 1
        EINK_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color); // 2
        EINK_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color); // 3
        EINK_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color); // 4
        EINK_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color); // 5
        EINK_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color); // 6
        EINK_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color); // 7
        EINK_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color);
      }
      if ((int)Esp < 0)
        Esp += 4 * XCurrent + 6;
      else
      {
        Esp += 10 + 4 * (XCurrent - YCurrent);
        YCurrent--;
      }
      XCurrent++;
    }
  }
  else
  { // Draw a hollow circle
    while (XCurrent <= YCurrent)
    {
      EINK_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color); // 1
      EINK_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color); // 2
      EINK_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color); // 3
      EINK_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color); // 4
      EINK_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color); // 5
      EINK_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color); // 6
      EINK_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color); // 7
      EINK_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color); // 0
      if ((int)Esp < 0)
        Esp += 4 * XCurrent + 6;
      else
      {
        Esp += 10 + 4 * (XCurrent - YCurrent);
        YCurrent--;
      }
      XCurrent++;
    }
  }
}

// 显示字符
void EINK_ShowChar(u16 x, u16 y, u16 chr, u16 size1, u16 color)
{
  u16 i, m, temp, size2, chr1;
  u16 x0, y0;
  x += 1, y += 1, x0 = x, y0 = y;
  if (size1 == 8)
    size2 = 6;
  else
    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); // 得到字体一个字符对应点阵集所占的字节数
  chr1 = chr - ' ';                                            // 计算偏移后的值
  for (i = 0; i < size2; i++)
  {
    		if(size1==8)
    			  {temp=asc2_0806[chr1][i];} //调用0806字体

    // if (size1 == 16)
    // {
    //   temp = asc2_1608[chr1][i];
    // } // 调用1608字体
    else
      return;
    for (m = 0; m < 8; m++)
    {
      if (temp & 0x01)
        EINK_DrawPoint(x, y, color);
      else
        EINK_DrawPoint(x, y, !color);
      temp >>= 1;
      y++;
    }
    x++;
    if ((size1 != 8) && ((x - x0) == size1 / 2))
    {
      x = x0;
      y0 = y0 + 8;
    }
    y = y0;
  }
}

// 显示字符串
// x,y:起点坐标
// size1:字体大小
//*chr:字符串起始地址
// mode:0,反色显示;1,正常显示
void EINK_ShowString(u16 x, u16 y, u8 *chr, u16 size1, u16 color)
{
  while (*chr != '\0') // 判断是不是非法字符!
  {

    EINK_ShowChar(x, y, *chr, size1, color);
    chr++;
    x += size1 / 2;
  }
}

// m^n
u32 EINK_Pow(u16 m, u16 n)
{
  u32 result = 1;
  while (n--)
  {
    result *= m;
  }
  return result;
}

// 显示数字
// x,y :起点坐标
// num :要显示的数字
// len :数字的位数
// size:字体大小
// mode:0,反色显示;1,正常显示
void EINK_ShowNum(u16 x, u16 y, u32 num, u16 len, u16 size1, u16 color)
{
  u8 t, temp, m = 0;
  if (size1 == 8)
    m = 2;
  for (t = 0; t < len; t++)
  {
    temp = (num / EINK_Pow(10, len - t - 1)) % 10;
    if (temp == 0)
    {
      EINK_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, color);
    }
    else
    {
      EINK_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, color);
    }
  }
}

// 显示图片
//  x,y:起点坐标
//  sizex：图片宽度
//  sizey:图片长度
//  BMP：图片数组
//  mode:图片显示的颜色
void EINK_ShowPicture(u16 x, u16 y, u16 sizex, u16 sizey, const u8 BMP[], u16 Color)
{
  u16 j = 0;
  u16 i, n, temp, m;
  u16 x0, y0;
  x += 1, y += 1, x0 = x, y0 = y;
  sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
  for (n = 0; n < sizey; n++)
  {
    for (i = 0; i < sizex; i++)
    {
      temp = BMP[j];
      j++;
      for (m = 0; m < 8; m++)
      {
        if (temp & 0x01)
          EINK_DrawPoint(x, y, Color);
        else
          EINK_DrawPoint(x, y, !Color);
        temp >>= 1;
        y++;
      }
      x++;
      if ((x - x0) == sizex)
      {
        x = x0;
        y0 = y0 + 8;
      }
      y = y0;
    }
  }
}
