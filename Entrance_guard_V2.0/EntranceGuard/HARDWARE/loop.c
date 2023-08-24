#include "loop.h"
#include "eink.h"
#include "OLED.h"
#include "eeprom.h"
#include "nfc.h"
#include "acousto_optic.h"
#include "stdio.h"
#include "key.h"
#include "menu.h"
uint8_t uid_buff[4];

void (*current_operation_index)();
unsigned char func_index = 0; //�������ʱ���ڳ��������ֵ
unsigned char last_index = 127;
unsigned char last_sec = 60;

void show_card()
{
    char cStr[30];
    for (uint8_t i = 0; i < nfc_uid.len; i++)
    {
        sprintf(cStr, "The Card ID is: %02X%02X%02X%02X", nfc_uid.uid[i][0], nfc_uid.uid[i][1], nfc_uid.uid[i][2], nfc_uid.uid[i][3]);
        printf("%s\r\n", cStr); // ��ӡ��ƬID
    }
}

void bsp_init()
{
    OLED_Init();
    // RC522_Init();
    // PcdReset();
    // /*���ù�����ʽ*/
    // M500PcdConfigISOType('A');
    // PcdHalt();
    Key_Init();
    OLED_Fill_Fast(OLED_WHITE);
    OLED_Fill_Fast(OLED_BACK);
    printf("init finish\r\n");
    // eeprom_read_card();
    // show_card();
    // eeprom_delete_all();
    // Buzzer_One(300);
}
void stop(uint16_t xms)
{
    /* ����PWRʱ�� */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* �����ж��� */
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);

    /* ����RTC����ʱ�� */
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
    Watch_Show();

    for (;;)
    {
        // key_proc();
        Menu_Show(current_key.Key_Name);
        // Add_Card();
        // Match_Card();
        // msSleep(500);
        // Green(1000, 2);
        // Red(500, 2);
    }
}
uint8_t NFC_Search_Card_Once(uint8_t *ucUID)
{
    uint8_t ucArray_ID[4];  /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/
    uint8_t ucStatusReturn; /*����״̬*/
    /*Ѱ��*/
    if ((ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID)) != MI_OK)
    {
        ucStatusReturn = PcdRequest(PICC_REQALL, ucArray_ID); // PICC_REQALL   PICC_REQIDL
    }
    if (ucStatusReturn == MI_OK)
    {

        /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
        if (PcdAnticoll(ucArray_ID) == MI_OK)
        {
            PcdSelect(ucArray_ID);
            memcpy(ucUID, ucArray_ID, 4);
            // sprintf(cStr, "The Card ID is: %02X%02X%02X%02X", ucArray_ID[0], ucArray_ID[1], ucArray_ID[2], ucArray_ID[3]);
            // printf("%s\r\n", cStr); // ��ӡ��ƬID
            PcdHalt();
            // Buzzer(500, 2);
        }
    }
    return ucStatusReturn;
}
void Add_Card()
{
    PcdAntennaOn(); // ��������
    if (NFC_Search_Card_Once(uid_buff) == MI_OK)
    {

        if (nfc_uid.len == 0)
        {
            Buzzer_One(100);
            eeprom_add_card(uid_buff);
            Buzzer_One(100);
        }

        for (uint8_t i = 0; i < nfc_uid.len; i++)
        {
            if (strncmp((const char *)uid_buff, (const char *)nfc_uid.uid[i], 4) == 0)
            {
                Buzzer(50, 5);
                break;
            }
            else if (i == nfc_uid.len - 1)
            {
                Buzzer_One(100);
                eeprom_add_card(uid_buff);
                Buzzer_One(100);
            }
        }
        memset(uid_buff, 0x00, 4);

        // Buzzer(500, 2);
    }
    PcdAntennaOff(); // �ر�����
}

void Match_Card()
{
    PcdAntennaOn(); // ��������
    if (NFC_Search_Card_Once(uid_buff) == MI_OK)
    {
        if (nfc_uid.len == 0)
        {
            Red_One(500);
            return;
        }
        for (uint8_t i = 0; i < nfc_uid.len; i++)
        {
            if (strncmp((const char *)uid_buff, (const char *)nfc_uid.uid[i], 4) == 0)
            {
                LL_TIM_CC_EnableChannel(TIM21, LL_TIM_CHANNEL_CH1);
                LL_TIM_EnableCounter(TIM21);
                LL_TIM_OC_SetCompareCH1(TIM21, 2500 - 1);
                GREEN_ON;
                // LL_TIM_OC_SetCompareCH1(TIM21, 0);
                msDelay(2000);
                LL_TIM_OC_SetCompareCH1(TIM21, 500 - 1);
                msDelay(2000);
                // LL_TIM_OC_SetCompareCH1(TIM21, 0);
                LL_TIM_CC_DisableChannel(TIM21, LL_TIM_CHANNEL_CH1);
                LL_TIM_DisableCounter(TIM21);
                GREEN_OFF;
                break;
            }
            else if (i == nfc_uid.len - 1)
            {
                Red_One(500);
            }
        }
    }
    memset(uid_buff, 0x00, 4);
    PcdAntennaOff(); // �ر�����
}