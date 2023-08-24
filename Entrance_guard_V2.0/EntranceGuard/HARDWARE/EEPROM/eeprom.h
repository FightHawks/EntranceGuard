


 #ifndef __EEPROM_H_
#define __EEPROM_H_

#include "main.h"
#define CARD_NUM 10
#define UID_LEN 4
#define LEN_ADDR 0X00
#define DATA_ADDR 0X01
typedef struct nfc_uid
{
    uint8_t len;
    uint8_t uid[CARD_NUM][UID_LEN];
}nfc_uid_t;
typedef enum 
{
    E2ROM_OK = 0,
    E2ROM_ERR,
    E2ROM_MAX,
}e2rom_state;
extern nfc_uid_t nfc_uid;
void I2CStart(void);
void I2CStop(void);
unsigned char I2CWaitAck(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);
void I2CInit(void);

uint8_t eeprom_read(uint8_t addr);
void eeprom_write (uint8_t addr,uint8_t dat);

void eeprom_read_card();
e2rom_state eeprom_add_card(uint8_t *uid);
void eeprom_delete_card_fast(uint8_t id);
void eeprom_delete_all();
void eeprom_delete_card(uint8_t *uid);
#endif


 
