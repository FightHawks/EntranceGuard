#include "nfc.h"
#include "main.h"
// M1����Ϊ16��������ÿ���������ĸ��飨��0����1����2����3�����
// ��16��������64���鰴���Ե�ַ���Ϊ��0~63
// ��0�������Ŀ�0�������Ե�ַ0�飩�����ڴ�ų��̴��룬�Ѿ��̻����ɸ���
// ÿ�������Ŀ�0����1����2Ϊ���ݿ飬�����ڴ������
// ÿ�������Ŀ�3Ϊ���ƿ飨���Ե�ַΪ:��3����7����11.....����������A����ȡ���ơ�����B��

/*******************************
 *����˵����
 *1--SDA  <----->PA4
 *2--SCK  <----->PA5
 *3--MOSI <----->PA7
 *4--MISO <----->PA6
 *5--����
 *6--GND <----->GND
 *7--RST <----->PB0
 *8--VCC <----->VCC
 ************************************/
/**
 * @brief I2C�Ķ�����ʱ
 * @param None
 * @retval None
 */
static void delay2(unsigned int n)
{
    for (uint32_t i = n; i > 0; i--)
    {
        __NOP();
    }
}
#define RC522_DELAY() msDelay(1)
#define delay_us(x) delay2(x * 20)
/*ȫ�ֱ���*/
unsigned char CT[2];    // ������
unsigned char SN[4];    // ����
unsigned char RFID[16]; // ���RFID
unsigned char lxl_bit = 0;
unsigned char card1_bit = 0;
unsigned char card2_bit = 0;
unsigned char card3_bit = 0;
unsigned char card4_bit = 0;
unsigned char total = 0;
unsigned char lxl[4] = {196, 58, 104, 217};
unsigned char card_1[4] = {83, 106, 11, 1};
unsigned char card_2[4] = {208, 121, 31, 57};
unsigned char card_3[4] = {176, 177, 143, 165};
unsigned char card_4[4] = {5, 158, 10, 136};
u8 KEY[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u8 AUDIO_OPEN[6] = {0xAA, 0x07, 0x02, 0x00, 0x09, 0xBC};
unsigned char RFID1[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0x80, 0x29, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
/*��������*/
unsigned char status;
unsigned char s = 0x08;

/* ��������RC522_Init
 * ����  ����ʼ��RC522����
 * ����  ����
 * ����  : ��
 * ����  ���ⲿ����              */
void RC522_Init(void)
{
    SPI1_Init();
    RC522_Reset_Disable();     // ��RST�øߣ������ڲ���λ�׶Σ�
    PcdReset();                // ��λRC522
    PcdAntennaOff();           // �ر�����
    RC522_DELAY();             // delay 1ms
    PcdAntennaOn();            // ������
    M500PcdConfigISOType('A'); // ���ù�����ʽ
}

/* ��������SPI1_Init
 * ����  ����ʼ��SPI1������
 * ����  ����
 * ����  : ��
 * ����  ���ⲿ����              */
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

    /* USER CODE BEGIN SPI1_Init 2 */

    //   /* USER CODE END SPI1_Init 2 */
    //     SPI_InitTypeDef SPI_InitStructure;
    //     GPIO_InitTypeDef GPIO_InitStructure;
    //     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); // PORTBʱ��ʹ��

    //     // CS
    //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    //     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
    //     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
    //     GPIO_Init(GPIOA, &GPIO_InitStructure);            // �����趨������ʼ��PF0��PF1

    //     // SCK
    //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    //     GPIO_Init(GPIOA, &GPIO_InitStructure);

    //     // MISO
    //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    //     GPIO_Init(GPIOA, &GPIO_InitStructure);

    //     // MOSI
    //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    //     GPIO_Init(GPIOA, &GPIO_InitStructure);

    //     // RST
    //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    //     GPIO_Init(GPIOB, &GPIO_InitStructure);

    //     // �ø�CS��
    //     RC522_CS_Disable();

    //     // ����SPI1����
    //     SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // ȫ˫����
    //     SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       // ����ģʽ��
    //     SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                   // ��������Ϊ8λ��
    //     SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                          // ʱ�Ӽ���CPOLΪ����ʱ�͵�ƽ��
    //     SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                        // ʱ�Ӳ�����Ϊʱ�������أ������أ���
    //     SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                           // NSS����������ı䣻
    //     SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // Ԥ��Ƶϵ��64��
    //     SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  // MSB����ģʽ��
    //     SPI_InitStructure.SPI_CRCPolynomial = 7;                            // CRCУ�飻

    //     // ��ʼ��SPI1
    //     SPI_Init(SPI1, &SPI_InitStructure);

    //     // ʹ��SPI1
    //     SPI_Cmd(SPI1, ENABLE);
}

/* ��������PcdRese
 * ����  ����λRC522
 * ����  ����
 * ����  : ��
 * ����  ���ⲿ����              */
void PcdReset(void)
{
    RC522_Reset_Disable();
    delay_us(1);
    RC522_Reset_Enable();
    delay_us(1);
    RC522_Reset_Disable();
    delay_us(1);
    WriteRawRC(CommandReg, 0x0f);

    while (ReadRawRC(CommandReg) & 0x10)
        ;

    delay_us(1);
    WriteRawRC(ModeReg, 0x3D);       // ���巢�ͺͽ��ճ���ģʽ ��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL, 30);     // 16λ��ʱ����λ
    WriteRawRC(TReloadRegH, 0);      // 16λ��ʱ����λ
    WriteRawRC(TModeReg, 0x8D);      // �����ڲ���ʱ��������
    WriteRawRC(TPrescalerReg, 0x3E); // ���ö�ʱ����Ƶϵ��
    WriteRawRC(TxAutoReg, 0x40);     // ���Ʒ����ź�Ϊ100%ASK
}

/* ��������SPI_RC522_SendByte
 * ����  ����RC522����1 Byte ����
 * ����  ��byte��Ҫ���͵�����
 * ����  : RC522���ص�����
 * ����  ���ڲ�����                 */
u8 SPI_RC522_SendByte(u8 byte)
{
    // while (LL_SPI_IsActiveFlag_TXE(SPI1) == 1)
    //     ;
    // LL_SPI_TransmitData8(SPI1, byte);
    // while (LL_SPI_IsActiveFlag_TXE(SPI1) == 1)
    //     ;
    // return LL_SPI_ReceiveData8(SPI1);


    int state = 0;
	uint32_t timeout_cnt;
	static const uint32_t timeout_cnt_num = 10000;
	
	// Wait until TXE flag is set to send data 
	timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_TXE(SPI1)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			state = -1;
			break;
		}
	}
	
	// Transmit data in 16 Bit mode
	LL_SPI_TransmitData16(SPI1, byte);
	
	// Check BSY flag 
	timeout_cnt = 0;
	while(LL_SPI_IsActiveFlag_BSY(SPI1)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			state = -1;
			break;
		}
	}
	
	// Check RXNE flag 
	timeout_cnt = 0;
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1)){
		timeout_cnt ++;
		if(timeout_cnt > timeout_cnt_num){
			state = -1;
			break;
		}
	}
	
	// Read 16-Bits in the data register
	
	return LL_SPI_ReceiveData8(SPI1);

}

