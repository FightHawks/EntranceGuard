#ifndef __OLED_H
#define __OLED_H
#include "main.h"
#include "font.h"

#define OLED_BLACK  0x00
#define OLED_WHITE 0xff
#define SIZE_0806  1
#define SIZE_1608  2

#define OFF        0
#define ON         1

// 颜色翻转
#define NORMAL     1
#define REVERSE    0

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ON(void);
void OLED_OFF(void);
void Set_Disp_Time(uint8_t command);
void Set_Contrast(uint8_t command);
void Inverse_Display(uint8_t state);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize, bool mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
// void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
// void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
// void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_Fill(unsigned char Fill_Data);
void OLED_Fill_Fast(unsigned char Fill_Data);
void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N);
void OLED_ShowCN_Str(u8 x , u8 y , u8 begin , u8 num);
void OLED_SetPos(unsigned char x,unsigned char y);  //设置起点坐标
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_DrawBMP_Fast(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_DrawGIF(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1, unsigned char k, int m, unsigned char GIF[][294]);

#endif
