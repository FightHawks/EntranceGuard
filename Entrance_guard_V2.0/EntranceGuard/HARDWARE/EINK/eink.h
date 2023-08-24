#ifndef __EINK_H
#define __EINK_H 

#include "main.h"	

//-----------------����LED�˿ڶ���---------------- 

#define LED_ON    LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_15, LL_GPIO_PIN_RESET)
#define LED_OFF   LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_15, LL_GPIO_PIN_SET)

//-----------------EINK�˿ڶ���---------------- 

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

#define ROTATE_0            0   //��Ļ������ʾ
#define ROTATE_90           90  //��Ļ��ת90����ʾ
#define ROTATE_180          180 //��Ļ��ת180����ʾ
#define ROTATE_270          270 //��Ļ��ת270����ʾ


#define WHITE          0xFF   //��ʾ��ɫ
#define BLACK          0x00   //��ʾ��ɫ
#define Display_buff_len   1
extern u8 Canvas[Display_buff_len];
void EINK_GPIOInit(void);   //��ʼ��GPIO
void EINK_WR_Bus(u8 dat);   //ģ��SPIʱ��
void EINK_WR_REG(u8 reg);   //д��һ������
void EINK_WR_DATA8(u8 dat); //д��һ���ֽ�
void EINK_AddressSet(u16 xs,u16 ys,u16 xe,u16 ye);  //����λ�ú���
void EINK_Init(void);       //��ʼ����Ļ

void Epaper_READBUSY(void); 
void EPD_Update(void);      

void Paint_NewImage(u8 *image,u16 Width,u16 Height,u16 Rotate,u16 Color); 					 //��������������ʾ����
void EINK_Clear(u16 Color);																													 //��������
void EINK_DrawPoint(u16 Xpoint,u16 Ypoint,u16 Color);                                //����
void EINK_DrawLine(u16 Xstart,u16 Ystart,u16 Xend,u16 Yend,u16 Color);               //����
void EINK_DrawRectangle(u16 Xstart,u16 Ystart,u16 Xend,u16 Yend,u16 Color,u8 mode);  //������
void EINK_DrawCircle(u16 X_Center,u16 Y_Center,u16 Radius,u16 Color,u8 mode);        //��Բ
void EINK_ShowChar(u16 x,u16 y,u16 chr,u16 size1,u16 color);                         //��ʾ�ַ�
void EINK_ShowString(u16 x,u16 y,u8 *chr,u16 size1,u16 color);                       //��ʾ�ַ���
void EINK_ShowNum(u16 x,u16 y,u32 num,u16 len,u16 size1,u16 color);                  //��ʾ����
void EINK_ShowChinese(u16 x,u16 y,u16 num,u16 size1,u16 color);                      //��ʾ����
void EINK_ShowPicture(u16 x,u16 y,u16 sizex,u16 sizey,const u8 BMP[],u16 color);      //��ʾͼƬ
void EINK_Display(unsigned char *Image);																		 				 //���µ���Ļ
void EINK_GUIInit(void);																														 //��ĻGUI��ʼ��
#define elink_Display EINK_Display(Canvas)

#endif


