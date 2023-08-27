#include "menu.h"
#include "OLED.h"
#include "key.h"


// Menu_table table[30]=
// {
// 	//首页
// 	{0,1,0,0,(*Show_Watch)},
	
// 	//第二层
// 	{ 1,2,5,0,(*Show_Temp_bmp)},  //tmep humi
// 	{ 2,3,6,0,(*Show_Game_bmp)},  //game
// 	{ 3,4,7,0,(*Show_Alarm_bmp)}, //alarm
// 	{ 4,1,8,0,(*Show_Info_bmp)},  //show info
	
// 	//第三层
// 	{ 5,5,5,1,(*Show_Temp)},    //temp and humi
// 	{ 6,6,6,2,(*Game)},         //game
// 	{ 7,7,7,3,(*Alarm)},        //alarm
// 	{ 8,8,8,4,(*Show_Info)}     //info
// //	{ 9,next,enter,backk,(*)},  //alarm_show
// //	{10,next,enter,backk,(*)},  //alarm_add
// };


static u8 func_index=_Main_UI;//当前页面索引值
static u8 last_index=_Main_UI;//上一个界面索引值
static void (*current_operation_func)(u8,u8);//定义一个函数指针

//索引表
Main_Menu table[20]=
{
	//Cur_Index,          previous,         next,              enter,              back,             (*current_operation)(u8,u8)
	//主界面     
	{_Main_UI,           _Main_UI,         _Main_UI,           _Card_Option,       _OLED_Lock,       Main_UI},
    {_Admin_Login,       _Admin_Login,     _Admin_Login,       _Admin_Login,       _Admin_Login,     Admin_Login_Func},
	//主菜单     
	{_Card_Option,       _Info_Option,     _Setting_Option,    _Add_Card  ,        _Main_UI,         Card_Option_Func},
	{_Setting_Option,    _Card_Option,     _Info_Option,       _Date_Setting,      _Main_UI,         Setting_Option_Func},
	{_Info_Option,       _Setting_Option,  _Card_Option,       _Device_Info,       _Main_UI,         Info_Option_Func},
	
	//子菜单
	{_Add_Card,          _Delete_Card,      _Change_Card,      _Add_Card,          _Card_Option,     Add_Card_Func},
    {_Change_Card,       _Add_Card,         _Delete_Card,      _Delete_Card,       _Card_Option,     Change_Card_Func},
    {_Delete_Card,       _Change_Card,      _Add_Card,         _Delete_Card,       _Card_Option,     Delete_Card_Func},
    
	{_Date_Setting,      _Time_Setting,    _Time_Setting,      _Date_Setting,      _Setting_Option,  Date_Setting_Func},
    {_Time_Setting,      _Date_Setting,    _Date_Setting,      _Time_Setting,      _Setting_Option,  Time_Setting_Func},

	{_Device_Info,       _Devoloper_Info,  _Devoloper_Info,    _Device_Info,       _Info_Option,     Device_Info_Func},
    {_Devoloper_Info,    _Device_Info,     _Device_Info,       _Devoloper_Info,    _Info_Option,     Devoloper_Info_Func},

	{_OLED_Lock,         _Main_UI,         _Main_UI,           _Main_UI,           _Main_UI,         OLED_Lock_Func},
};

void Menu_Init(void)
{
    current_operation_func=table[func_index].current_operation;
	(*current_operation_func)(func_index,current_key.Key_Name);
}
/*
函数功能：刷新界面
参数：无
返回值：无
*/
void GUI_Refresh(void)
{
	if(current_key.Key_Name != NULL_KEY)
	{
		last_index=func_index;//更新上一界面索引值
		switch(current_key.Key_Name)
		{
			case KEY_IN:
                if(current_key.Key_Type==KEY_SINGLE_CLICK) func_index=table[func_index].enter;
                else if(current_key.Key_Type==KEY_LONG_CLICK) func_index=table[func_index].back;
					break;
			case KEY_DOWM:func_index=table[func_index].next;//更新索引值
					break;
			case KEY_UP:func_index=table[func_index].previous;//更新索引值
					break;
			default:break;
		}
        current_key.Key_Name = NULL_KEY;
        current_key.Key_Type = KEY_NONE_CLICK;

		OLED_Fill_Fast(OLED_BACK);
        current_operation_func=table[func_index].current_operation;
	    (*current_operation_func)(func_index,current_key.Key_Name);
	}
	
		
}


