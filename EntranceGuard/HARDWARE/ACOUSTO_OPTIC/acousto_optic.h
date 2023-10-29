#ifndef __ACOUSTO_OPTIC_H_
#define __ACOUSTO_OPTIC_H_
#include "main.h"
#define BUZZER_ON  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
#define BUZZER_OFF LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
#define RED_ON     LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
#define RED_OFF    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
#define GREEN_ON   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
#define GREEN_OFF  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
void Acousto_Optic_Init();
void Buzzer_One(uint16_t inv);
void Green_One(uint16_t inv);
void Red_One(uint16_t inv);
void Buzzer(uint16_t inv, uint8_t time);
void Green(uint16_t inv, uint8_t time);
void Red(uint16_t inv, uint8_t time);
#endif