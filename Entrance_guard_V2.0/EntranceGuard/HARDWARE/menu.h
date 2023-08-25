#ifndef __MENU_H
#define __MENU_H
#include "main.h"

#define ON  1
#define OFF 0

// typedef struct
// {
// 	unsigned char current;           //��ǰ������
// 	unsigned char next;              //����
// 	unsigned char enter;             //ȷ��
// 	unsigned char back;              //�˳�
// 	void (*current_operation)(void); //��ǰҳ�����
// } Menu_table;

typedef struct
{
	unsigned char Cur_Index;//��ǰ������
	unsigned char previous;//��һҳ
	unsigned char next;//��һҳ
	unsigned char enter;//ȷ��
	unsigned char back;//����
	void (*current_operation)(unsigned char,unsigned char, unsigned char);//	��ǰ����ִ�еĺ���(����)
}Main_Menu;

//�����������ֵ
enum
{
	_Main_UI = 0,
    _Admin_Login, 
	_Card_Option,
    _Setting_Option,
	_Info_Option,
	_Add_Card,
    _Change_Card,
    _Delete_Card,
    _Date_Setting,
    _Time_Setting,
    _Device_Info,
    _Devoloper_Info,
    _OLED_Lock,
};

void Menu_Init(void);
void GUI_Refresh(void);

void Main_UI(u8 page_index,u8 key_val);
void Admin_Login_Func(u8 page_index,u8 key_val);
void Card_Option_Func(u8 page_index,u8 key_val);
void Setting_Option_Func(u8 page_index,u8 key_val);
void Info_Option_Func(u8 page_index,u8 key_val);
void Add_Card_Func(u8 page_index,u8 key_val);
void Change_Card_Func(u8 page_index,u8 key_val);
void Delete_Card_Func(u8 page_index,u8 key_val);
void Date_Setting_Func(u8 page_index,u8 key_val);
void Time_Setting_Func(u8 page_index,u8 key_val);
void Device_Info_Func(u8 page_index,u8 key_val);
void Devoloper_Info_Func(u8 page_index,u8 key_val);
void OLED_Lock_Func(u8 page_index,u8 key_val);
 
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

// //�˵���ʾ����
// void Menu_Show(unsigned char key);
// void Watch_Show(void);

// void key_in_single_click_callback();
// void key_dowm_single_click_callback();
// void key_in_long_click_callback();
#endif