void Main_UI(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"Main_UI",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"0000-00-00",SIZE_0806);
    OLED_ShowStr(32+0,4,(unsigned char *)"00:00:00",SIZE_0806);
}
void Admin_Login_Func(u8 page_index,u8 key_val)
{

}
void Card_Option_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"card_option",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Setting_Option_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"setting_option",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Info_Option_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"info_option",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Add_Card_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"add_card",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Change_Card_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"change_card",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Delete_Card_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"card_option",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Date_Setting_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"date_setting",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Time_Setting_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"time_setting",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Device_Info_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"Device_Info",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void Devoloper_Info_Func(u8 page_index,u8 key_val)
{
    OLED_ShowStr(32+0,0,(unsigned char *)"developer_info",SIZE_0806);
    OLED_ShowStr(32+0,2,(unsigned char *)"icron",SIZE_0806);
}
void OLED_Lock_Func(u8 page_index,u8 key_val)
{
    
}
// void Show_Watch(void){
// 	OLED_ShowStr(34+0,2,(unsigned char *)"0000-00-00",1);
// 	OLED_ShowStr(32+0,4,(unsigned char *)"00:00:00",1);
	
// }

// void Show_Temp_bmp(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"temp bmp",1);
// }

// void Show_Game_bmp(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"game bmp",1);
// }

// void Show_Alarm_bmp(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"alarm bmp",1);
// }

// void Show_Info_bmp(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"info bmp",1);
// }

// void Show_Temp(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"temp and humi",1);
// }

// void Game(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"game start",1);
// }

// void Alarm(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"alarm set",1);
// }

// void Show_Info(void){
// 	OLED_ShowStr(0,0,(unsigned char *)"all info",1);
// }

// //菜单显示处理
// void Menu_Show(unsigned char key)
// {
// 	extern void (*current_operation_index)();
// 	extern unsigned char func_index; //主程序此时所在程序的索引值
// 	extern unsigned char last_index;

// 	switch (key)
// 	{
// 	case KEY_LEFT:
// 		func_index = table[func_index].back;
// 		break;
// 	case KEY_IN:
// 		func_index = table[func_index].enter;
// 		break;
// 	case KEY_DOWM:
// 		func_index = table[func_index].next;
// 		break;
// 	default:
// 		break;
// 	}
//     current_key.Key_Name = NULL_KEY;
//     current_key.Key_Type = KEY_NONE_CLICK;

// 	if(func_index != last_index)
// 	{
// 		current_operation_index = table[func_index].current_operation;
// 		OLED_Fill_Fast(0x00); //清屏
// 		(*current_operation_index)();
		
// 		last_index = func_index;
// 	}
// }

// void Watch_Show(void)
// {
// 	extern unsigned char func_index;
// 	extern unsigned char last_sec;
	
// 	if(func_index == 0){
// 		OLED_ShowNum(34+0 ,2,111 ,4,12);
// 		OLED_ShowNum(34+30,2,222,2,12);
// 		OLED_ShowNum(34+48,2,222 ,2,12);
		
// 		OLED_ShowNum(32+0 ,4,444,2,16);
// 		OLED_ShowNum(32+24,4,555 ,2,16);
// 		OLED_ShowNum(32+48,4,666 ,2,16);
// 	}
// }

// void key_in_single_click_callback()
// {
    
//     Menu_Show(2);//enter
// }

// void key_dowm_single_click_callback()
// {
//     Menu_Show(0);//next
// }

// void key_in_long_click_callback()
// {
//     Menu_Show(1); //exit
// }
