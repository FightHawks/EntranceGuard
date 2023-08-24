#include "menu.h"
#include "lcd_spi_200.h"
#include "motor_inc.h"
#include "key.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "loop.h"
#include "motor_inc.h"
#include "task.h"
static uint8_t listid[256] = {0};
display_t display;
list_t *top_menu_l = NULL;
list_t *para_l = NULL;
list_t *holder_para = NULL;
list_t *target_para = NULL;
list_t *data_l = NULL;
menu_t *top_menu = NULL;
menu_t *menu_m = NULL;
menu_t *para_m = NULL;
menu_t *holder_m = NULL;
;
list_t *holder_l = NULL;
;
list_t *para_x_l = NULL;
list_t *para_y_l = NULL;
extern int16_t testX;
extern int16_t testY;
void display_init()
{
	display.cursor.SelectLine = 1;
	// AddList(&para_x_l, "Pkp", &holder.ppid_x.kp, 0.01, FLOAT_E);
	// AddList(&para_x_l, "Pki", &holder.ppid_x.ki, 0.1, FLOAT_E);
	// AddList(&para_x_l, "Pkd", &holder.ppid_x.kd, 0.1, FLOAT_E);
	// AddList(&para_x_l, "Vkp", &holder.vpid_x.kp, 0.01, FLOAT_E);
	// AddList(&para_x_l, "Vki", &holder.vpid_x.ki, 0.1, FLOAT_E);
	// AddList(&para_x_l, "Vkd", &holder.vpid_x.kd, 0.1, FLOAT_E);

	// AddList(&para_y_l, "Pkp", &holder.ppid_y.kp, 0.01, FLOAT_E);
	// AddList(&para_y_l, "Pki", &holder.ppid_y.ki, 0.1, FLOAT_E);
	// AddList(&para_y_l, "Pkd", &holder.ppid_y.kd, 0.1, FLOAT_E);
	// AddList(&para_y_l, "Vkp", &holder.vpid_y.kp, 0.01, FLOAT_E);
	// AddList(&para_y_l, "Vki", &holder.vpid_y.ki, 0.1, FLOAT_E);
	// AddList(&para_y_l, "Vkd", &holder.vpid_y.kd, 0.1, FLOAT_E);
	AddList(&para_y_l, "X", &testX, 1, INT16_E);
	AddList(&para_y_l, "Y", &testY, 1, INT16_E);
	AddList(&para_x_l, "x1", &hd_task.tholder.border_duty[0].x, 1, INT32_E);
	AddList(&para_x_l, "y1", &hd_task.tholder.border_duty[0].y, 1, INT32_E);
	AddList(&para_x_l, "x2", &hd_task.tholder.border_duty[1].x, 1, INT32_E);
	AddList(&para_x_l, "y2", &hd_task.tholder.border_duty[1].y, 1, INT32_E);
	AddList(&para_x_l, "x3", &hd_task.tholder.border_duty[2].x, 1, INT32_E);
	AddList(&para_x_l, "y3", &hd_task.tholder.border_duty[2].y, 1, INT32_E);
	AddList(&para_x_l, "x4", &hd_task.tholder.border_duty[3].x, 1, INT32_E);
	AddList(&para_x_l, "y4", &hd_task.tholder.border_duty[3].y, 1, INT32_E);
	AddList(&para_x_l, "x5", &hd_task.tholder.border_duty[4].x, 1, INT32_E);
	AddList(&para_x_l, "y5", &hd_task.tholder.border_duty[4].y, 1, INT32_E);
	AddList(&para_x_l, "x6", &hd_task.tholder.border_duty[5].x, 1, INT32_E);
	AddList(&para_x_l, "y6", &hd_task.tholder.border_duty[5].y, 1, INT32_E);
	AddList(&para_x_l, "dt", &hd_task.tholder.line.dt, 1, UINT16_E);
	// AddList(&holder_para, "reverse", &track.reverse, 1, UINT8_E);
	// AddList(&holder_para, "target", &car.target.speed, 1, FLOAT_E);
	// AddList(&target_para, "angle", &car.zero_angle, 1, FLOAT_E);
	// AddList(&target_para, "speed", &car.target.speed, 1, FLOAT_E);
	// AddList(&target_para, "des", &car.target.distance, 10, FLOAT_E);
	// AddDataList("mid_dev", &track.mid_dev, FLOAT_E);
	// AddDataList("sum_l", &track.sum[0], UINT8_E);
	// AddDataList("sum_r", &track.sum[1], UINT8_E);
	// AddDataList("yaw", &JY901.data.angle.yaw, FLOAT_E);
	// AddDataList("inv_time", &JY901.data.time.inv_time, UINT16_E);
	// AddDataList("status", &car_task.status, UINT8_E);
	// AddDataList("runstatus", &car_task.run_status, UINT8_E);
	// AddDataList("type", &car_task.line_type, UINT8_E);
	AddDataList("MVdata0", &MV_data[0], FLOAT_E);
	AddDataList("MVdata1", &MV_data[1], FLOAT_E);
	AddDataList("MVdata2", &MV_data[2], FLOAT_E);
	AddDataList("MVdata3", &MV_data[3], FLOAT_E);
	AddDataList("MVdata4", &MV_data[4], FLOAT_E);
	AddDataList("MVdata5", &MV_data[5], FLOAT_E);
	AddDataList("MVdata6", &MV_data[6], FLOAT_E);
	AddDataList("MVdata7", &MV_data[7], FLOAT_E);
	AddDataList("MVdata8", &MV_data[8], FLOAT_E);
	AddDataList("MVdata9", &MV_data[9], FLOAT_E);

	Init_Menu(&top_menu, &top_menu_l, "MEUN");
	Init_Menu(&para_m, &para_l, "PARA_M");
	Init_Menu(&holder_m, &holder_l, "Holder");
	AddMenu(&holder_m, "X", para_x_l, PARA);
	AddMenu(&holder_m, "Y", para_y_l, PARA);
	AddMenu(&para_m, "target", target_para, PARA);
	AddMenu(&top_menu, "DATA", data_l, DATA);
	AddMenuMenu(&top_menu, "PARA", para_l, MENU, para_m);
	AddMenuMenu(&para_m, "holder", holder_l, MENU, holder_m);
	display.menu = top_menu;
}

