#include "key.h"
#include "stdio.h"
key_t key[KEY_NUM] = {0, 0, 0, 0, 0};
key_event_t current_key = {NULL_KEY, 0};

// uint32_t key_time = 0;
static void Key_GPIO_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    // PORT
    key[0].prot = KEY_PORT_0;
    key[1].prot = KEY_PORT_1;
    key[2].prot = KEY_PORT_2;
    key[3].prot = KEY_PORT_3;
    key[4].prot = KEY_PORT_4;

    // PIN
    key[0].pin = KEY_PIN_0;
    key[1].pin = KEY_PIN_1;
    key[2].pin = KEY_PIN_2;
    key[3].pin = KEY_PIN_3;
    key[4].pin = KEY_PIN_4;
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        GPIO_InitStruct.Pin = key[i].pin;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
        LL_GPIO_Init(key[i].prot, &GPIO_InitStruct);
    }
}

void Key_Init()
{
    Key_GPIO_Init();
    LL_TIM_EnableCounter(TIM2);
    LL_TIM_EnableIT_UPDATE(TIM2); // 使能更新中断
    
}

uint8_t GetKeyStatus(uint8_t KeyNo)
{
    return key[KeyNo].key_type;
}

uint8_t GetKeyStatusRE(uint8_t KeyNo)
{
    uint8_t key_type = key[KeyNo].key_type;
    if(key[KeyNo].is_callback == 0){
        key[KeyNo].is_callback = 1; 
        // key[KeyNo].last_type = key[KeyNo].key_type;
        // key[KeyNo].key_type = KEY_NONE_CLICK;
        return key_type;
    }
    return KEY_NONE_CLICK;
}

static void scan_key()
{
    for (int i = 0; i < KEY_NUM; i++)
    {
        switch (key[i].judge_sta)
        {
        case 0:
        {
            if (key[i].key_sta == KEY_DOWN_STATUS) // 按键按下
            {
                key[i].judge_sta = 1;
                key[i].key_time = 0;
            }

            break;
        }
        case 1: // 消抖过程
        {
            if (key[i].key_sta == KEY_DOWN_STATUS)
            {
                key[i].judge_sta = 2;
            }
            else
                key[i].judge_sta = 0; // 未按下

            break;
        }
        case 2:
        {
            if ((key[i].key_sta == KEY_UP_STATUS) && key[i].key_time < LONG_KEY_TIME &&(key[i].key_type != KEY_LONG_CLICK)) // 等待松开过程,且非长按键
            {
                key[i].last_type = key[i].key_type;
                key[i].key_type = KEY_SINGLE_CLICK; // 按键1单次按下
                key[i].is_callback = 0; 
          
                key[i].judge_sta = 0;               // 松开且是长按键
                current_key.Key_Name = i;
                current_key.Key_Type = KEY_SINGLE_CLICK;
            }
            else if (key[i].key_sta == KEY_UP_STATUS && key[i].key_time >= LONG_KEY_TIME){
                key[i].judge_sta = 0; // 松开且是长按键
                // key[i].key_type = KEY_NONE_CLICK;
            }
            else
            {
                if (key[i].key_time >= LONG_KEY_TIME){
                    key[i].key_type = KEY_LONG_CLICK; // 长按键
                    key[i].key_time = 0;
                    key[i].is_callback = 0; 
                    current_key.Key_Name = i;
                    current_key.Key_Type = KEY_LONG_CLICK;
                }
                key[i].key_time++;                    // 长按键计时 还没松开
            }
            break;
        }
        }
    }
}
void ScanKeyStatus()
{
    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        key[i].key_sta = LL_GPIO_IsInputPinSet(key[i].prot, key[i].pin);
    }
    scan_key();
}
void key_proc()
{
    static key_event_t key_event;
    for (int i = 0; i < KEY_NUM; i++)
    {
        key_event.Key_Type = GetKeyStatusRE(i);
        key_event.Key_Name = i;
        if (key_event.Key_Type != KEY_NONE_CLICK)
            Key_Event_Callback(&key_event);
    }
}

// 短按
__WEAK void key_up_single_click_callback()
{
    printf("up\r\n");
}
__WEAK void key_dowm_single_click_callback()
{
    printf("down\r\n");
}
__WEAK void key_left_single_click_callback()
{
    printf("left\r\n");
}
__WEAK void key_right_single_click_callback()
{
    printf("right\r\n");
}
__WEAK void key_in_single_click_callback()
{
    printf("in\r\n");
}

// 长按
__WEAK void key_up_long_click_callback()
{
}
__WEAK void key_dowm_long_click_callback()
{
}
__WEAK void key_left_long_click_callback()
{
}
__WEAK void key_right_long_click_callback()
{
}
__WEAK void key_in_long_click_callback()
{
    printf("long_in\r\n");
}

__WEAK void Key_Event_Callback(key_event_t *event)
{
    switch (event->Key_Name)
    {
    case KEY_UP:
    {
        if (event->Key_Type == KEY_SINGLE_CLICK)
        {
            key_up_single_click_callback();
        }
        else if (event->Key_Type == KEY_LONG_CLICK)
        {
            key_up_long_click_callback();
        }
    }
    break;
    case KEY_DOWM:
    {
        if (event->Key_Type == KEY_SINGLE_CLICK)
        {
            key_dowm_single_click_callback();
        }
        else if (event->Key_Type == KEY_LONG_CLICK)
        {
            key_dowm_long_click_callback();
        }
    }
    break;
    case KEY_LEFT:
    {
        if (event->Key_Type == KEY_SINGLE_CLICK)
        {
            key_left_single_click_callback();
        }
        else if (event->Key_Type == KEY_LONG_CLICK)
        {
            key_left_long_click_callback();
        }
    }
    break;
    case KEY_RIGHT:
    {
        if (event->Key_Type == KEY_SINGLE_CLICK)
        {
            key_right_single_click_callback();
        }
        else if (event->Key_Type == KEY_LONG_CLICK)
        {
            key_right_long_click_callback();
        }
    }
    break;
    case KEY_IN:
    {
        if (event->Key_Type == KEY_SINGLE_CLICK)
        {
            key_in_single_click_callback();
        }
        else if (event->Key_Type == KEY_LONG_CLICK)
        {
            key_in_long_click_callback();
        }
    }
    break;

        break;
    default:
        break;
    }
}
// 任务相关按键回调
