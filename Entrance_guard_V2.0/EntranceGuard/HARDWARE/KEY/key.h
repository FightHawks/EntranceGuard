
#ifndef __KEY_H_
#define __KEY_H_
#include "main.h"
#include <stdbool.h>
#define KEY_NUM 5
#define KEY_DOWN_STATUS 0
#define KEY_UP_STATUS 1

#define SINGAL_KEY_TIME 80
#define LONG_KEY_TIME 70
#define DOUBLE_KEY_TIME 10
// 上
#define KEY_PORT_0 GPIOA
#define KEY_PIN_0 LL_GPIO_PIN_11
// 下
#define KEY_PORT_1 GPIOA
#define KEY_PIN_1 LL_GPIO_PIN_12
// 左
#define KEY_PORT_2 GPIOA
#define KEY_PIN_2 LL_GPIO_PIN_10
// 右
#define KEY_PORT_3 GPIOA
#define KEY_PIN_3 LL_GPIO_PIN_8
// 确定
#define KEY_PORT_4 GPIOA
#define KEY_PIN_4 LL_GPIO_PIN_15

enum
{
  KEY_NONE_CLICK = 0,
  KEY_SINGLE_CLICK,
  KEY_LONG_CLICK,
};
enum
{
  KEY_UP = 0,
  KEY_DOWM,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_IN,
  NULL_KEY,
};

typedef struct
{
  uint8_t Key_Name;
  uint8_t Key_Type;
} key_event_t;
typedef struct
{
  bool key_sta;
  GPIO_TypeDef *prot;
  uint32_t pin;
  uint8_t judge_sta;
  uint8_t key_type;
  uint32_t key_time;
  uint8_t last_key;
  uint8_t last_type;
  uint8_t is_callback;
} key_t;

extern key_event_t current_key;


void ScanKeyStatus();
void Key_Init();
uint8_t GetKeyStatus(uint8_t KeyNo);
void key_proc();
// callback
void Key_Event_Callback(key_event_t *event);
// single_click
void key_up_single_click_callback();
void key_dowm_single_click_callback();
void key_left_single_click_callback();
void key_right_single_click_callback();
void key_in_single_click_callback();
// long_click
void key_up_long_click_callback();
void key_dowm_long_click_callback();
void key_left_long_click_callback();
void key_right_long_click_callback();
void key_in_long_click_callback();

#endif
