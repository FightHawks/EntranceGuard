#include "loop.h"

void show_card()
{
    char cStr[30];
    for (uint8_t i = 0; i < nfc_uid.len; i++)
    {
        sprintf(cStr, "Name is:%c%c%c ID is: %02X%02X%02X%02X",  nfc_uid.name[i][0], nfc_uid.name[i][1], nfc_uid.name[i][2],nfc_uid.uid[i][0], nfc_uid.uid[i][1], nfc_uid.uid[i][2], nfc_uid.uid[i][3]);
        printf("%s\r\n", cStr); // 打印卡片ID
    }
}

void bsp_init()
{
    msDelay(200);
    OLED_Init();
    RC522_Init();
    PcdReset();
    /*设置工作方式*/
    M500PcdConfigISOType('A');
    PcdHalt();
    Key_Init();
    OLED_Fill_Fast(OLED_WHITE);
    OLED_Fill_Fast(OLED_BLACK);
    printf("init finish\r\n");
    eeprom_read_card();
    show_card();
    // eeprom_delete_all();
    // Buzzer_One(300);
}
void stop(uint16_t xms)
{
    /* 开启PWR时钟 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* 设置中断线 */
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);

    /* 设置RTC唤醒时间 */
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_WAKEUP_Disable(RTC);
    while (!LL_RTC_IsActiveFlag_WUTW(RTC))
        ;
    LL_RTC_WAKEUP_SetAutoReload(RTC, xms * 9);
    LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_4);
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
    __WFI();
    // power_on();
}

void app_loop()
{

    bsp_init();
    Menu_Init();

    for (;;)
    {
        GUI_Refresh();
        Popup_Callback();
        key_proc();
        // Menu_Show(current_key.Key_Name);
        // Add_Card();
        // if()
        Match_Card();
        // msSleep(500);
        // Green(1000, 2);
        // Red(500, 2);
    }
}
