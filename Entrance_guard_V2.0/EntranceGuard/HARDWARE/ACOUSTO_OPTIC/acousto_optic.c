
#include "acousto_optic.h"
void Acousto_Optic_Init()
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    /**/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_3 | LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4 | LL_GPIO_PIN_3 | LL_GPIO_PIN_0);
}

void Buzzer_One(uint16_t inv)
{
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
    msStop(inv);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
}
void Green_One(uint16_t inv)
{
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
    msStop(inv);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
}
void Red_One(uint16_t inv)
{
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
    msStop(inv);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
}
void Buzzer(uint16_t inv, uint8_t time)
{
    for (uint8_t i = 0; i < time; i++)
    {
        Buzzer_One(inv/2);
        msStop(inv/2);
    }
}
void Green(uint16_t inv, uint8_t time)
{
    for (uint8_t i = 0; i < time; i++)
    {
        Green_One(inv/2);
        msStop(inv/2);
    }
}
void Red(uint16_t inv, uint8_t time)
{
    for (uint8_t i = 0; i < time; i++)
    {
        Red_One(inv/2);
        msStop(inv/2);
    }
}