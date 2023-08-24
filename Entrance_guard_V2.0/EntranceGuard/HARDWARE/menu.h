#ifndef __MENU_H_
#define __MENU_H_

#include "main.h"
#include "lcd_spi_200.h"
#include "menu.h"
#define CHAR_NUM LCD_Height / 24

typedef struct __MENU_T
{
   uint8_t menu_type;
   char name[10];
   struct __LIST_T *list;
   uint8_t ID;
   uint8_t sline;
   uint8_t sTop;
   struct __MENU_T *next;
   struct __MENU_T *prev;
   struct __MENU_T *parentmenu;
} menu_t;
typedef struct __LIST_T
{
   char name[10];
   void *p;
   uint8_t data_type;
   menu_t *parentmenu;
   menu_t *submenu;
   uint8_t ID;
   float diff;
   struct __LIST_T *next;
} list_t;

typedef struct __CURSOR_T
{
   uint16_t SelectLine;
   uint16_t ID;
   list_t* p; 
} cursor_t;
typedef struct __DISPLAY_T
{
   char text[CHAR_NUM];
   uint32_t BackColor;
   uint32_t textColor;
   cursor_t cursor;
   uint8_t all;
   uint8_t top;
   uint8_t end;
   menu_t *menu;
} display_t;

enum
{
   NONE,
   UINT8_E,
   UINT16_E,
   UINT32_E,
   UINT64_E,
   INT8_E,
   INT16_E,
   INT32_E,
   INT64_E,
   DOUBLE_E,
   FLOAT_E,
};
enum
{
   MENU = 0,
   DATA,
   PARA,
};

void LCD_DisplyListLine(list_t *para, display_t *p);
void ListInit(list_t *head, char *name, void *data, float diff, uint8_t type);
uint8_t AddDisplayList(list_t **head, char *name, void *data, float diff, uint8_t type);
uint8_t CreatListNode(list_t **head, char *name, void *data, float diff, uint8_t type, uint8_t ID);
uint8_t RecordListNum(list_t *head, uint8_t *IDarr);
uint8_t DeleteList(list_t **head, uint8_t ID);
void Menu_Init(menu_t *menu, char *name, list_t *list, uint8_t type, uint8_t id);
uint8_t CreatMenuNode(menu_t **head, char *name, list_t *list, uint8_t type, uint8_t id);

extern display_t display;
extern list_t *menu_l;
extern list_t *para_l;
extern list_t *data_l;

extern menu_t *menu;
void menu_proc(display_t *display);
void LCD_DisplyList(display_t *p);
uint8_t AddDisplayList(list_t **head, char *name, void *data, float diff, uint8_t type);
void Init_Menu(menu_t **head, list_t **menu_list, char *name);
void AddMenuMenu(menu_t **head, char *name, list_t *list, uint8_t type, menu_t *submenu);
void display_init();
void ParaIncrease();
void ParaDecrease();
void SelectDown();
void SelectUp();
#define AddMenu(head, name, list, type) AddMenuMenu(head, name, list, type, NULL);
#define AddMuenList(list, name) AddDisplayList(list, name, NULL, 0, NONE)
#define AddDataList(name, data, type) AddDisplayList(&data_l, name, (void *)data, 0, type)
#define AddParaList(name, data, diff, type) AddDisplayList(&para_l, name, (void *)data, diff, type)
#define AddList(list, name, data, diff, type) AddDisplayList(list, name, (void *)data, diff, type)
#endif