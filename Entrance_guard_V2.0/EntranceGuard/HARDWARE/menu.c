#include "menu.h"
#include "OLED.h"
#include "key.h"

Menu_table table[30]=
{
	//首页
	{0,1,0,0,(*Show_Watch)},
	
	//第二层
	{ 1,2,5,0,(*Show_Temp_bmp)},  //tmep humi
	{ 2,3,6,0,(*Show_Game_bmp)},  //game
	{ 3,4,7,0,(*Show_Alarm_bmp)}, //alarm
	{ 4,1,8,0,(*Show_Info_bmp)},  //show info
	
	//第三层
	{ 5,5,5,1,(*Show_Temp)},    //temp and humi
	{ 6,6,6,2,(*Game)},         //game
	{ 7,7,7,3,(*Alarm)},        //alarm
	{ 8,8,8,4,(*Show_Info)}     //info
//	{ 9,next,enter,backk,(*)},  //alarm_show
//	{10,next,enter,backk,(*)},  //alarm_add
};

void Show_Watch(void){
	OLED_ShowStr(34+0,2,(unsigned char *)"0000-00-00",1);
	OLED_ShowStr(32+0,4,(unsigned char *)"00:00:00",1);
	
}

void Show_Temp_bmp(void){
	OLED_ShowStr(0,0,(unsigned char *)"temp bmp",1);
}

void Show_Game_bmp(void){
	OLED_ShowStr(0,0,(unsigned char *)"game bmp",1);
}

void Show_Alarm_bmp(void){
	OLED_ShowStr(0,0,(unsigned char *)"alarm bmp",1);
}

void Show_Info_bmp(void){
	OLED_ShowStr(0,0,(unsigned char *)"info bmp",1);
}

void Show_Temp(void){
	OLED_ShowStr(0,0,(unsigned char *)"temp and humi",1);
}

void Game(void){
	OLED_ShowStr(0,0,(unsigned char *)"game start",1);
}

void Alarm(void){
	OLED_ShowStr(0,0,(unsigned char *)"alarm set",1);
}

void Show_Info(void){
	OLED_ShowStr(0,0,(unsigned char *)"all info",1);
}

//菜单显示处理
void Menu_Show(unsigned char key)
{
	extern void (*current_operation_index)();
	extern unsigned char func_index; //主程序此时所在程序的索引值
	extern unsigned char last_index;

	switch (key)
	{
	case KEY_LEFT:
		func_index = table[func_index].back;
		break;
	case KEY_IN:
		func_index = table[func_index].enter;
		break;
	case KEY_DOWM:
		func_index = table[func_index].next;
		break;
	default:
		break;
	}
    current_key.Key_Name = NULL_KEY;
    current_key.Key_Type = KEY_NONE_CLICK;

	if(func_index != last_index)
	{
		current_operation_index = table[func_index].current_operation;
		OLED_Fill_Fast(0x00); //清屏
		(*current_operation_index)();
		
		last_index = func_index;
	}
}

void Watch_Show(void)
{
	extern unsigned char func_index;
	extern unsigned char last_sec;
	
	if(func_index == 0){
		OLED_ShowNum(34+0 ,2,111 ,4,12);
		OLED_ShowNum(34+30,2,222,2,12);
		OLED_ShowNum(34+48,2,222 ,2,12);
		
		OLED_ShowNum(32+0 ,4,444,2,16);
		OLED_ShowNum(32+24,4,555 ,2,16);
		OLED_ShowNum(32+48,4,666 ,2,16);
	}
}

void key_in_single_click_callback()
{
    
    Menu_Show(2);//enter
}

void key_dowm_single_click_callback()
{
    Menu_Show(0);//next
}

void key_in_long_click_callback()
{
    Menu_Show(1); //exit
}
