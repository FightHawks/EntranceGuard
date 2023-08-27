#ifndef __MENU_H
#define __MENU_H
#include "main.h"

#define MAX_ITEM 4 //每页最大列表数
#define CALLBACK_INTERVAL 10   // 10MS

typedef struct
{
	unsigned char Cur_Index;//当前索引项
	unsigned char previous;//上一页
	unsigned char next;//下一页
	unsigned char enter;//确认
	unsigned char back;//返回
	void (*current_operation)(unsigned char *, unsigned char *);//	当前索引执行的函数(界面)
}mainMenu_t;

typedef struct
{
	uint8_t len;
	uint8_t* str;
	void (*current_operation)();//	当前索执行的函数
}menuList_t;

//各界面的索引值
enum
{
	_Main_UI = 0, //首页
	_Main_Option, //主选择界面（暂时弃用）

    _Admin_Login, //登录界面

	_Card_Icron,
	_Setting_Icron,
	_Info_Icron,

	_Card_Option, //卡片功能选择
    _Setting_Option, //设置功能选择
	_Info_Option, //信息选项

	_Add_Card,
    _Change_Card,
    _Delete_Card,
    _Date_Setting,
    _Time_Setting,
    _Device_Info,
    _Devoloper_Info,
    _OLED_Lock, //熄屏界面

	_List_Menu,
	_Popup,
};

enum
{
	_Top = 0,
	_Left,
	_Right,
	_Button,
};

typedef struct
{
	bool is_exist;
	uint16_t callback_time;
	int32_t last_time;
}popup_sta_t;

void Menu_Init(void);
void GUI_Refresh(void);
void Show_List(menuList_t *menuList_Table, uint8_t cur_pos, uint8_t max_item, uint8_t len);
void Main_UI(u8* page_index,u8* key_val);
void Main_Option_Func(u8* page_index,u8* key_val);
void Admin_Login_Func(u8* page_index,u8* key_val);
void Card_Option_Func(u8* page_index,u8* key_val);
void Setting_Option_Func(u8* page_index,u8* key_val);
void Info_Option_Func(u8* page_index,u8* key_val);
void Add_Card_Func(u8* page_index,u8* key_val);
void Change_Card_Func(u8* page_index,u8* key_val);
void Date_Setting_Func(u8* page_index,u8* key_val);
void Time_Setting_Func(u8* page_index,u8* key_val);
void Device_Info_Func(u8* page_index,u8* key_val);
void Devoloper_Info_Func(u8* page_index,u8* key_val);
void OLED_Lock_Func(u8* page_index,u8* key_val);
void List_Menu_Func(u8* page_index,u8* key_val);
void Popup_Func(u8* page_index,u8* key_val);
void Delete_Card_Func(u8* page_index,u8* key_val);

menuList_t* Creat_DeleteCard_List(void);

void Creat_Popup(uint8_t previous, uint8_t next, uint8_t enter, uint8_t back,void (*current_operation)(u8 *, u8 *), uint16_t last_time);
// void Popup_Thread(popup_sta_t *Popup_Sta, uint8_t Interval_Time);
void Popup_Thread(void);
void Popup_Callback(void);

void Developing_Popup(u8* page_index,u8* key_val);
void AddCard_Popup(u8* page_index,u8* key_val);
void popup_test(u8* page_index,u8* key_val);
 
// extern u8 func_index;//当前页面索引值
// extern u8 menu_last_index;//上一个界面索引值
// extern void (*current_operation_func)(u8*,u8*);//定义一个函数指针

// extern Menu_table table[30];
    
// void Show_Watch(void);

// void Show_Temp_bmp(void);
// void Show_Game_bmp(void);
// void Show_Alarm_bmp(void);
// void Show_Info_bmp(void);

// void Show_Temp(void);
// void Game(void);
// void Alarm(void);
// void Show_Info(void);

// //菜单显示处理
// void Menu_Show(unsigned char key);
// void Watch_Show(void);

// void key_in_single_click_callback();
// void key_dowm_single_click_callback();
// void key_in_long_click_callback();
#endif