/* ��������ReadRawRC
 * ����  ����RC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 * ����  : �Ĵ����ĵ�ǰֵ
 * ����  ���ڲ�����                 */
u8 ReadRawRC(u8 ucAddress)
{
    u8 ucAddr, ucReturn;
    ucAddr = ((ucAddress << 1) & 0x7E) | 0x80;

    RC522_CS_Enable();
    SPI_RC522_SendByte(ucAddr);
    ucReturn = SPI_RC522_ReadByte();
    RC522_CS_Disable();
    return ucReturn;
}

/* ��������WriteRawRC
 * ����  ��дRC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ  �� ucValue��д��Ĵ�����ֵ
 * ����  : ��
 * ����  ���ڲ�����   */
void WriteRawRC(u8 ucAddress, u8 ucValue)
{
    u8 ucAddr;
    ucAddr = (ucAddress << 1) & 0x7E;

    RC522_CS_Enable();
    SPI_RC522_SendByte(ucAddr);
    SPI_RC522_SendByte(ucValue);
    RC522_CS_Disable();
}

/* ��������M500PcdConfigISOType
 * ����  ������RC522�Ĺ�����ʽ
 * ����  ��ucType��������ʽ
 * ����  : ��
 * ����  ���ⲿ����        */
void M500PcdConfigISOType(u8 ucType)
{
    if (ucType == 'A') // ISO14443_A
    {
        ClearBitMask(Status2Reg, 0x08);

        WriteRawRC(ModeReg, 0x3D);   // 3F
        WriteRawRC(RxSelReg, 0x86);  // 84
        WriteRawRC(RFCfgReg, 0x7F);  // 4F
        WriteRawRC(TReloadRegL, 30); // tmoLength);// TReloadVal = 'h6a =tmoLength(dec)
        WriteRawRC(TReloadRegH, 0);
        WriteRawRC(TModeReg, 0x8D);
        WriteRawRC(TPrescalerReg, 0x3E);
        delay_us(2);

        PcdAntennaOn(); // ������
    }
}

