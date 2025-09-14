#ifndef __MENU_H__
#define __MENU_H__

#include "modelconfig.h"

extern uint8_t KeyNum;
void menu_init(void);
void UI_Show_Clock(void);
int UI_First_Page(void);
int UI_SettingPage(void);
void UI_Show_SettingPage(void);
int UI_Sliding_Menu(void);
void Stop_Watch_Tick(void);
int Stop_Watch(void);
int LED(void);
int MPU6050(void);
int Game(void);
int Emoji(void);
int Gradienter(void);

#endif