void menu_proc(display_t *p)
{
	switch (p->menu->menu_type)
	{
	case MENU:
	case DATA:
	case PARA:
		LCD_DisplyList(p);
		break;
	}
}
void LCD_DisplyListLine(list_t *para, display_t *p)
{

	sprintf(p->text, "                    ");
	switch (para->data_type)
	{
	case NONE:
		sprintf(p->text, "%s", para->name);
		break;
	case UINT8_E:
		sprintf(p->text, "%s: %d", para->name, *((uint8_t *)para->p));
		break;
	case UINT16_E:
		sprintf(p->text, "%s: %d", para->name, *((uint16_t *)para->p));
		break;
	case UINT32_E:
		sprintf(p->text, "%s: %d", para->name, *((uint32_t *)para->p));
		break;
	case UINT64_E:
		sprintf(p->text, "%s: %llu", para->name, *((uint64_t *)para->p));
		break;
	case INT8_E:
		sprintf(p->text, "%s: %d", para->name, *((int8_t *)para->p));
		break;
	case INT16_E:
		sprintf(p->text, "%s: %d", para->name, *((int16_t *)para->p));
		break;
	case INT32_E:
		sprintf(p->text, "%s: %d", para->name, *((int32_t *)para->p));
		break;
	case INT64_E:
		sprintf(p->text, "%s: %lld", para->name, *((int64_t *)para->p));
		break;
	case FLOAT_E:
		sprintf(p->text, "%s: %.4f", para->name, *((float *)para->p));
		break;
	case DOUBLE_E:
		sprintf(p->text, "%s: %.4f", para->name, *((double *)para->p));
		break;
	default:
		break;
	}
}
void LCD_DisplyList(display_t *p)
{
	list_t *head = p->menu->list;
	uint8_t line = 0;
	sprintf(p->text, "       %s      ", p->menu->name);
	LCD_DisplayStringLine(0, p->text);
	p->all = RecordListNum(head, listid);
	if (p->menu->menu_type == MENU && p->top == 0)
	{
		p->top = 1;
	}
	p->end = MIN(p->all - p->top, (LCD_Height / LCD_AsciiFonts->Height) - 1);
	line = p->end;

	for (uint8_t i = 0; i < p->top; i++)
	{
		head = head->next;
	}
	for (uint8_t i = 0; i < line; i++)
	{
		if (p->cursor.SelectLine == i + 1)
		{
			p->textColor = LCD_RED;
			p->cursor.ID = head->ID;
			p->cursor.p = head;
		}
		else
		{
			p->textColor = LCD_WHITE;
		}
		LCD_SetColor(p->textColor);
		LCD_DisplyListLine(head, p);
		LCD_DisplayStringLine(i + 1, p->text);
		head = head->next;
		LCD_SetColor(LCD_WHITE);
	}
}
void ListInit(list_t *head, char *name, void *data, float diff, uint8_t type)
{
	head->next = NULL;
	sprintf(head->name, "%s", name);
	head->p = data;
	head->data_type = type;
	head->diff = diff;
}
uint8_t AddDisplayList(list_t **head, char *name, void *data, float diff, uint8_t type)
{
	uint8_t i = 0;
	RecordListNum(*head, listid);
	for (i = 0; i < 255; i++)
	{
		if (listid[i] == 0)
			break;
	}
	CreatListNode(head, name, data, diff, type, i + 1);
	return i + 1;
}
uint8_t CreatListNode(list_t **head, char *name, void *data, float diff, uint8_t type, uint8_t ID)
{
	list_t *tail = *head;
	list_t *newnode = (list_t *)malloc(sizeof(list_t));
	newnode->next = NULL;
	if (tail == NULL)
	{
		newnode->ID = ID;
		*head = newnode;
		ListInit(newnode, name, data, diff, type);
		return 1;
	}
	else
	{
		while (tail->next != NULL)
		{
			if (tail->ID == ID)
			{
				free(newnode);
				return 0;
			}
			tail = tail->next;
		}
		if (tail->ID == ID)
		{
			free(newnode);
			return 0;
		}
		tail->next = newnode;
		newnode->ID = ID;
		ListInit(newnode, name, data, diff, type);
		return 1;
	}
}