/*
 * ��������SetBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void SetBitMask(u8 ucReg, u8 ucMask)
{
    u8 ucTemp;

    ucTemp = ReadRawRC(ucReg);
    WriteRawRC(ucReg, ucTemp | ucMask); // set bit mask
}

/* ��������ClearBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����           */
void ClearBitMask(u8 ucReg, u8 ucMask)
{
    u8 ucTemp;
    ucTemp = ReadRawRC(ucReg);

    WriteRawRC(ucReg, ucTemp & (~ucMask)); // clear bit mask
}

/* ��������PcdAntennaOn
 * ����  ����������
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����            */
void PcdAntennaOn(void)
{
    u8 uc;
    uc = ReadRawRC(TxControlReg);

    if (!(uc & 0x03))
        SetBitMask(TxControlReg, 0x03);
}

/* ��������PcdAntennaOff
 * ����  ����������
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����             */
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

/* ��������PcdComMF522
 * ����  ��ͨ��RC522��ISO14443��ͨѶ
 * ����  ��ucCommand��RC522������
 *         pInData��ͨ��RC522���͵���Ƭ������
 *         ucInLenByte���������ݵ��ֽڳ���
 *         pOutData�����յ��Ŀ�Ƭ��������
 *         pOutLenBit���������ݵ�λ����
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ڲ�����              */
char PcdComMF522(u8 ucCommand, u8 *pInData, u8 ucInLenByte, u8 *pOutData, u32 *pOutLenBit)
{
    char cStatus = MI_ERR;
    u8 ucIrqEn = 0x00;
    u8 ucWaitFor = 0x00;
    u8 ucLastBits;
    u8 ucN;
    u32 ul;

    switch (ucCommand)
    {
    case PCD_AUTHENT:     // Mifare��֤
        ucIrqEn = 0x12;   // ��������ж�����ErrIEn  ��������ж�IdleIEn
        ucWaitFor = 0x10; // ��֤Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ
        break;

    case PCD_TRANSCEIVE:  // ���շ��� ���ͽ���
        ucIrqEn = 0x77;   // ����TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
        ucWaitFor = 0x30; // Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ�� �����жϱ�־λ
        break;

    default:
        break;
    }

    WriteRawRC(ComIEnReg, ucIrqEn | 0x80); // IRqInv��λ�ܽ�IRQ��Status1Reg��IRqλ��ֵ�෴
    ClearBitMask(ComIrqReg, 0x80);         // Set1��λ����ʱ��CommIRqReg������λ����
    WriteRawRC(CommandReg, PCD_IDLE);      // д��������
    SetBitMask(FIFOLevelReg, 0x80);        // ��λFlushBuffer����ڲ�FIFO�Ķ���дָ���Լ�ErrReg��BufferOvfl��־λ�����

    for (ul = 0; ul < ucInLenByte; ul++)
        WriteRawRC(FIFODataReg, pInData[ul]); // д���ݽ�FIFOdata

    WriteRawRC(CommandReg, ucCommand); // д����

    if (ucCommand == PCD_TRANSCEIVE)
        SetBitMask(BitFramingReg, 0x80); // StartSend��λ�������ݷ��� ��λ���շ�����ʹ��ʱ����Ч

    ul = 1000; // ����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms

    do // ��֤ ��Ѱ���ȴ�ʱ��
    {
        ucN = ReadRawRC(ComIrqReg); // ��ѯ�¼��ж�
        ul--;
    } while ((ul != 0) && (!(ucN & 0x01)) && (!(ucN & ucWaitFor))); // �˳�����i=0,��ʱ���жϣ���д��������

    ClearBitMask(BitFramingReg, 0x80); // ��������StartSendλ

    if (ul != 0)
    {
        if (!((ReadRawRC(ErrorReg) & 0x1B))) // �������־�Ĵ���BufferOfI CollErr ParityErr ProtocolErr
        {
            cStatus = MI_OK;

            if (ucN & ucIrqEn & 0x01) // �Ƿ�����ʱ���ж�
                cStatus = MI_NOTAGERR;

            if (ucCommand == PCD_TRANSCEIVE)
            {
                ucN = ReadRawRC(FIFOLevelReg); // ��FIFO�б�����ֽ���

                ucLastBits = ReadRawRC(ControlReg) & 0x07; // �����յ����ֽڵ���Чλ��

                if (ucLastBits)
                    *pOutLenBit = (ucN - 1) * 8 + ucLastBits; // N���ֽ�����ȥ1�����һ���ֽڣ�+���һλ��λ�� ��ȡ����������λ��
                else
                    *pOutLenBit = ucN * 8; // �����յ����ֽ������ֽ���Ч

                if (ucN == 0)
                    ucN = 1;

                if (ucN > MAXRLEN)
                    ucN = MAXRLEN;

                for (ul = 0; ul < ucN; ul++)
                    pOutData[ul] = ReadRawRC(FIFODataReg);
            }
        }
        else
            cStatus = MI_ERR;
    }

    SetBitMask(ControlReg, 0x80); // stop timer now
    WriteRawRC(CommandReg, PCD_IDLE);

    return cStatus;
}

