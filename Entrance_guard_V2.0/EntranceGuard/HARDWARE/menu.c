#include "menu.h"
#include "main.h"
#include "OLED.h"
#include "key.h"
#include "stdio.h"
static u8 func_index=_Main_UI;//当前页面索引值
static u8 menu_last_index=_Main_UI;//上一个界面索引值
static void (*current_operation_func)(u8*,u8*);//定义一个函数指针
static menuList_t * menuList_Table;
static popup_sta_t Popup_Sta = {0, 0, 0};

//索引表
mainMenu_t MainMenu_Table[20]=
{
	//Cur_Index,          previous,         next,              enter,              back,             (*current_operation)(u8,u8)
	//主界面     
	{_Main_UI,           _Main_UI,         _Main_UI,           _Card_Icron,       _OLED_Lock,        Main_UI},
    {_Main_Option,       _Main_Option,     _Main_Option,       _Card_Icron,       _Main_Option,      Main_Option_Func},
    //
    {_Admin_Login,       _Admin_Login,     _Admin_Login,       _Admin_Login,       _Admin_Login,     Admin_Login_Func},
	//主菜单     
    {_Card_Icron,        _Info_Icron,      _Setting_Icron,     _Card_Option,       _Main_UI,         Main_Option_Func},
    {_Setting_Icron,     _Card_Icron,      _Info_Icron,        _Setting_Option,    _Main_UI,         Main_Option_Func},
    {_Info_Icron,        _Setting_Icron,  _Card_Icron,         _Info_Option,       _Main_UI,         Main_Option_Func},

	{_Card_Option,       _Card_Option,     _Card_Option,        _Card_Option,      _Card_Icron,      Card_Option_Func},
	{_Setting_Option,    _Setting_Option,  _Setting_Option,     _Setting_Option,   _Setting_Icron,   Setting_Option_Func},
	{_Info_Option,       _Info_Option,     _Info_Option,        _Info_Option,      _Info_Icron,      Info_Option_Func},
	//子菜单
	{_Add_Card,          _Add_Card,        _Add_Card,          _Add_Card,          _Card_Option,     Add_Card_Func},
    {_Change_Card,       _Change_Card,      _Change_Card,      _Change_Card,       _Card_Option,     Change_Card_Func},
    {_Delete_Card,       _Delete_Card,     _Delete_Card,       _Delete_Card,       _Card_Option,     Delete_Card_Func},
    
	{_Date_Setting,      _Date_Setting,    _Date_Setting,      _Date_Setting,      _Setting_Option,  Date_Setting_Func},
    {_Time_Setting,      _Time_Setting,    _Time_Setting,      _Time_Setting,      _Setting_Option,  Time_Setting_Func},

	{_Device_Info,       _Device_Info,     _Device_Info,       _Device_Info,       _Info_Option,     Device_Info_Func},
    {_Devoloper_Info,    _Devoloper_Info,  _Devoloper_Info,    _Devoloper_Info,    _Info_Option,     Devoloper_Info_Func},

	{_OLED_Lock,         _Main_UI,         _Main_UI,           _Main_UI,           _Main_UI,         OLED_Lock_Func},

    {_List_Menu,         _List_Menu,       _List_Menu,         _List_Menu,         _List_Menu,       List_Menu_Func},
    {_Popup,             _Popup,           _Popup,             _Popup,             _Popup,           Popup_Func},
};

