#include <stdio.h>
#include "SI522A_interface.h"
#include "main.h"

/* 函数名：SPI1_Init
 * 描述  ：初始化SPI1的配置
 * 输入  ：无
 * 返回  : 无
 * 调用  ：外部调用              */
void SPI1_Init(void)
{
    /* USER CODE BEGIN SPI1_Init 0 */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_SPI_InitTypeDef SPI_InitStruct = {0};
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
    /* USER CODE END SPI1_Init 0 */

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    /**SPI1 GPIO Configuration
    PA5   ------> SPI1_SCK
    PA6   ------> SPI1_MISO
    PA7   ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_Enable(SPI1);
}

void SI522A_SPI_LL_Init()
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );		
 
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//CE
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  			
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// GPIO_SetBits(GPIOA,GPIO_Pin_2); // CE
	
	// GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;	//IRQ
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_InitStructure.GPIO_PuPd = IRQMODE;     //新增修改
	// GPIO_Init(GPIOA, &GPIO_InitStructure);
 	
	// // Initializes the SPI interface
	// SI522_CS_Enable(); 
	// SPI1_Init();
	// SPI1_SetSpeed(SPI_BaudRatePrescaler_16);
	// SI522_CS_Enable(); 
    SPI1_Init();

}

void SI522A_SPI_LL_WriteRawRC(unsigned char RegAddr,unsigned char value)
{
	SI522_CS_Enable();
	RegAddr = (RegAddr & 0x3f) << 1;                 //code the first byte	
	SI522_CS_Disable();	
	LL_SPI_TransmitData8(SPI1,RegAddr);	//write address first
	LL_SPI_TransmitData8(SPI1,value);				//write value then 	
	SI522_CS_Enable();
}
u8 SPI1_ReadWriteByte(u8 TxData)
{		
    int state = 0;
    uint32_t timeout_cnt;
    static const uint32_t timeout_cnt_num = 10000;

    // Wait until TXE flag is set to send data
    timeout_cnt = 0;
    while (!LL_SPI_IsActiveFlag_TXE(SPI1))
    {
        timeout_cnt++;
        if (timeout_cnt > timeout_cnt_num)
        {
            state = -1;
            break;
        }
    }
	LL_SPI_TransmitData8(SPI1,TxData); //通过外设SPIx发送一个数据  
    // Check BSY flag
    timeout_cnt = 0;
    while (LL_SPI_IsActiveFlag_BSY(SPI1))
    {
        timeout_cnt++;
        if (timeout_cnt > timeout_cnt_num)
        {
            state = -1;
            break;
        }
    }

    // Check RXNE flag
    timeout_cnt = 0;
    while (!LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
        timeout_cnt++;
        if (timeout_cnt > timeout_cnt_num)
        {
            state = -1;
            break;
        }
    }						    
	return LL_SPI_ReceiveData8(SPI1); //返回通过SPIx最近接收的数据					    
}
unsigned char SI522A_SPI_LL_ReadRawRC(unsigned char RegAddr)
{
	uint8_t RegVal=0;

	RegAddr = (RegAddr & 0x3f) << 1 | 0x80;   //code the first byte	

	SI522_CS_Disable();
	SPI1_ReadWriteByte(RegAddr);	//write address first
	RegVal = SPI1_ReadWriteByte(0);	//write value then 	
	SI522_CS_Enable();
	
	return RegVal;
}

void I_SI522A_IO_Init(void)
{
    SI522A_SPI_LL_Init();
}
void I_SI522A_IO_Write(unsigned char RegAddr,unsigned char value)
{
    SI522A_SPI_LL_WriteRawRC(RegAddr,value);
}
unsigned char I_SI522A_IO_Read(unsigned char RegAddr)
{
    return SI522A_SPI_LL_ReadRawRC(RegAddr);
}


