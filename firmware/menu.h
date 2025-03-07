
#ifndef __MENU_H__
#define __MENU_H__
#include <stdbool.h>

#define MENU_STATE_INIT 0
#define MENU_STATE_MAIN 1
#define MENU_STATE_DUMPING 2
#define MENU_STATE_KEYBOARD 3
#define MENU_STATE_MOUSE 4
#define MENU_STATE_GAME 5
#define MENU_STATE_DEBUG 6

void Menu_Press_Key(uint8_t key);
extern __xdata bool MenuActive;
extern __xdata  bool DumpReport;

extern __xdata char SendBuffer[];

extern __xdata  uint8_t menuKey;

void SendKeyboardBuffer(void);
void Menu_Task(void);
extern __xdata uint8_t menuState;
void inputProcess(void);

//#define DEBUGOUT(...) 

#define SendKeyboardString(...) {sprintf(SendBuffer, __VA_ARGS__); SendKeyboardBuffer();}


extern __xdata bool KeyboardDebugOutput;

extern __xdata uint8_t LEDStatus;
#endif