/*
    menu.c

    Handles the menu that appears when you press HIDman's button
*/


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "ch559.h"
#include "util.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "settings.h"
#include "usbhidkeys.h"

__xdata char SendBuffer[255];

bool DumpReport = 0;

__xdata bool MenuActive = 0;

#define SEND_STATE_IDLE 0
#define SEND_STATE_SHIFTON 1
#define SEND_STATE_MAKE 2
#define SEND_STATE_BREAK 3
#define SEND_STATE_SHIFTOFF 4

uint8_t sendBufferState = SEND_STATE_IDLE;

/*bool Sendbuffer_Task()
{
    bool reEnter = 0;
    do
    {
        reEnter = 0;
        switch (sendBufferState)
        {
        case SEND_STATE_IDLE:
            if (SendBuffer[0])
            {
                sendBufferState = SEND_STATE_SHIFTON;
                reEnter = 1;
            }
            DEBUG_OUT("idle\n");
            break;
        case SEND_STATE_SHIFTON:
            currchar = SendBuffer[BufferIndex];
            if (currchar)
            {
                if (currchar >= 0x41 && currchar <= 0x5A)
                {
                    if (SendKeyboard(KEY_LSHIFT_MAKE))
                    {
                        sendBufferState = SEND_STATE_MAKE;
                        reEnter = 1;
                    }
                }
                else
                {
                    sendBufferState = SEND_STATE_MAKE;
                    reEnter = 1;
                }
            }
            else
            {
                sendBufferState = SEND_STATE_IDLE;
                BufferIndex = 0;
                SendBuffer[0] = 0;
            }
            DEBUG_OUT("Shifton\n");
            break;

        case SEND_STATE_MAKE:
            if (SendKeyboard(HIDtoPS2_Make[ASCIItoHID[currchar]]))
            {
                sendBufferState = SEND_STATE_BREAK;
                reEnter = 1;
            }
            DEBUG_OUT("Make\n");
            break;

        case SEND_STATE_BREAK:
            if (SendKeyboard(HIDtoPS2_Break[ASCIItoHID[currchar]]))
            {
                sendBufferState = SEND_STATE_SHIFTOFF;
                reEnter = 1;
            }
            DEBUG_OUT("Break\n");
            break;

        case SEND_STATE_SHIFTOFF:
            if (SendKeyboard(KEY_LSHIFT_BREAK))
            {
                BufferIndex++;
                sendBufferState = SEND_STATE_SHIFTON;
                reEnter = 1;
            }
            DEBUG_OUT("Shiftoff\n");
            break;
        }
    } while (reEnter);
}*/

void SendKeyboardBuffer()
{
    uint8_t currchar;
    uint8_t BufferIndex = 0;

    while (1)
    {
        currchar = SendBuffer[BufferIndex];

        if (!currchar)
            return;

        if (currchar >= 0x41 && currchar <= 0x5A)
            while (!SendKeyboard(
                (FlashSettings->KeyboardMode == MODE_PS2) ? KEY_LSHIFT_MAKE : XT_KEY_LSHIFT_MAKE
                ))
                delay(10);

        while (!SendKeyboard(
            FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoPS2_Make[ASCIItoHID[currchar]] : HIDtoXT_Make[ASCIItoHID[currchar]]
            ))
            delay(10);
        while (!SendKeyboard(
            FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoPS2_Break[ASCIItoHID[currchar]] : HIDtoXT_Break[ASCIItoHID[currchar]]
            ))
            delay(10);
        if (currchar >= 0x41 && currchar <= 0x5A)
        {
            while (!SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? KEY_LSHIFT_BREAK : XT_KEY_LSHIFT_BREAK))
                delay(10);
        }
        BufferIndex++;
    }
}

uint8_t fuckcount = 1;



uint8_t menuState = MENU_STATE_INIT;

uint8_t menuKey = 0;

void Menu_Press_Key(uint8_t key)
{
    menuKey = key;
}

void Menu_Task()
{
    switch (menuState)
    {
    case MENU_STATE_INIT:
        SendKeyboardString("\n\nHIDMAN v0.1 Main Menu\n\n");
        SendKeyboardString("1. Configure game controller mappings\n");
        SendKeyboardString("2. Log HID Data\n");
        SendKeyboardString("4. Advanced USB Keyboard - ");
        if (FlashSettings->KeyboardReportMode){ SendKeyboardString("Yes\n");} else {SendKeyboardString("No\n");}
        SendKeyboardString("5. Advanced USB Mouse - ");
        if (FlashSettings->MouseReportMode) {SendKeyboardString("Yes\n");} else {SendKeyboardString("No\n");}
        SendKeyboardString("6. Intellimouse Support - ");
        if (FlashSettings->Intellimouse) {SendKeyboardString("Yes\n");} else {SendKeyboardString("No\n");}
        SendKeyboardString("ESC to exit menu\n\n");
        
        menuState = MENU_STATE_MAIN;
        menuKey = 0;
        break;
    case MENU_STATE_MAIN:
        if (menuKey)
        {
            switch (menuKey)
            {
            case KEY_1:
                SendKeyboardString("Not Implemented\n");
                menuState = MENU_STATE_INIT;
                break;

            case KEY_2:
                SendKeyboardString("Logging HID Data. Press ESC to stop...\n");
                DumpReport = 1;
                menuState = MENU_STATE_DUMPING;
                break;

            case KEY_4:
                HMSettings.KeyboardReportMode ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_5:
                HMSettings.MouseReportMode ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_6:
                HMSettings.Intellimouse ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_ESC: // ESC
                SendKeyboardString("Goodbye\n");
                menuState = MENU_STATE_INIT;
                MenuActive = 0;
                break;
            }
            menuKey = 0;
        }
        break;

    case MENU_STATE_DUMPING:
        if (menuKey == 0x29)
        {
            menuState = MENU_STATE_INIT;
            DumpReport = 0;
            break;
        }
        break;
    }
}