/* ��������PcdRequest
 * ����  ��Ѱ��
 * ����  ��ucReq_code��Ѱ����ʽ
 *                     = 0x52��Ѱ��Ӧ�������з���14443A��׼�Ŀ�
 *                     = 0x26��Ѱδ��������״̬�Ŀ�
 *         pTagType����Ƭ���ʹ���
 *                   = 0x4400��Mifare_UltraLight
 *                   = 0x0400��Mifare_One(S50)
 *                   = 0x0200��Mifare_One(S70)
 *                   = 0x0800��Mifare_Pro(X))
 *                   = 0x4403��Mifare_DESFire
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����            */
char PcdRequest(u8 ucReq_code, u8 *pTagType)
{
    char cStatus;
    u8 ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ClearBitMask(Status2Reg, 0x08);  // ����ָʾMIFARECyptol��Ԫ��ͨ�Լ����п�������ͨ�ű����ܵ����
    WriteRawRC(BitFramingReg, 0x07); //	���͵����һ���ֽڵ� ��λ
    SetBitMask(TxControlReg, 0x03);  // TX1,TX2�ܽŵ�����źŴ��ݾ����͵��Ƶ�13.56�������ز��ź�

    ucComMF522Buf[0] = ucReq_code; // ���� ��Ƭ������

    cStatus = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &ulLen); // Ѱ��

    if ((cStatus == MI_OK) && (ulLen == 0x10)) // Ѱ���ɹ����ؿ�����
    {
        *pTagType = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    }

    else
        cStatus = MI_ERR;

    return cStatus;
}

/* ��������PcdAnticoll
 * ����  ������ײ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����           */
char PcdAnticoll(u8 *pSnr)
{
    char cStatus;
    u8 uc, ucSnr_check = 0;
    u8 ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ClearBitMask(Status2Reg, 0x08);  // ��MFCryptol Onλ ֻ�гɹ�ִ��MFAuthent����󣬸�λ������λ
    WriteRawRC(BitFramingReg, 0x00); // ����Ĵ��� ֹͣ�շ�
    ClearBitMask(CollReg, 0x80);     // ��ValuesAfterColl���н��յ�λ�ڳ�ͻ�����

    ucComMF522Buf[0] = 0x93; // ��Ƭ����ͻ����
    ucComMF522Buf[1] = 0x20;

    cStatus = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &ulLen); // �뿨Ƭͨ��

    if (cStatus == MI_OK) // ͨ�ųɹ�
    {
        for (uc = 0; uc < 4; uc++)
        {
            *(pSnr + uc) = ucComMF522Buf[uc]; // ����UID
            ucSnr_check ^= ucComMF522Buf[uc];
        }

        if (ucSnr_check != ucComMF522Buf[uc])
            cStatus = MI_ERR;
    }

    SetBitMask(CollReg, 0x80);

    return cStatus;
}

