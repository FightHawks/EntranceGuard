#ifndef __EINK_H
#define __EINK_H 

#include "main.h"	

//-----------------测试LED端口定义---------------- 

#define LED_ON    LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_15, LL_GPIO_PIN_RESET)
#define LED_OFF   LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_15, LL_GPIO_PIN_SET)

//-----------------EINK端口定义---------------- 

#define EINK_SCL_Clr() LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_5)//CLK
#define EINK_SCL_Set() LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_5)

#define EINK_SDA_Clr() LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_7)//DIN
#define EINK_SDA_Set() LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_7)

#define EINK_RES_Clr() LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_4)//RES
#define EINK_RES_Set() LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_4)

#define EINK_DC_Clr() LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_6)//DC
#define EINK_DC_Set() LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_6)
 
#define EINK_CS_Clr()  LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_3)//CS
#define EINK_CS_Set()  LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_3)

// #define EINK_BUSY()    HAL_GPIO_ReadPin(GPIOA,LL_GPIO_PIN_5)

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int


#define EINK_W   200
#define EINK_H   200


typedef struct {
    u8 *Image;
    u16 Width;
    u16 Height;
    u16 WidthMemory;
    u16 HeightMemory;
    u16 Color;
    u16 Rotate;
    u16 WidthByte;
    u16 HeightByte;
} PAINT;
extern PAINT Paint;

#define ROTATE_0            0   //屏幕正向显示
#define ROTATE_90           90  //屏幕旋转90度显示
#define ROTATE_180          180 //屏幕旋转180度显示
#define ROTATE_270          270 //屏幕旋转270度显示


#define WHITE          0xFF   //显示白色
#define BLACK          0x00   //显示黑色
#define Display_buff_len   1
extern u8 Canvas[Display_buff_len];
void EINK_GPIOInit(void);   //初始化GPIO
void EINK_WR_Bus(u8 dat);   //模拟SPI时序
void EINK_WR_REG(u8 reg);   //写入一个命令
void EINK_WR_DATA8(u8 dat); //写入一个字节
void EINK_AddressSet(u16 xs,u16 ys,u16 xe,u16 ye);  //设置位置函数
void EINK_Init(void);       //初始化屏幕

void Epaper_READBUSY(void); 
void EPD_Update(void);      

void Paint_NewImage(u8 *image,u16 Width,u16 Height,u16 Rotate,u16 Color); 					 //创建画布控制显示方向
void EINK_Clear(u16 Color);																													 //清屏函数
void EINK_DrawPoint(u16 Xpoint,u16 Ypoint,u16 Color);                                //画点
void EINK_DrawLine(u16 Xstart,u16 Ystart,u16 Xend,u16 Yend,u16 Color);               //画线
void EINK_DrawRectangle(u16 Xstart,u16 Ystart,u16 Xend,u16 Yend,u16 Color,u8 mode);  //画矩形
void EINK_DrawCircle(u16 X_Center,u16 Y_Center,u16 Radius,u16 Color,u8 mode);        //画圆
void EINK_ShowChar(u16 x,u16 y,u16 chr,u16 size1,u16 color);                         //显示字符
void EINK_ShowString(u16 x,u16 y,u8 *chr,u16 size1,u16 color);                       //显示字符串
void EINK_ShowNum(u16 x,u16 y,u32 num,u16 len,u16 size1,u16 color);                  //显示数字
void EINK_ShowChinese(u16 x,u16 y,u16 num,u16 size1,u16 color);                      //显示中文
void EINK_ShowPicture(u16 x,u16 y,u16 sizex,u16 sizey,const u8 BMP[],u16 color);      //显示图片
void EINK_Display(unsigned char *Image);																		 				 //更新到屏幕
void EINK_GUIInit(void);																														 //屏幕GUI初始化
#define elink_Display EINK_Display(Canvas)

#endif