void Menu_Init(void)
{
    current_operation_func=MainMenu_Table[func_index].current_operation;
	(*current_operation_func)(&func_index,&current_key.Key_Name);
}
/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(void)
{
    if(current_key.is_GuiCallback != 1 && func_index == _Popup){
        current_key.is_PopupCallback = 0;
        current_key.is_GuiCallback = 1;
        return;
    }
	if(current_key.is_GuiCallback != 1)
	{
		menu_last_index=func_index;//更新上一界面索引值
        if(func_index == _Card_Icron || func_index == _Setting_Icron || func_index == _Info_Icron)
        {
            switch(current_key.Key_Name)
            {
                case KEY_IN://(current_key.Key_Type==KEY_SINGLE_CLICK)?func_index=MainMenu_Table[func_index].enter:func_index=MainMenu_Table[func_index].back;
                    if(current_key.Key_Type==KEY_SINGLE_CLICK) func_index=MainMenu_Table[func_index].enter;
                    else if(current_key.Key_Type==KEY_LONG_CLICK) func_index=MainMenu_Table[func_index].back;
                        break;
                case KEY_RIGHT:func_index=MainMenu_Table[func_index].next;//更新索引值
                        break;
                case KEY_LEFT:func_index=MainMenu_Table[func_index].previous;//更新索引值
                        break;
                case KEY_DOWN:func_index=MainMenu_Table[func_index].next;//更新索引值
                        break;
                case KEY_UP:func_index=MainMenu_Table[func_index].previous;//更新索引值
                        break;
                default:break;
            }
        }
        else{// if(func_index != _Card_Icron && func_index != _Setting_Icron && func_index != _Info_Icron){
            switch(current_key.Key_Name)
            {
                case KEY_IN://(current_key.Key_Type==KEY_SINGLE_CLICK)?func_index=MainMenu_Table[func_index].enter:func_index=MainMenu_Table[func_index].back;
                    if(current_key.Key_Type==KEY_SINGLE_CLICK) func_index=MainMenu_Table[func_index].enter;
                    else if(current_key.Key_Type==KEY_LONG_CLICK) func_index=MainMenu_Table[func_index].back;
                        break;
                case KEY_DOWN:func_index=MainMenu_Table[func_index].next;//更新索引值
                        break;
                case KEY_UP:func_index=MainMenu_Table[func_index].previous;//更新索引值
                        break;
                default:break;
            }
        }
		
        // current_key.Key_Name = NULL_KEY;
        // current_key.Key_Type = KEY_NONE_CLICK;
		// OLED_Fill_Fast(OLED_BLACK);
        current_operation_func=MainMenu_Table[func_index].current_operation;
	    (*current_operation_func)(&func_index,&current_key.Key_Name);

        current_key.is_GuiCallback = 1;
	}
}

void Show_Scrollbar(uint8_t len, uint8_t cur_pos, uint8_t posi)
{
    switch (posi)
    {
    case _Top:
        /* code */
        break;
    case _Left:
        
        break;
    case _Right:
        /* code */
        break;
    case _Button:
        /* code */
        break;
    default:
        break;
    }
}

