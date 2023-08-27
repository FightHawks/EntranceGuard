#include "OLED.h"
// #include "OLED_Codetab.h"

/*��������*/
#define OLED_SCL_Clr()      LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6)//CLK
#define OLED_SCL_Set()      LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6)
#define OLED_SDA_Clr()      LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_7)//DIN
#define OLED_SDA_Set()      LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_7)
/*���ų�ʼ��*/
void OLED_I2C_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
  /**/
  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  /*Configure GPIO pins : PA0 PA1 PA2 PA15 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin Output Level */
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6 | LL_GPIO_PIN_7);
	
	 OLED_SCL_Set();
	 OLED_SDA_Set();
}

/**
  * @brief  I2C��ʼ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Start(void)
{
	 OLED_SDA_Set();
	 OLED_SCL_Set();
	 OLED_SDA_Clr();
	 OLED_SCL_Clr();
}

/**
  * @brief  I2Cֹͣ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Stop(void)
{
	 OLED_SDA_Clr();
	 OLED_SCL_Set();
	 OLED_SDA_Set();
}

/**
  * @brief  I2C����һ���ֽ�
  * @param  Byte Ҫ���͵�һ���ֽ�
  * @retval ��
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if (Byte & (0x80 >> i))
		{
			OLED_SDA_Set();
		}
		else
		{
			OLED_SDA_Clr();
		}
		 OLED_SCL_Set();
		 OLED_SCL_Clr();
	}
	 OLED_SCL_Set();	//�����һ��ʱ�ӣ�������Ӧ���ź�
	 OLED_SCL_Clr();
}

/**
  * @brief  OLEDд����
  * @param  Command Ҫд�������
  * @retval ��
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLEDд����
  * @param  Data Ҫд�������
  * @retval ��
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x40);		//д����
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand(0xAE);	//�ر���ʾ
	OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0xf0);	//ԭ����80
	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x3F);
	OLED_WriteCommand(0xD3);	//������ʾƫ��
	OLED_WriteCommand(0x00);
	OLED_WriteCommand(0x40);	//������ʾ��ʼ��
	OLED_WriteCommand(0xA0);	//�������ҷ���0xA1���� 0xA0���ҷ���
	OLED_WriteCommand(0xC0);	//�������·���0xC8���� 0xC0���·���
	OLED_WriteCommand(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand(0x12);
	// OLED_WriteCommand(0x81);	//���öԱȶȿ���
	// OLED_WriteCommand(0xFF);
	Set_Contrast(0xff);	//���öԱȶȿ���
	OLED_WriteCommand(0xD9);	//����Ԥ�������
	OLED_WriteCommand(0xf1);	//ԭ��0xF1
	OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);
	OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�
	// OLED_WriteCommand(0xA6);	//��������/��ת��ʾ
	Inverse_Display(OFF);
	Set_Disp_Time(0xF0);
	OLED_WriteCommand(0x8D);	//���ó���
	OLED_WriteCommand(0x14);
	OLED_WriteCommand(0xAF);	//������ʾ
	OLED_Clear();				//OLED����
}

/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�õ�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�ø�4λ
}

void OLED_SetPos(unsigned char x,unsigned char y)  //�����������
{
	OLED_WriteCommand(0xb0+y);
	OLED_WriteCommand((x&0xf0)>>4|0x10); //ȡ��λ
	OLED_WriteCommand((x&0x0f)|0x01);    //ȡ��λ
}

//ȫ�����
void OLED_Fill(unsigned char Fill_Data)  
{
	unsigned char m,n;
	
	for(m=0;m<8;m++){
		OLED_WriteCommand(0xb0+m);
		OLED_WriteCommand(0x00);
		OLED_WriteCommand(0x10);
	
	  for(n=0;n<128;n++){
		OLED_WriteData(Fill_Data);
		}
	}
}

//ȫ�����
void OLED_Fill_Fast(unsigned char Fill_Data)  
{
	unsigned char m,n;
	
	for(m=0;m<8;m++){
		OLED_WriteCommand(0xb0+m);
		OLED_WriteCommand(0x00);
		OLED_WriteCommand(0x10);
		
		OLED_I2C_Start();
		OLED_I2C_SendByte(0x78);		//�ӻ���ַ
		OLED_I2C_SendByte(0x40);		//д����
		
		for(n=0;n<128;n++){
			// OLED_WriteData(Fill_Data);
			OLED_I2C_SendByte(Fill_Data);
		}
		OLED_I2C_Stop();
	}
}

/**
  * @brief  OLED����
  * @param  ��
  * @retval ��
  */