/* ��������PcdSelect
 * ����  ��ѡ����Ƭ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����         */
char PcdSelect(u8 *pSnr)
{
    char ucN;
    u8 uc;
    u8 ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;

    for (uc = 0; uc < 4; uc++)
    {
        ucComMF522Buf[uc + 2] = *(pSnr + uc);
        ucComMF522Buf[6] ^= *(pSnr + uc);
    }

    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);
    ClearBitMask(Status2Reg, 0x08);
    ucN = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &ulLen);

    if ((ucN == MI_OK) && (ulLen == 0x18))
        ucN = MI_OK;
    else
        ucN = MI_ERR;

    return ucN;
}

/* ��������CalulateCRC
 * ����  ����RC522����CRC16
 * ����  ��pIndata������CRC16������
 *         ucLen������CRC16�������ֽڳ���
 *         pOutData����ż�������ŵ��׵�ַ
 * ����  : ��
 * ����  ���ڲ�����              */
void CalulateCRC(u8 *pIndata, u8 ucLen, u8 *pOutData)
{
    u8 uc, ucN;

    ClearBitMask(DivIrqReg, 0x04);
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80);

    for (uc = 0; uc < ucLen; uc++)
        WriteRawRC(FIFODataReg, *(pIndata + uc));

    WriteRawRC(CommandReg, PCD_CALCCRC);
    uc = 0xFF;

    do
    {
        ucN = ReadRawRC(DivIrqReg);
        uc--;
    } while ((uc != 0) && !(ucN & 0x04));

    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/* ��������PcdAuthState
 * ����  ����֤��Ƭ����
 * ����  ��ucAuth_mode��������֤ģʽ
 *                     = 0x60����֤A��Կ
 *                     = 0x61����֤B��Կ
 *         u8 ucAddr�����ַ
 *         pKey������
 *         pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����          */
char PcdAuthState(u8 ucAuth_mode, u8 ucAddr, u8 *pKey, u8 *pSnr)
{
    char cStatus;
    u8 uc, ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ucComMF522Buf[0] = ucAuth_mode;
    ucComMF522Buf[1] = ucAddr;

    for (uc = 0; uc < 6; uc++)
        ucComMF522Buf[uc + 2] = *(pKey + uc);

    for (uc = 0; uc < 6; uc++)
        ucComMF522Buf[uc + 8] = *(pSnr + uc);

    cStatus = PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &ulLen);

    if ((cStatus != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {
        cStatus = MI_ERR;
    }

    return cStatus;
}

/* ��������PcdWrite
 * ����  ��д���ݵ�M1��һ��
 * ����  ��u8 ucAddr�����ַ
 *         pData��д������ݣ�16�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����           */
char PcdWrite(u8 ucAddr, u8 *pData)
{
    char cStatus;
    u8 uc, ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = ucAddr;

    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    cStatus = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &ulLen);

    if ((cStatus != MI_OK) || (ulLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        cStatus = MI_ERR;

    if (cStatus == MI_OK)
    {
        memcpy(ucComMF522Buf, pData, 16);
        for (uc = 0; uc < 16; uc++)
            ucComMF522Buf[uc] = *(pData + uc);

        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        cStatus = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &ulLen);

        if ((cStatus != MI_OK) || (ulLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
            cStatus = MI_ERR;
    }

    return cStatus;
}

/* ��������PcdRead
 * ����  ����ȡM1��һ������
 * ����  ��u8 ucAddr�����ַ
 *         pData�����������ݣ�16�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����             */
char PcdRead(u8 ucAddr, u8 *pData)
{
    char cStatus;
    u8 uc, ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = ucAddr;

    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    cStatus = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &ulLen);

    if ((cStatus == MI_OK) && (ulLen == 0x90))
    {
        for (uc = 0; uc < 16; uc++)
            *(pData + uc) = ucComMF522Buf[uc];
    }

    else
        cStatus = MI_ERR;

    return cStatus;
}

/* ��������PcdHalt
 * ����  �����Ƭ��������״̬
 * ����  ����
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����        */
char PcdHalt(void)
{
    u8 ucComMF522Buf[MAXRLEN];
    u32 ulLen;

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;

    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
    PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &ulLen);

    return MI_OK;
}