void Creat_Popup(uint8_t previous, uint8_t next, uint8_t enter, uint8_t back,void (*current_operation)(u8 *, u8 *), uint16_t last_time)
{
    Popup_Sta.is_exist = 1;
    Popup_Sta.last_time = last_time;

    MainMenu_Table[_Popup].previous = previous;
    MainMenu_Table[_Popup].next = next;
    MainMenu_Table[_Popup].enter = enter;
    MainMenu_Table[_Popup].back = back;
    MainMenu_Table[_Popup].current_operation = current_operation;
    
    menu_last_index = func_index;
    func_index = _Popup;
    // current_operation_func=MainMenu_Table[func_index].current_operation;
    (*current_operation)(&func_index,&current_key.Key_Name);
}
void Popup_Thread(void)
{
    if(Popup_Sta.is_exist) Popup_Sta.callback_time ++;
    else{
        Popup_Sta.last_time = 0;
        Popup_Sta.is_exist = 0;
        Popup_Sta.callback_time = 0;
    }
}
void Popup_Callback(void)
{
    if(Popup_Sta.is_exist && Popup_Sta.callback_time){
        if((Popup_Sta.last_time) > Popup_Sta.callback_time*CALLBACK_INTERVAL)
        {
            Popup_Sta.last_time -= Popup_Sta.callback_time*CALLBACK_INTERVAL;
            Popup_Sta.callback_time = 0;

            menu_last_index = func_index;
            current_operation_func=MainMenu_Table[_Popup].current_operation;
            (*current_operation_func)(&func_index,&current_key.Key_Name);
        }
        else{
            Popup_Sta.last_time = 0;
            Popup_Sta.is_exist = 0;
            Popup_Sta.callback_time = 0;

            menu_last_index = func_index;
            func_index = MainMenu_Table[func_index].previous;
            current_operation_func=MainMenu_Table[func_index].current_operation;
            (*current_operation_func)(&func_index,&current_key.Key_Name);

            return;
        }
    }
    else return;
}
void Show_List(menuList_t *menuList_Table, uint8_t cur_pos, uint8_t max_item, uint8_t len)
{
    if(cur_pos <= max_item)
    {
        for(u8 i = 0; i < MIN(nfc_uid.len, max_item); i++)
        {
            if(cur_pos == i+1) OLED_ShowStr(0,2*i+1,(unsigned char *)menuList_Table[i].str,SIZE_0806, REVERSE);
            else OLED_ShowStr(0,2*i+1,(unsigned char *)menuList_Table[i].str,SIZE_0806, NORMAL);
        }
    }
    else{
        u8 tem = max_item;
        for(u8 i = tem; i > 0; i--)
        {
            if(tem-i == max_item-1) OLED_ShowStr(0,2*(tem-i)+1,(unsigned char *)menuList_Table[cur_pos-i].str,SIZE_0806, REVERSE);
            else OLED_ShowStr(0,2*(tem-i)+1,(unsigned char *)menuList_Table[cur_pos-i].str,SIZE_0806, NORMAL);
        }
    }
}
// 界面函数
void Main_UI(u8* page_index,u8* key_val)
{
    OLED_Clear();
    if(menu_last_index == _OLED_Lock)   OLED_ON();
    OLED_ShowStr(32+0,0,(unsigned char *)"Main_UI",SIZE_0806, 1);
    OLED_ShowStr(32+0,2,(unsigned char *)"0000-00-00",SIZE_0806, 1);
    OLED_ShowStr(32+0,4,(unsigned char *)"00:00:00",SIZE_0806, 1);
}
void Main_Option_Func(u8* page_index,u8* key_val)
{
    if(menu_last_index != _Card_Icron && menu_last_index != _Setting_Icron && menu_last_index != _Info_Icron) OLED_Clear();
    OLED_ShowStr(6    ,3,(unsigned char *)"<",SIZE_0806, NORMAL);
    OLED_ShowStr(121-6,3,(unsigned char *)">",SIZE_0806, NORMAL);
	// OLED_Display_Image(0,0,11,7,(char*)Signal_11x7,NO_ALGIN);//显示信号塔
	// OLED_Display_Image(14,0,9,9,(char*)Bluetooth_9x9,NO_ALGIN);//显示蓝牙
	// OLED_Display_Image(112,0,16,8,(char*)Bat_16x8,NO_ALGIN);//显示电量
	switch(*page_index)
	{
		case _Card_Icron:
                        OLED_ShowStr(128/2-33,3,(unsigned char *)"<",SIZE_0806, NORMAL);
                        OLED_ShowStr(128/2-33,3,(unsigned char *)"card_icron",SIZE_0806, NORMAL);
                        OLED_ShowStr(128/2-21,6,(unsigned char *)"o  -  -",SIZE_0806, NORMAL);
                        break;
		case _Setting_Icron:
                        OLED_ShowStr(128/2-33,3,(unsigned char *)"          ",SIZE_0806, NORMAL);
                        OLED_ShowStr(128/2-33,3,(unsigned char *)"set_icron",SIZE_0806, NORMAL);
                        OLED_ShowStr(128/2-21,6,(unsigned char *)"-  o  -",SIZE_0806, NORMAL);
                        break;
		case _Info_Icron:
                        OLED_ShowStr(128/2-33,3,(unsigned char *)"          ",SIZE_0806, NORMAL);
                        OLED_ShowStr(128/2-33,3,(unsigned char *)"info_icron",SIZE_0806, NORMAL);
                        OLED_ShowStr(128/2-21,6,(unsigned char *)"-  -  o",SIZE_0806, NORMAL);
                        break;
		default:break;
	}
}
void Admin_Login_Func(u8* page_index,u8* key_val)
{

}
void Card_Option_Func(u8* page_index,u8* key_val)
{
	static u8 cur_pos=1;
    bool head2tail = 1;
    uint8_t option_num = 3;
    
    NFC_Event.Add_Card = OFF;
    OLED_Clear();
	if(menu_last_index ==_Card_Option)//判断是否是第一次进入此界面
	{
        // if(current_key.is_callback == 0){
            
            switch(*key_val)
            {
                case KEY_UP: if(head2tail)cur_pos==1?cur_pos=option_num:cur_pos--;
                            else cur_pos==1?cur_pos=1:cur_pos--;
                        break;
                case KEY_IN://确定(设置)按键
                {
                    switch(cur_pos)
                    {
                        case 1:
                                menu_last_index = *page_index;
                                *page_index = _Add_Card;
                                current_operation_func=MainMenu_Table[func_index].current_operation;
                                (*current_operation_func)(&func_index,&current_key.Key_Name);
                                break;
                        case 2:
                                Creat_Popup(_Card_Option, _Popup, _Popup, _Popup, Developing_Popup, 1000);
                                break;
                        case 3:
                                menu_last_index = *page_index;
                                *page_index = _Delete_Card;
                                current_operation_func=MainMenu_Table[func_index].current_operation;
                                (*current_operation_func)(&func_index,&current_key.Key_Name);
                                break;
                        default:break;
                    }
                }
                        break;
                case KEY_DOWN:if(head2tail)cur_pos==option_num?cur_pos=1:cur_pos++;
                            else cur_pos==option_num?cur_pos=option_num:cur_pos++;
                        break;
                default:break;
            }
        // }
	}
	// else cur_pos=1;//第一次进入此界面,界面指针清零
	if(*page_index ==_Card_Option)
	{
        OLED_ShowStr(0,0,(unsigned char *)"_____CardSetting_____",SIZE_0806, NORMAL);
        OLED_ShowStr(0,1,(unsigned char *)" Add_Card            ",SIZE_0806, cur_pos==1?REVERSE:NORMAL);
        OLED_ShowStr(0,3,(unsigned char *)" Change_Card         ",SIZE_0806, cur_pos==2?REVERSE:NORMAL);
        OLED_ShowStr(0,5,(unsigned char *)" Delete_Card         ",SIZE_0806, cur_pos==3?REVERSE:NORMAL);
	}
	// else if(cur_pos<=8)
	// {
	// 	// OLED_Display_String(0,0,8,16,"5.RSSI:",cur_pos==5?REVERSE:NORMAL,NO_ALGIN);
	// 	// OLED_Display_String(0,2,8,16,"6.Esp touch",cur_pos==6?REVERSE:NORMAL,NO_ALGIN);
	// }
}
void Setting_Option_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    Creat_Popup(_Setting_Icron, _Popup, _Popup, _Popup, Developing_Popup, 1000);
    // // OLED_ShowStr(32+0,0,(unsigned char *)"setting_option",SIZE_0806, 1);
    // // OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806, 1);
    // static u8 cur_pos=1;
    // uint8_t option_num = 4;
    // OLED_Clear();
	// if(menu_last_index ==_Setting_Option)//判断是否是第一次进入此界面
	// {
    //     // if(current_key.is_callback == 0){
            
    //         switch(*key_val)
    //         {
    //             case KEY_UP: cur_pos==1?cur_pos=1:cur_pos--;
    //                     break;
    //             case KEY_IN://确定(设置)按键
    //             {
    //                 switch(cur_pos)
    //                 {
    //                     case 1:
    //                             menu_last_index = *page_index;
    //                             *page_index = _Add_Card;
    //                             current_operation_func=MainMenu_Table[func_index].current_operation;
    //                             (*current_operation_func)(&func_index,&current_key.Key_Name);
    //                             break;
    //                     case 2:
    //                             menu_last_index = *page_index;
    //                             *page_index = _Change_Card;
    //                             current_operation_func=MainMenu_Table[func_index].current_operation;
    //                             (*current_operation_func)(&func_index,&current_key.Key_Name);
    //                             break;
    //                     case 3:
    //                             menu_last_index = *page_index;
    //                             *page_index = _Delete_Card;
    //                             current_operation_func=MainMenu_Table[func_index].current_operation;
    //                             (*current_operation_func)(&func_index,&current_key.Key_Name);
    //                             break;
    //                     default:break;
    //                 }
    //             }
    //                     break;
    //             case KEY_DOWN:cur_pos==option_num?cur_pos=option_num:cur_pos++;
    //                     break;
    //             default:break;
    //         }
    //     // }
	// }
	// // else cur_pos=1;//第一次进入此界面,界面指针清零
	// if(*page_index ==_Card_Option)
	// {
    //     OLED_ShowStr(0,0,(unsigned char *)" Brightness ",SIZE_0806, cur_pos==1?REVERSE:NORMAL);
    //     OLED_ShowStr(0,2,(unsigned char *)" Buzzer ",SIZE_0806, cur_pos==2?REVERSE:NORMAL);
	// }
	// else if(cur_pos<=8)
	// {
	// 	// OLED_Display_String(0,0,8,16,"5.RSSI:",cur_pos==5?REVERSE:NORMAL,NO_ALGIN);
	// 	// OLED_Display_String(0,2,8,16,"6.Esp touch",cur_pos==6?REVERSE:NORMAL,NO_ALGIN);
	// }
}
void Info_Option_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    // OLED_ShowStr(32+0,0,(unsigned char *)"info_option",SIZE_0806, 1);
    // OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806, 1);
    Creat_Popup( _Info_Icron, _Popup, _Popup, _Popup, Developing_Popup, 1000);
}
void Add_Card_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    NFC_Event.Add_Card = ON;
    Creat_Popup(_Card_Option, _Popup, _Popup, _Popup, AddCard_Popup, 10);
}
void Change_Card_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    Creat_Popup(_Card_Option, _Popup, _Popup, _Popup, Developing_Popup, 1000);
}
void Delete_Card_Func(u8* page_index,u8* key_val)
{
    // NFC_Event.Delete_Card = ON;
    // OLED_Clear();
    // OLED_ShowStr(28,1,(unsigned char *)"Deleting...",SIZE_0806, 1);
    // eeprom_delete_all();
    // OLED_ShowStr(28,1,(unsigned char *)"Successed",SIZE_0806, 1);
    // msDelay(500);

    // NFC_Event.Delete_Card = OFF;

    // menu_last_index = *page_index;
    // *page_index = _Card_Option;
    // current_operation_func=MainMenu_Table[func_index].current_operation;
    // (*current_operation_func)(&func_index,&current_key.Key_Name);
    static u8 cur_pos=1;//光标
    OLED_Clear();

    if(cur_pos > nfc_uid.len) cur_pos = nfc_uid.len;
    if(nfc_uid.len == 0){
        OLED_ShowStr(52,3,(unsigned char *)"NULL",SIZE_0806, NORMAL);
        return;
    }
    OLED_ShowStr(0,0,(unsigned char *)"__ID____________Name_",SIZE_0806, NORMAL);
    // 创建列表数组
    menuList_Table = Creat_DeleteCard_List();
    // 按键处理
    if(menu_last_index ==_Delete_Card)//判断是否是第一次进入此界面
	{
        switch(*key_val)
        {
            case KEY_UP: cur_pos==1?cur_pos=1:cur_pos--;//cur_pos==1?cur_pos=nfc_uid.len:cur_pos--;
                break;
            case KEY_IN://确定(设置)按键
            {
                NFC_Event.Delete_Card = ON;
                eeprom_delete_card_fast(cur_pos -1);
                NFC_Event.Delete_Card = OFF;
                // 刷新显存列表
                for(u8 i = 0; i < nfc_uid.len; i++) free(menuList_Table[i].str);
                free(menuList_Table);
                if(cur_pos > nfc_uid.len) cur_pos = nfc_uid.len;
                if(nfc_uid.len == 0){
                    OLED_ShowStr(52,3,(unsigned char *)"NULL",SIZE_0806, NORMAL);
                    return;
                }
                OLED_ShowStr(0,0,(unsigned char *)"__ID____________Name_",SIZE_0806, NORMAL);
                menuList_Table = Creat_DeleteCard_List();
                break;
            }
            case KEY_DOWN:cur_pos==nfc_uid.len?cur_pos=nfc_uid.len:cur_pos++;//cur_pos==nfc_uid.len?cur_pos=1:cur_pos++;
                break;
            default:break;
        }
	}
    // 列表显示
    Show_List(menuList_Table, cur_pos, MAX_ITEM, nfc_uid.len);
    // 释放内存
    for(u8 i = 0; i < nfc_uid.len; i++) free(menuList_Table[i].str);
    free(menuList_Table);
}
void Date_Setting_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_ShowStr(32+0,0,(unsigned char *)"date_setting",SIZE_0806, 1);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806, 1);
}
void Time_Setting_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_ShowStr(32+0,0,(unsigned char *)"time_setting",SIZE_0806, 1);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806, 1);
}
void Device_Info_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_ShowStr(32+0,0,(unsigned char *)"Device_Info",SIZE_0806, 1);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806, 1);
}
void Devoloper_Info_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_ShowStr(32+0,0,(unsigned char *)"developer_info",SIZE_0806, 1);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806, 1);
}
void OLED_Lock_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_OFF();
}
void List_Menu_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_ShowStr(32+0,0,(unsigned char *)"_List_Menu",SIZE_0806, 1);
}
void Popup_Func(u8* page_index,u8* key_val)
{
    OLED_Clear();
    OLED_ShowStr(32+0,0,(unsigned char *)"_Popup",SIZE_0806, 1);
}

