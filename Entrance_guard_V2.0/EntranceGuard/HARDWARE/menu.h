#ifndef __MENU_H
#define __MENU_H

typedef struct
{
	unsigned char current;           //当前索引号
	unsigned char next;              //向下
	unsigned char enter;             //确定
	unsigned char back;              //退出
	void (*current_operation)(void); //当前页面操作
} Menu_table;

extern Menu_table table[30];
    
void Show_Watch(void);

void Show_Temp_bmp(void);
void Show_Game_bmp(void);
void Show_Alarm_bmp(void);
void Show_Info_bmp(void);

void Show_Temp(void);
void Game(void);
void Alarm(void);
void Show_Info(void);

//菜单显示处理
void Menu_Show(unsigned char key);
void Watch_Show(void);

void key_in_single_click_callback();
void key_dowm_single_click_callback();
void key_in_long_click_callback();
#endif