uint8_t DeleteList(list_t **head, uint8_t ID)
{
	list_t *tail = *head;
	list_t **pp = head;
	list_t *p;
	if (tail == NULL)
	{
		free(tail);
		return 1;
	}
	else if (tail->ID == ID)
	{
		*pp = tail->next;
		free(tail);
		return 1;
	}
	else
	{
		while (tail->next->next != NULL)
		{
			if (tail->next->ID == ID)
			{
				p = tail->next->next;
				free(tail->next);
				tail->next = p;
				return 1;
			}
			tail = tail->next;
		}
		if (tail->next->ID == ID)
		{
			free(tail->next);
			tail->next = NULL;
			return 1;
		}
		return 0;
	}
}

uint8_t RecordListNum(list_t *head, uint8_t *IDarr)
{
	list_t *phead = head;
	uint8_t *arr = IDarr;
	uint8_t Num = 0;
	memset(arr, 0, 256);
	if (phead->next == NULL)
	{
		if (phead != NULL)
		{
			*(arr++) = phead->ID;
		}

		return 1;
	}
	while (phead != NULL)
	{
		if (arr != NULL)
		{
			*(arr++) = phead->ID;
		}
		Num++;
		phead = phead->next;
	}
	return Num;
}

void Menu_Init(menu_t *menu, char *name, list_t *list, uint8_t type, uint8_t id)
{
	menu->list = list;
	menu->menu_type = type;
	menu->ID = id;
	sprintf(menu->name, "%s", name);
}
uint8_t CreatMenuNode(menu_t **head, char *name, list_t *list, uint8_t type, uint8_t id)
{
	menu_t *headp = *head;
	menu_t *tail = *head;
	menu_t *newnode = (menu_t *)malloc(sizeof(menu_t));
	newnode->next = NULL;
	if (tail == NULL)
	{
		*head = newnode;
		newnode->next = newnode;
		newnode->prev = newnode;
		newnode->parentmenu = headp;
		Menu_Init(newnode, name, list, type, id);
		return id;
	}
	else
	{
		tail = tail->prev;
		tail->next = newnode;
		newnode->prev = tail;
		newnode->next = headp;
		headp->prev = newnode;
		newnode->parentmenu = headp;
		Menu_Init(newnode, name, list, type, id);
		return id;
	}
}
char *strlowr(char *str)
{
	char *orign = str;
	for (; *str != '\0'; str++)
		*str = tolower(*str);
	return orign;
}
void Init_Menu(menu_t **head, list_t **menu_list, char *name)
{
	menu_t *p = NULL;
	AddMuenList(menu_list, strlowr(name));
	CreatMenuNode(head, name, *menu_list, MENU, 0);
	p = *head;
	p->parentmenu = p;
}
void AddMenuMenu(menu_t **head, char *name, list_t *list, uint8_t type, menu_t *submenu)
{
	menu_t *p = *head;
	list_t *lp = p->list;
	if (type == MENU)
	{
		uint8_t id = CreatMenuNode(head, name, list, type, AddMuenList(&(p->list), strlowr(name)));
		while (1)
		{
			if (lp->ID == id)
			{
				lp->submenu = submenu;
				submenu->parentmenu = p;
				break;
			}
			else
				lp = lp->next;
			if (lp == NULL)
				break;
		}
	}
	else
		CreatMenuNode(head, name, list, type, AddMuenList(&(p->list), strlowr(name)));
}