// 弹窗函数
void Developing_Popup(u8* page_index,u8* key_val)
{
    OLED_ShowStr(2,0,(unsigned char *)" ------------------ ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,1,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,2,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,3,(unsigned char *)" I   Developing   I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,4,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,5,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,6,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,7,(unsigned char *)" ------------------ ",SIZE_0806, NORMAL);
}
void AddCard_Popup(u8* page_index,u8* key_val)
{
    addcard_state state = Add_Card_Error;

    OLED_ShowStr(28,1,(unsigned char *)"Detecting...",SIZE_0806, 1);
    OLED_ShowStr(35,3,(unsigned char *)"IC Card",SIZE_0806, 1);
    state = Add_Card();
    switch (state)
    {
        case Add_Card_OK:
        {
            OLED_ShowStr(35,5,(unsigned char *)" Succeed ",SIZE_0806, 1);
            Popup_Sta.last_time = 0;
            msDelay(300);
            break;
        }
        case Card_Exist:{
            OLED_ShowStr(35,5,(unsigned char *)"Existent ",SIZE_0806, 1);
            break;
        }
        case Add_Card_Error:
        {
            OLED_ShowStr(35,5,(unsigned char *)"Not_Found",SIZE_0806, 1);
            break;
        }
        default:{
            break;
        }
    }

    if(current_key.is_PopupCallback == 0)
    {
        if(menu_last_index == _Popup){
            switch(current_key.Key_Name)
            {
                case KEY_IN:{
                    Popup_Sta.last_time = 0;
                    break;
                }
                case KEY_RIGHT:
                        break;
                case KEY_LEFT:
                        break;
                case KEY_DOWN:
                        break;
                case KEY_UP:
                        break;
                default:break;
            }
        }
        current_key.is_PopupCallback = 1;
    }
}
void popup_test(u8* page_index,u8* key_val)
{
    OLED_ShowStr(2,0,(unsigned char *)" ------------------ ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,1,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,2,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,3,(unsigned char *)" I   popup_test   I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,4,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,5,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,6,(unsigned char *)" I                I ",SIZE_0806, NORMAL);
    OLED_ShowStr(2,7,(unsigned char *)" ------------------ ",SIZE_0806, NORMAL);
}

// void Free_Menu_List(menuList_t list[]){
//     for(u8 i = 0; i < sizeof(list)/sizeof(list[0]); i++)
//     {
//         free(list[i].str);
//     }
//     free(list);
// }
menuList_t* Creat_DeleteCard_List(void)
{
    menuList_t *menuList_Table =(menuList_t*)malloc(nfc_uid.len * sizeof(menuList_t)); //申请数组
    for(uint8_t i = 0; i < nfc_uid.len; i++)
    {
        u8 *cStr = malloc(21*sizeof(u8));
        sprintf((char *)cStr, "%d %02X%02X%02X%02X      %c%c%c  ", i+1, nfc_uid.uid[i][0], nfc_uid.uid[i][1], nfc_uid.uid[i][2], nfc_uid.uid[i][3], nfc_uid.name[i][0], nfc_uid.name[i][1], nfc_uid.name[i][2]);
        menuList_Table[i].len =(uint8_t) 4*6;
        menuList_Table[i].str = cStr;
        // menuList_Table[i].current_operation  = ;
    }
    return menuList_Table;
}

