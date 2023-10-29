#include "loop.h"
task_t task = {
    .key_time = 0,
    .prot = GPIOA,
    .pin = LL_GPIO_PIN_15,
    .switch_time = 6,
    .status = SETTING,
};

void SystemClock_Config16(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
    {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    while (LL_PWR_IsActiveFlag_VOS() != 0)
    {
    }
    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while (LL_RCC_HSI_IsReady() != 1)
    {
    }
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_LSI_Enable();

    /* Wait till LSI is ready */
    while (LL_RCC_LSI_IsReady() != 1)
    {
    }
    LL_PWR_EnableBkUpAccess();
    if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    }
    LL_RCC_EnableRTC();
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_4, LL_RCC_PLL_DIV_4);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1)
    {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    LL_Init1msTick(16000000);

    LL_SetSystemCoreClock(16000000);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_HSI);
}
void show_card()
{
    char cStr[30];
    for (uint8_t i = 0; i < nfc_uid.len; i++)
    {
        sprintf(cStr, "Name is:%c%c%c ID is: %02X%02X%02X%02X", nfc_uid.name[i][0], nfc_uid.name[i][1], nfc_uid.name[i][2], nfc_uid.uid[i][0], nfc_uid.uid[i][1], nfc_uid.uid[i][2], nfc_uid.uid[i][3]);
        printf("%s\r\n", cStr); // 打印卡片ID
    }
}

void bsp_init()
{
    msDelay(200);
    if (task.status == SETTING)
    {
        SystemClock_Config16();
    }
    else if (task.status == WORK)
    {
        SystemClock_Config();
    }
    OLED_Init();
    RC522_Init();
    PcdReset();
    /*设置工作方式*/
    M500PcdConfigISOType('A');
    PcdHalt();
    Key_Init();
    Acousto_Optic_Init();
    OLED_Fill_Fast(OLED_WHITE);
    OLED_Fill_Fast(OLED_BLACK);
    // printf("init finish\r\n");
    eeprom_read_card();
    // show_card();
    // eeprom_delete_all();
    // Buzzer_One(300);
}
void bsp_deinit()
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_ALL);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_ALL);
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_ALL);
    LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    /**/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI1);
    LL_SPI_Disable(SPI1);
}
void bsp_reinit()
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    LL_SPI_Enable(SPI1);
    Acousto_Optic_Init();
    if (task.status == SETTING)
    {
        OLED_Init();
    }
    else if (task.status == WORK)
    {
        SystemClock_Config();
    }
}
void sleep(uint16_t xms)
{
    // /* 开启PWR时钟 */
    // LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    // /* 设置中断线 */
    // LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);
    // LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
    // LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);

    // /* 设置RTC唤醒时间 */
    // LL_RTC_DisableWriteProtection(RTC);
    // LL_RTC_WAKEUP_Disable(RTC);
    // while (!LL_RTC_IsActiveFlag_WUTW(RTC))
    //     ;
    // LL_RTC_WAKEUP_SetAutoReload(RTC, xms * 9);
    // LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_4);
    // LL_RTC_EnableIT_WUT(RTC);
    // LL_RTC_WAKEUP_Enable(RTC);
    // LL_RTC_EnableWriteProtection(RTC);
    // // power_off();
    // // LL_SYSTICK_DisableIT();
    // // LL_RCC_HSI_Disable();
    // LL_PWR_EnableUltraLowPower();
    // LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
    // //
    // LL_LPM_EnableDeepSleep();
    // __WFI();
    // LL_RTC_DisableIT_WUT(RTC);
    // LL_RTC_WAKEUP_Disable(RTC);
    // LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_20);
    // LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_20);
    // // power_on();

    LL_RCC_EnableRTC();

    /* 失能RTC写保护 */
    LL_RTC_DisableWriteProtection(RTC);

    /* 设置预分频 */
    //   LL_RTC_SetAsynchPrescaler(RTC, 0x7F);
    //   LL_RTC_SetSynchPrescaler(RTC, 0xFF);

    /* 修改重装载值时需要先禁止唤醒定时器 */
    LL_RTC_WAKEUP_Disable(RTC);

    /* 等待WUTWF置1 */
    while (LL_RTC_IsActiveFlag_WUTW(RTC) != 1)
        ;
    /* 设置重装载值 */
    LL_RTC_WAKEUP_SetAutoReload(RTC, 9 * xms);
    /* 选择唤醒时钟 */
    LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_4);
    /* 使能内部唤醒线 */
    //   LL_PWR_EnableInternWU();
    /* 配置唤醒中断线20 */
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);
    /* 配置唤醒中断优先级 */
    NVIC_SetPriority(RTC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(RTC_IRQn);
    /* 清除唤醒标志 */
    LL_RTC_ClearFlag_WUT(RTC);
    /* 清除中断 */
    LL_RTC_EnableIT_WUT(RTC);
    LL_RTC_WAKEUP_Enable(RTC);
    LL_SYSTICK_DisableIT();
    // // NVIC_EnableIRQ()
    LL_TIM_DisableCounter(TIM2);
    LL_TIM_DisableIT_UPDATE(TIM2); // 使能更新中断
    /* 使能写保护 */
    LL_RTC_EnableWriteProtection(RTC);

    LL_PWR_EnableUltraLowPower();
    LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);
    LL_LPM_EnableDeepSleep();
    __WFI();
    LL_RTC_ClearFlag_WUT(RTC);
    NVIC_DisableIRQ(RTC_IRQn);
    LL_SYSTICK_EnableIT();
    // // NVIC_EnableIRQ()
    LL_TIM_EnableCounter(TIM2);
    LL_TIM_EnableIT_UPDATE(TIM2); // 使能更新中断
}
// void stop(uint16_t xms)
// {
//     /* 开启PWR时钟 */
//     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
//     /* 设置中断线 */
//     LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);
//     LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
//     LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);