void OLED_Clear(void)
{  
	OLED_Fill_Fast(OLED_BLACK);
	/* uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	} */
}


void OLED_ON(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	/**/
	/* GPIO Ports Clock Enable */
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	/*Configure GPIO pins : PA0 PA1 PA2 PA15 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	OLED_WriteCommand(0X8D);  //���õ�ɱ�
	OLED_WriteCommand(0X14);  //������ɱ�
	OLED_WriteCommand(0XAF);  //OLED����
}
 
void OLED_OFF(void)
{
	// ����ģʽ��sda scl�ߵ�ƽ������ͣ�~9uA��
	// LL_GPIO_InitTypeDef GPIO_InitStruct;

	OLED_WriteCommand(0X8D);  //���õ�ɱ�
	OLED_WriteCommand(0X10);  //�رյ�ɱ�
	OLED_WriteCommand(0XAE);  //OLED����

	/**/
	/* GPIO Ports Clock Enable */
	// LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	// LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	/*Configure GPIO pins : PA0 PA1 PA2 PA15 */
	// GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
	// GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	// GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	// GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	// LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/**
  * @brief  ������ʾʱ��
  * @param  hex:range(0x10h-0xF0h)
  * @retval ��
  */
void Set_Disp_Time(uint8_t command)
{
	OLED_WriteCommand(0xD5);	//�����ڲ�����
	OLED_WriteCommand(command);
}

/**
  * @brief  ������ʾ�Աȶ�
  * @param  hex:range(0x00h-0xFFh)
  * @retval ��
  */
void Set_Contrast(uint8_t command)
{
	OLED_WriteCommand(0x81);
	OLED_WriteCommand(command);
}

//��ɫ��ת
void Inverse_Display(uint8_t state)
{
	if(state == ON) OLED_WriteCommand(0xA7);
	else if(state == OFF) OLED_WriteCommand(0xA6);
}

/**
  * @brief  OLED��ʾһ���ַ�
  * @param  y ��λ�ã���Χ��1~8
  * @param  x ��λ�ã���Χ��0~128
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @param  Char_Size �ַ��ߴ磬������12(6x8�ַ�)��16��8x16�ַ�)
  * @retval ��
  */
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//???????			
		if(x>128-1){x=0;y=y+2;}
		// if(Char_Size == 16)
		// {
		// 	OLED_SetPos(x,y);	
		// 	for(i=0;i<8;i++)
		// 		OLED_WriteData(OLED_F8X16[c*16+i]);
		// 	OLED_SetPos(x,y+1);
		// 	for(i=0;i<8;i++)
		// 		OLED_WriteData(OLED_F8X16[c*16+i+8]);
		// }
		// else 
		if(Char_Size == 12)
		{	
			OLED_SetPos(x,y);
			for(i=0;i<6;i++)
				OLED_WriteData(asc2_0806[c][i]);
			
		}
}


//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
// void OLED_DrawPoint(u8 x,u8 y,u8 t)
// {
// 	u8 pos,bx,temp=0;
// 	if(x>127||y>63)return;//������Χ��.
// 	pos=7-y/8;
// 	bx=y%8;
// 	temp=1<<(7-bx);
// 	if(t)OLED_GRAM[x][pos]|=temp;
// 	else OLED_GRAM[x][pos]&=~temp;	    
// }