void ParaIncrease()
{
	list_t *p = display.menu->list;
	switch (display.menu->menu_type)
	{
	case PARA:
		p = display.cursor.p;
		switch (p->data_type)
		{
		case NONE:
			break;
		case UINT8_E:
			(*(uint8_t *)p->p) += p->diff;
			break;
		case UINT16_E:
			(*(uint16_t *)p->p) += p->diff;
			break;
		case UINT32_E:
			(*(uint32_t *)p->p) += p->diff;
			break;
		case UINT64_E:
			(*(uint64_t *)p->p) += p->diff;
			break;
		case INT8_E:
			(*(int8_t *)p->p) += p->diff;
			break;
		case INT16_E:
			(*(int16_t *)p->p) += p->diff;
			break;
		case INT32_E:
			(*(int32_t *)p->p) += p->diff;
			break;
		case INT64_E:
			(*(int64_t *)p->p) += p->diff;
			break;
		case FLOAT_E:
			(*(float *)p->p) += p->diff;
			break;
		case DOUBLE_E:
			(*(double *)p->p) += p->diff;
			break;
		default:
			break;
		}
		break;
	}
}

void ParaDecrease()
{
	list_t *p = display.menu->list;
	switch (display.menu->menu_type)
	{
	case PARA:
		p = display.cursor.p;
		switch (p->data_type)
		{
		case NONE:
			break;
		case UINT8_E:
			(*(uint8_t *)p->p) -= p->diff;
			break;
		case UINT16_E:
			(*(uint16_t *)p->p) -= p->diff;
			break;
		case UINT32_E:
			(*(uint32_t *)p->p) -= p->diff;
			break;
		case UINT64_E:
			(*(uint64_t *)p->p) -= p->diff;
			break;
		case INT8_E:
			(*(int8_t *)p->p) -= p->diff;
			break;
		case INT16_E:
			(*(int16_t *)p->p) -= p->diff;
			break;
		case INT32_E:
			(*(int32_t *)p->p) -= p->diff;
			break;
		case INT64_E:
			(*(int64_t *)p->p) -= p->diff;
			break;
		case FLOAT_E:
			(*(float *)p->p) -= p->diff;
			break;
		case DOUBLE_E:
			(*(double *)p->p) -= p->diff;
			break;
		default:
			break;
		}
		break;
	}
}

void SelectDown()
{
	display.cursor.SelectLine = display.cursor.SelectLine + 1;
	if (display.cursor.SelectLine > display.end)
	{
		display.cursor.SelectLine = display.end;
		if (display.top < display.all - (LCD_Height / LCD_AsciiFonts->Height) + 1)
		{
			display.top += 1;
			display.menu->sTop = display.top;
		}
	}
	display.menu->sline = display.cursor.SelectLine;
}
void SelectUp()
{
	display.cursor.SelectLine = display.cursor.SelectLine - 1;
	if (display.cursor.SelectLine < 1)
	{
		display.cursor.SelectLine = 1;
		if (display.top > 0)
		{
			display.top -= 1;
			display.menu->sTop = display.top;
		}
	}
	display.menu->sline = display.cursor.SelectLine;
}
