#ifndef SI522A_INTERFACE_BUS_H__
#define SI522A_INTERFACE_BUS_H__

#define SLA_ADDR 0x50
#define SI522_CS_Enable() LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3)
#define SI522_CS_Disable() LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3)

#define SI522_Reset_Enable() LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define SI522_Reset_Disable() LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4)
void  I_SI522A_IO_Init(void);
void  I_SI522A_IO_Write(unsigned char RegAddr, unsigned char value);
unsigned char  I_SI522A_IO_Read(unsigned char RegAddr);

#endif
