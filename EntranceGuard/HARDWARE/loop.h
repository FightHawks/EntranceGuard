#ifndef __LOOP_H_
#define __LOOP_H_
#include "main.h"
#include "loop.h"
#include "eink.h"
#include "OLED.h"
#include "eeprom.h"
#include "nfc.h"
#include "acousto_optic.h"
#include "stdio.h"
#include "key.h"
#include "menu.h"
typedef struct __TASK_T
{
    uint8_t status;
    uint8_t key_time;
    uint8_t switch_time;
    GPIO_TypeDef *prot;
    uint32_t pin;
}task_t;
enum
{
    SETTING = 0,
    WORK,
};
extern task_t task;
void app_loop();
#endif
