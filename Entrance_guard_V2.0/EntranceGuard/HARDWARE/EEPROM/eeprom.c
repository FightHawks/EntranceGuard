#include "eeprom.h"
// #include "acousto_optic.h"
// #include "string.h"
#define DELAY_TIME 10

/**
 * @brief SDA线输入模式配置
 * @param None
 * @retval None
 */
void SDA_Input_Mode()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  /**/
  /*Configure GPIO pins : PA0 PA1 PA2 PA15 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin Output Level */
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
}

/**
 * @brief SDA线输出模式配置
 * @param None
 * @retval None
 */
void SDA_Output_Mode()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  /**/
  /*Configure GPIO pins : PA0 PA1 PA2 PA15 */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin Output Level */
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
}

/**
 * @brief SDA线输出一个位
 * @param val 输出的数据
 * @retval None
 */
void SDA_Output(uint16_t val)
{
  if (val)
  {
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
  }
  else
  {
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
  }
}

/**
 * @brief SCL线输出一个位
 * @param val 输出的数据
 * @retval None
 */
void SCL_Output(uint16_t val)
{
  if (val)
  {
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
  }
  else
  {
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
  }
}

/**
 * @brief SDA输入一位
 * @param None
 * @retval GPIO读入一位
 */
uint8_t SDA_Input(void)
{
  if (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7) == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief I2C的短暂延时
 * @param None
 * @retval None
 */
static void delay1(unsigned int n)
{
  for (uint32_t i = n; i > 0; i--)
  {
    __NOP();
  }

  // // uint32_t i;
  // // for ( i = 0; i < n; ++i);
  // uint16_t  differ = 0xffff-n-105;
  // //设置定时器2的技术初始值
  // __HAL_TIM_SetCounter(&htim7,differ);
  // //开启定时器
  // HAL_TIM_Base_Start(&htim7);

  // while( differ<0xffff-105)
  //   {
  //     differ = __HAL_TIM_GetCounter(&htim7);
  //   };
  // //关闭定时器
  // HAL_TIM_Base_Stop(&htim7);
}

/**
 * @brief I2C起始信号
 * @param None
 * @retval None
 */
void I2CStart(void)
{
  SDA_Output(1);
  delay1(DELAY_TIME);
  SCL_Output(1);
  delay1(DELAY_TIME);
  SDA_Output(0);
  delay1(DELAY_TIME);
  SCL_Output(0);
  delay1(DELAY_TIME);
}

/**
 * @brief I2C结束信号
 * @param None
 * @retval None
 */
void I2CStop(void)
{
  SCL_Output(0);
  delay1(DELAY_TIME);
  SDA_Output(0);
  delay1(DELAY_TIME);
  SCL_Output(1);
  delay1(DELAY_TIME);
  SDA_Output(1);
  delay1(DELAY_TIME);
}

/**
 * @brief I2C等待确认信号
 * @param None
 * @retval None
 */
unsigned char I2CWaitAck(void)
{
  unsigned short cErrTime = 5;
  SDA_Input_Mode();
  delay1(DELAY_TIME);
  SCL_Output(1);
  delay1(DELAY_TIME);
  while (SDA_Input())
  {
    cErrTime--;
    delay1(DELAY_TIME);
    if (0 == cErrTime)
    {
      SDA_Output_Mode();
      I2CStop();
      return ERROR;
    }
  }
  SDA_Output_Mode();
  SCL_Output(0);
  delay1(DELAY_TIME);
  return SUCCESS;
}

/**
 * @brief I2C发送确认信号
 * @param None
 * @retval None
 */
void I2CSendAck(void)
{
  SDA_Output(0);
  delay1(DELAY_TIME);
  delay1(DELAY_TIME);
  SCL_Output(1);
  delay1(DELAY_TIME);
  SCL_Output(0);
  delay1(DELAY_TIME);
}

/**
 * @brief I2C发送非确认信号
 * @param None
 * @retval None
 */
void I2CSendNotAck(void)
{
  SDA_Output(1);
  delay1(DELAY_TIME);
  delay1(DELAY_TIME);
  SCL_Output(1);
  delay1(DELAY_TIME);
  SCL_Output(0);
  delay1(DELAY_TIME);
}

/**
 * @brief I2C发送一个字节
 * @param cSendByte 需要发送的字节
 * @retval None
 */
void I2CSendByte(unsigned char cSendByte)
{
  unsigned char i = 8;
  while (i--)
  {
    SCL_Output(0);
    delay1(DELAY_TIME);
    SDA_Output(cSendByte & 0x80);
    delay1(DELAY_TIME);
    cSendByte += cSendByte;
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
  }
  SCL_Output(0);
  delay1(DELAY_TIME);
}

/**
 * @brief I2C接收一个字节
 * @param None
 * @retval 接收到的字节
 */
unsigned char I2CReceiveByte(void)
{
  unsigned char i = 8;
  unsigned char cR_Byte = 0;
  SDA_Input_Mode();
  while (i--)
  {
    cR_Byte += cR_Byte;
    SCL_Output(0);
    delay1(DELAY_TIME);
    delay1(DELAY_TIME);
    SCL_Output(1);
    delay1(DELAY_TIME);
    cR_Byte |= SDA_Input();
  }
  SCL_Output(0);
  delay1(DELAY_TIME);
  SDA_Output_Mode();
  return cR_Byte;
}

//
void I2CInit(void)
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
}

