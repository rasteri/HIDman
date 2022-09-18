
#ifndef __MENU_H__
#define __MENU_H__
#include <stdbool.h>

void Menu_Press_Key(uint8_t key);
extern __xdata bool MenuActive;
extern bool DumpReport;

extern __xdata char SendBuffer[];

extern uint8_t menuKey;

void SendKeyboardBuffer();

#define SendKeyboardString(...) {sprintf(SendBuffer, __VA_ARGS__); SendKeyboardBuffer();}

#endif