//     /* 设置RTC唤醒时间 */
//     LL_RTC_DisableWriteProtection(RTC);
//     LL_RTC_WAKEUP_Disable(RTC);
//     while (!LL_RTC_IsActiveFlag_WUTW(RTC))
//         ;
//     LL_RTC_WAKEUP_SetAutoReload(RTC, xms * 9);
//     LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_4);
//     LL_RTC_EnableIT_WUT(RTC);
//     LL_RTC_WAKEUP_Enable(RTC);
//     LL_RTC_EnableWriteProtection(RTC);
//     // power_off();
//     LL_PWR_EnableUltraLowPower();
//     LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
//     LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);
//     LL_LPM_EnableDeepSleep();
//     __WFI();
//     SystemClock_Config();
//     // LL_RTC_DisableIT_WUT(RTC);
//     // LL_RTC_WAKEUP_Disable(RTC);
//     // LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_20);
//     // LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_20);
//     // MX_TIM21_Init();
//     // power_on();

//     //   LL_RCC_EnableRTC();

//     //   /* 失能RTC写保护 */
//     //   LL_RTC_DisableWriteProtection(RTC);

//     //   /* 设置预分频 */
//     //   LL_RTC_SetAsynchPrescaler(RTC, 0x7F);
//     //   LL_RTC_SetSynchPrescaler(RTC, 0xFF);

//     //   /* 修改重装载值时需要先禁止唤醒定时器 */
//     //   LL_RTC_WAKEUP_Disable(RTC);

//     //   /* 等待WUTWF置1 */
//     //   while (LL_RTC_IsActiveFlag_WUTW(RTC) != 1);
//     //   /* 设置重装载值 */
//     //   LL_RTC_WAKEUP_SetAutoReload(RTC, 5);
//     //   /* 选择唤醒时钟 */
//     //   LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);
//     //   /* 使能内部唤醒线 */
//     // //   LL_PWR_EnableInternWU();
//     //   /* 配置唤醒中断线20 */
//     //   LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
//     //   LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);
//     //   /* 配置唤醒中断优先级 */
//     //   NVIC_SetPriority(RTC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
//     //   NVIC_EnableIRQ(RTC_IRQn);
//     //   /* 清除唤醒标志 */
//     //   LL_RTC_ClearFlag_WUT(RTC);
//     //   /* 清除中断 */
//     //   LL_RTC_EnableIT_WUT(RTC);
//     //   LL_RTC_WAKEUP_Enable(RTC);

//     //   /* 使能写保护 */
//     //   LL_RTC_EnableWriteProtection(RTC);
// }
void stop(uint16_t xms)
{
    /* 开启PWR时钟 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    // LL_TIM_DisableCounter(TIM2);
    // LL_TIM_DisableIT_UPDATE(TIM2); // 使能更新中断
    /* 设置中断线 */
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);
    LL_RTC_ClearFlag_WUT(RTC);
    /* 设置RTC唤醒时间 */
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_WAKEUP_Disable(RTC);
    while (!LL_RTC_IsActiveFlag_WUTW(RTC))
        ;
    LL_RTC_WAKEUP_SetAutoReload(RTC, xms * 9);
    LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_4);
    NVIC_EnableIRQ(RTC_IRQn);
    LL_RTC_EnableIT_WUT(RTC);
    LL_RTC_WAKEUP_Enable(RTC);
    LL_RTC_EnableWriteProtection(RTC);
    // power_off();
    // LL_SYSTICK_DisableIT();
    // LL_RCC_HSI_Disable();
    LL_PWR_EnableUltraLowPower();
    LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
    //
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);
    LL_LPM_EnableDeepSleep();
    // bsp_deinit();
    __WFI();
    LL_RTC_ClearFlag_WUT(RTC);
    if (task.status == SETTING)
    {
        SystemClock_Config16();
    }
    else if (task.status == WORK)
    {
        SystemClock_Config();
    }
    // bsp_reinit();
    MX_TIM21_Init();
    NVIC_DisableIRQ(RTC_IRQn);
    // LL_RTC_DisableIT_WUT(RTC);
    // LL_RTC_WAKEUP_Disable(RTC);
    // LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_20);
    // LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_20);
    // LL_TIM_EnableCounter(TIM2);
    // LL_TIM_EnableIT_UPDATE(TIM2); // 使能更新中断

    // power_on();
}

void app_loop()
{

    bsp_init();
    Menu_Init();

    for (;;)
    {
        // stop(100);
        // sleep(1);
        // GUI_Refresh();
        // Popup_Callback();
        // Match_Card();
        switch (task.status)
        {
        case SETTING:
            GUI_Refresh();
            Popup_Callback();
            if (func_index == _OLED_Lock)
            {
                task.status = WORK;
                SystemClock_Config();
                Buzzer(200, 2);
                // BUZZER_ON;
                // msStop(500);
                // BUZZER_OFF;
                // msStop(50000);
            }

            // key_proc();
            break;
        case WORK:
            
            if (LL_GPIO_IsInputPinSet(task.prot, task.pin) == KEY_DOWN_STATUS)
            {
                task.key_time++;
                if (task.key_time >= task.switch_time)
                {
                    task.status = SETTING;
                    task.key_time = 0;
                    Buzzer(200, 4);
                    SystemClock_Config16();
                    ToMain_UI();
                    break;
                }
            }
            Match_Card();
            GREEN_ON;
            msStop(500);
            GREEN_OFF;
            msStop(500);
            break;
        default:
            break;
        }

        // Menu_Show(current_key.Key_Name);
        // Add_Card();
        // if()

        // msSleep(500);
        // Green(1000, 2);
        // Red(500, 2);
    }
}