uint8_t eeprom_read(uint8_t addr)
{
  uint8_t dat;
  I2CStart();
  I2CSendByte(0xa0);
  I2CWaitAck();
  I2CSendByte(addr);
  I2CWaitAck();
  I2CStop();

  I2CStart();
  I2CSendByte(0xa1);
  I2CWaitAck();
  dat = I2CReceiveByte();
  I2CWaitAck();
  I2CStop();
  return dat;
}
void eeprom_write(uint8_t addr, uint8_t dat)
{
  I2CStart();
  I2CSendByte(0xa0);
  I2CWaitAck();
  I2CSendByte(addr);
  I2CWaitAck();
  I2CSendByte(dat);
  I2CWaitAck();
  I2CStop();
}

void eeprom_read_card()
{
  nfc_uid.len = eeprom_read(LEN_ADDR);
  if (nfc_uid.len > CARD_NUM)
  {
    nfc_uid.len = CARD_NUM;
  };
  msDelay(1);
  for (uint8_t i = 0; i < nfc_uid.len; i++)
  {
    for (uint8_t j = 0; j < UID_LEN; j++)
    {
      nfc_uid.uid[i][j] = eeprom_read(UID_ADDR + i * UID_LEN + j);
      msDelay(1);
    }
    for (uint8_t j = 0; j < NAME_LEN; j++)
    {
      nfc_uid.name[i][j] = eeprom_read(NAME_ADDR + i * NAME_LEN + j);
      msDelay(1);
    }
  }
}
void eeprom_write_card()
{
  if (nfc_uid.len > CARD_NUM)
  {
    nfc_uid.len = CARD_NUM;
  };
  eeprom_write(LEN_ADDR, nfc_uid.len);
  msDelay(1);
  for (uint8_t i = 0; i < nfc_uid.len; i++)
  {
    for (uint8_t j = 0; j < UID_LEN; j++)
    {
      eeprom_write(UID_ADDR + i * UID_LEN + j, nfc_uid.uid[i][j]);
      msDelay(1);
    }
    for (uint8_t j = 0; j < NAME_LEN; j++)
    {
      eeprom_write(NAME_ADDR + i * NAME_LEN + j, nfc_uid.name[i][j]);
      msDelay(1);
    }
  }
}
void eeprom_err_warning()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    BUZZER_ON;
    RED_ON;
    msDelay(100);
    BUZZER_OFF;
    RED_OFF;
    msDelay(100);
  }
}
e2rom_state eeprom_add_card(uint8_t *uid, uint8_t *name)
{
  nfc_uid.len++;
  if (nfc_uid.len > CARD_NUM)
  {
    nfc_uid.len = CARD_NUM;
    eeprom_err_warning();
    return E2ROM_MAX;
  }
  else
  {
    Buzzer_One(200);
    memcpy(nfc_uid.uid[nfc_uid.len - 1], uid, UID_LEN);
    memcpy(nfc_uid.name[nfc_uid.len - 1], name, NAME_LEN);
    msDelay(200);
    eeprom_write_card();
    Buzzer_One(200);
    return E2ROM_OK;
  }
}
void eeprom_delete_card_fast(uint8_t id)
{
  uint8_t *sp, *np;
  Buzzer_One(200);
  np = nfc_uid.uid[nfc_uid.len - 1];
  sp = nfc_uid.uid[id];
  memcpy(nfc_uid.uid[id], nfc_uid.uid[nfc_uid.len - 1], UID_LEN);
  memcpy(nfc_uid.name[id], nfc_uid.name[nfc_uid.len - 1], NAME_LEN);
  msDelay(200);
  nfc_uid.len--;
  eeprom_write_card();
  Buzzer_One(200);
}
void eeprom_delete_card(uint8_t *uid)
{
  if (nfc_uid.len == 0)
  {
    eeprom_err_warning();
    return;
  }
  else
  {
    uint8_t *sp;
    Buzzer_One(200);
    for (uint8_t i = 0; i < nfc_uid.len; i++)
    {
      if (strncmp((const char *)nfc_uid.uid[i], (const char *)uid, UID_LEN) == 0)
      {
        sp = nfc_uid.uid[i];
      }
    }
    memcpy(sp, nfc_uid.uid[nfc_uid.len - 1], UID_LEN);
    msDelay(200);
    eeprom_write_card();
    nfc_uid.len--;
    Buzzer_One(200);
  }
}
void eeprom_delete_all()
{
  Buzzer_One(200);
  memset(&nfc_uid, 0x00, sizeof(nfc_uid_t));
  msDelay(200);
  eeprom_write_card();
  Buzzer_One(200);
}