/**
  * @brief  OLED��ʾ�ַ���
  * @param  x ��ʼ��λ�ã���Χ��0~127
  * @param  Column ��ʼ��λ�ã���Χ��0~7
  * @param  ch[] Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @param  TextSize �ַ���С(1:6*8 ; 2:8*16)
  * @param  mode:1
  * @retval ��
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize, bool mode)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++){
					if(mode) OLED_WriteData(asc2_0806[c][i]);
					else if(!mode) OLED_WriteData(~asc2_0806[c][i]);
				}
				x += 6;
				j++;
			}
		}break;
		// case 2:
		// {
		// 	while(ch[j] != '\0')
		// 	{
		// 		c = ch[j] - 32;
		// 		if(x > 120)
		// 		{
		// 			x = 0;
		// 			y++;
		// 		}
		// 		OLED_SetPos(x,y);
		// 		for(i=0;i<8;i++){
		// 			if(mode) OLED_WriteData(OLED_F8X16[c*16+i]);
		// 			else if(!mode) OLED_WriteData(~OLED_F8X16[c*16+i]);
		// 		}
		// 		OLED_SetPos(x,y+1);
		// 		for(i=0;i<8;i++){
		// 			if(mode) OLED_WriteData(OLED_F8X16[c*16+i+8]);
		// 			else if(!mode) OLED_WriteData(~OLED_F8X16[c*16+i+8]);
		// 		}
		// 		x += 8;
		// 		j++;
		// 	}
		// }break;
	}
}

/**
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED��ʾ����
  * @param  x ��ʼ��λ�ã���Χ��0~127
  * @param  Column ��ʼ��λ�ã���Χ��1~7
  * @param  num Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Size2 �ַ���С(12:6*8 ; 16:8*16)
  * @retval ��
  */
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

// //��ʾ���� ��x��������  y��������  N��������
// void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N)
// {
//     unsigned char wm=0;
// 	unsigned int addr = 32*N;
// 	OLED_SetPos(x,y);
//     for(wm=0;wm<16;wm++)
// 	{
// 		OLED_WriteData( F16X16[addr]);
// 		addr +=1;
// 	}
// 	OLED_SetPos(x,y+1);
// 	for(wm=0;wm<16;wm++)
// 	{
// 		OLED_WriteData( F16X16[addr]);
// 		addr +=1;
// 	}
 
// }

//��ʾ���ִ�
//�����Լ�д����ʾ�����ַ����ĺ�����Ҫ�Ȱ������ַ�����������������ʽ�������������ȡ��ģ�����asc.h��Ӧ��λ��(��������)
//��������ֱ�Ϊ��x:��ʼ������
//y��������(����0-7)
//begin:����������ַ����ĵ�һ����������asc.c�ֿ���������  
//num:����Ҫ��д������
//����Ҫ����ԡ���ȡ����ģ����������������ֿ������Ϊ0,1,������0��������ڶ��У����x��0��y��2��begin��0��num��2
// void OLED_ShowCN_Str(u8 x , u8 y , u8 begin , u8 num)
// {
// 	u8 i;
// 	for(i=0;i<num;i++){OLED_ShowCN(i*16+x,y,i+begin);}
// }

// Parameters     : x0,y0 ��ʼ������(x0:0~127, y0:0~7); x1,y1 ͼƬ�յ�����(x1:1~128,y1:1~8)
// Description    : ѡ��ͼƬ
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_WriteData(BMP[j++]);
		}
	}
}

// Parameters     : x0,y0 ��ʼ������(x0:0~127, y0:0~7); x1,y1 ͼƬ�յ�����(x1:1~128,y1:1~8)
// Description    : ѡ��ͼƬ
void OLED_DrawBMP_Fast(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);

		OLED_I2C_Start();
		OLED_I2C_SendByte(0x78);		//�ӻ���ַ
		OLED_I2C_SendByte(0x40);		//д����
		
		for(x=x0;x<x1;x++)
		{
			// OLED_WriteData(BMP[j++]);
			OLED_I2C_SendByte(BMP[j++]);
		}
		OLED_I2C_Stop();
	}
}

/*
	@brief			????
	@param			x0:?????
				y0:?????
				x1:?????
				y1:?????
				k: ???
				m: ??????
				BMP[][m]:?????????
	@retval			?
 */
void OLED_DrawGIF(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1, unsigned char k, int m, unsigned char GIF[][294])
{
	unsigned int j=0; //????
 	unsigned char x,y,i; //????
  
 	if(y1%8==0) y=y1/8;   //????????8????
 	 else y=y1/8+1;
	for (i=0;i<k;i++) //???????
	{
		j = 0;
		for(y=y0;y<y1;y++) //??????,?????
		{
			OLED_SetPos(x0,y); //?????????
   			
			for(x=x0;x<x1;x++) //?x1 - x0 ?
	    		{
						
	    			OLED_WriteData(GIF[i][j++]);	//?????    	
	    		}
		}
		//delay_ms(80);//?????????

	}
}

