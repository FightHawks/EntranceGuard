#ifndef __MENU_H
#define __MENU_H

typedef struct
{
	unsigned char current;           //��ǰ������
	unsigned char next;              //����
	unsigned char enter;             //ȷ��
	unsigned char back;              //�˳�
	void (*current_operation)(void); //��ǰҳ�����
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

//�˵���ʾ����
void Menu_Show(unsigned char key);
void Watch_Show(void);

void key_in_single_click_callback();
void key_dowm_single_click_callback();
void key_in_long_click_callback();
#endif
