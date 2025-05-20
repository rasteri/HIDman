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
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "mouse.h"
#include "settings.h"
#include "usbhidkeys.h"
#include "system.h"
#include "andyalloc.h"
#include "scancode.h"

__xdata char SendBuffer[255];
__xdata char KeyboardPrintfBuffer[80];


__xdata bool KeyboardDebugOutput = 0;
__xdata bool MenuActive = 0;

#define SEND_STATE_IDLE 0
#define SEND_STATE_SHIFTON 1
#define SEND_STATE_MAKE 2
#define SEND_STATE_BREAK 3
#define SEND_STATE_SHIFTOFF 4

__xdata uint8_t sendBufferState = SEND_STATE_IDLE;
__xdata uint8_t oldSendBufferState = SEND_STATE_IDLE;

 char *currchar;

uint16_t MenuRateLimit = 0;


void Sendbuffer_Task()
{
    // currently limiting, don't send anything yet
    if (MenuRateLimit)
        return;

    oldSendBufferState = sendBufferState;

    switch (sendBufferState)
    {
    case SEND_STATE_IDLE:
        if (*currchar)
        {
            sendBufferState = SEND_STATE_SHIFTON;
        }
        break;
    case SEND_STATE_SHIFTON:
        if (*currchar)
        {
            if (*currchar >= 0x41 && *currchar <= 0x5A)
            {
                if (SendKeyboard((FlashSettings->KeyboardMode == MODE_PS2) ? KEY_SET2_LSHIFT_MAKE : KEY_SET1_LSHIFT_MAKE))
                {
                    sendBufferState = SEND_STATE_MAKE;
                }
            }
            else
            {
                sendBufferState = SEND_STATE_MAKE;
            }
        }
        else
        {
            sendBufferState = SEND_STATE_IDLE;
        }
        break;

    case SEND_STATE_MAKE:
        if(SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Make[ASCIItoHID[*currchar]] : HIDtoSET1_Make[ASCIItoHID[*currchar]]))
        {
            sendBufferState = SEND_STATE_BREAK;
        }
        break;

    case SEND_STATE_BREAK:
        if (SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Break[ASCIItoHID[*currchar]] : HIDtoSET1_Break[ASCIItoHID[*currchar]]))
        {
            sendBufferState = SEND_STATE_SHIFTOFF;
        }
        break;

    case SEND_STATE_SHIFTOFF:
        if (*currchar >= 0x41 && *currchar <= 0x5A)
        {
            if (SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? KEY_SET2_LSHIFT_BREAK : KEY_SET1_LSHIFT_BREAK))
            {
                currchar++;
                sendBufferState = SEND_STATE_SHIFTON;
            }
        }
        else {
            currchar++;
            sendBufferState = SEND_STATE_SHIFTON;
        }

        break;
    }

    if (
        (oldSendBufferState != sendBufferState) &&
        (FlashSettings->MenuRateLimit || FlashSettings->KeyboardMode == MODE_XT || FlashSettings->KeyboardMode == MODE_AMSTRAD)
        )
        MenuRateLimit = 25;
    
}

__xdata bool MenuExiting = 0;

void SendKeyboardBuffer(void)
{
    uint8_t currchar;
    uint8_t BufferIndex = 0;

    while (1)
    {
        SoftWatchdog = 0;
        currchar = SendBuffer[BufferIndex];

        if (!currchar)
        {
            return;
        }

        // capitals, hold shift first
        if (currchar >= 0x41 && currchar <= 0x5A)
            while (!SendKeyboard(
                (FlashSettings->KeyboardMode == MODE_PS2) ? KEY_SET2_LSHIFT_MAKE : KEY_SET1_LSHIFT_MAKE))
                ;

        // press the key
        PressKey(currchar);

        // release the key
        ReleaseKey(currchar);

        // release shift
        if (currchar >= 0x41 && currchar <= 0x5A)
        {
            while (!SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? KEY_SET2_LSHIFT_BREAK : KEY_SET1_LSHIFT_BREAK))
                ;
        }
        BufferIndex++;
    }
}

__xdata uint8_t fuckcount = 1;

__xdata uint8_t menuState = MENU_STATE_INIT;
__xdata uint8_t lastMenuState = MENU_STATE_INIT;

__xdata uint8_t menuKey = 0;

void Menu_Press_Key(uint8_t key)
{
    menuKey = key;
}

void YesNo(bool x)
{
    if (x)
    {
        SendKeyboardString("Yes\n");
    }
    else
    {
        SendKeyboardString("No\n");
    }
}

uint8_t firsttime = 1;

void Menu_Task(void)
{
    if (firsttime) {
        SendBuffer[0] = 0;
        currchar = SendBuffer;
        firsttime = 0;
    }

    Sendbuffer_Task();

    switch (menuState)
    {
        case MENU_STATE_INIT:

            // SendKeyboardString("abababababababababababababababab"); // 32
            // SendKeyboardString("abababababababababababababababababababababababababababababababab"); // 64

            menuState = MENU_STATE_MAIN;
            menuKey = 0;
            break;

        case MENU_STATE_MAIN:
            if (lastMenuState != MENU_STATE_MAIN)
            {
                SendBuffer[0] = 0;
                SendKeyboardString("\n--\nHIDman v1.1.5\n\n");
                SendKeyboardString("1. Key\n");
                SendKeyboardString("2. Mouse\n");
                SendKeyboardString("3. Game\n");
                SendKeyboardString("\n4. Adv.\n\n");
                SendKeyboardString("ESC. Exit\n\n");
                currchar = SendBuffer;
                lastMenuState = menuState;
            }

            // if we've finished printing "Goodbye", disable menu
            if (sendBufferState == SEND_STATE_IDLE && MenuExiting){
                menuState = MENU_STATE_INIT;
                MenuActive = 0;
                lastMenuState = 0;
                MenuExiting = 0;
            }

            switch (menuKey)
            {
                case KEY_1:     menuState = MENU_STATE_KEYBOARD; break;
                case KEY_2:     menuState = MENU_STATE_MOUSE; break;
                case KEY_3:     menuState = MENU_STATE_GAME; break;
                case KEY_4:     menuState = MENU_STATE_ADVANCED; break;

                case KEY_ESC:
                    SendBuffer[0] = 0;
                    SendKeyboardString("Goodbye\n");
                    currchar = SendBuffer;
                    MenuExiting = 1;
                    break;
            }
            
            
            break;

        case MENU_STATE_KEYBOARD:
            if (lastMenuState != MENU_STATE_KEYBOARD){
                SendBuffer[0] = 0;
                SendKeyboardString("\n--\nKeyboard\n\n");
                SendKeyboardString("1. Adv. USB - ");
                YesNo(FlashSettings->KeyboardReportMode);

                SendKeyboardString("2. 83 Key Mode - ");
                YesNo(FlashSettings->XT83Keys);

                SendKeyboardString("\nESC Main Menu\n");
                currchar = SendBuffer;
                lastMenuState = menuState;
            }
            switch (menuKey) {
                case KEY_1:     HMSettings.KeyboardReportMode ^= 1;     SyncSettings(); lastMenuState = 0; break;
                case KEY_2:     HMSettings.XT83Keys ^= 1;               SyncSettings(); lastMenuState = 0; break;
                case KEY_ESC:   menuState = MENU_STATE_MAIN; break;
            }
        break;

        case MENU_STATE_MOUSE:
            if (lastMenuState != MENU_STATE_MOUSE)
            {
                SendBuffer[0] = 0;
                SendKeyboardString("\n--\nMouse\n\n");
                SendKeyboardString("1. Adv. USB - ");
                YesNo(FlashSettings->MouseReportMode);

                SendKeyboardString("2. Intellimouse - ");
                YesNo(FlashSettings->Intellimouse);

                SendKeyboardString("\nESC Main Menu\n");
                currchar = SendBuffer;
                lastMenuState = menuState;
            }

            switch (menuKey)
            {
                case KEY_1:     HMSettings.MouseReportMode ^= 1;        SyncSettings(); lastMenuState = 0; break;
                case KEY_2:     HMSettings.Intellimouse ^= 1;           SyncSettings(); lastMenuState = 0; break;
                case KEY_ESC:   menuState = MENU_STATE_MAIN; break;
            }
            break;

        case MENU_STATE_GAME:
            if (lastMenuState != MENU_STATE_GAME)
            {
                SendBuffer[0] = 0;
                SendKeyboardString("\n--\nGame Controllers\n\n");
                SendKeyboardString("1. Use as Mouse - ");
                YesNo(FlashSettings->GameControllerAsMouse);

                SendKeyboardString("\nESC main menu\n");
                currchar = SendBuffer;

                lastMenuState = menuState;
            }

            switch (menuKey)
            {
                case KEY_1:     HMSettings.GameControllerAsMouse ^= 1;        SyncSettings(); lastMenuState = 0; break;
                case KEY_ESC:   menuState = MENU_STATE_MAIN; break;
            }
            break;

        case MENU_STATE_ADVANCED:
            if (lastMenuState != MENU_STATE_ADVANCED)
            {
                SendBuffer[0] = 0;
                SendKeyboardString("\n--\nAdvanced\n\n");
                SendKeyboardString("1. Factory Reset\n");
                SendKeyboardString("2. Log HID Data\n");
                SendKeyboardString("3. PS2 mouse status\n");
                SendKeyboardString("4. Serial Log - ");
                YesNo(FlashSettings->SerialDebugOutput);
                SendKeyboardString("5. PS2 AUX Output - ");
                YesNo(FlashSettings->EnableAUXPS2);
                SendKeyboardString("6. Menu Rate Limit - ");
                YesNo(FlashSettings->MenuRateLimit);

                //SendKeyboardString("5. Memory Test\n\n");
                SendKeyboardString("\nESC main menu\n");
                currchar = SendBuffer;
                lastMenuState = menuState;
            }
            switch (menuKey)
            {
                case KEY_1:
                    // stop timer0 resetting watchdog
                    ET0 = 0;
                    while(1);
                    break;

                case KEY_2:
                    SendBuffer[0] = 0;
                    SendKeyboardString("ESC to stop, R to redetect\n");
                    currchar = SendBuffer;
                    KeyboardDebugOutput = 1;
                    menuState = MENU_STATE_DUMPING;
                    break;

                case KEY_3:
                    SendBuffer[0] = 0;
                    KeyboardPrintf("Type           %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Type);
                    KeyboardPrintf("Rate           %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Rate);
                    KeyboardPrintf("Resolution     %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Resolution);
                    KeyboardPrintf("Scaling        %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Scaling);
                    KeyboardPrintf("Data reporting %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2DataReporting);
                    KeyboardPrintf("\nCommand buffer\n");
                    for (UINT8 i = 0; i < MOUSE_BUFFER_SIZE; i++)
                    {
                        if (!(i & 0x000F))
                            KeyboardPrintf("\n");
                        KeyboardPrintf("%02X ", MouseBuffer[i]);
                    }
                    currchar = SendBuffer;
                    break;

                case KEY_4:     HMSettings.SerialDebugOutput ^= 1;  SyncSettings(); lastMenuState = 0; break;
                case KEY_5:     HMSettings.EnableAUXPS2 ^= 1;       SyncSettings(); lastMenuState = 0; break;
                case KEY_6:     HMSettings.MenuRateLimit ^= 1;       SyncSettings(); lastMenuState = 0; break;

                case KEY_ESC:   menuState = MENU_STATE_MAIN; break;

                /*case KEY_5:
                    SendKeyboardString("Used %lx, free %lx\n", MemoryUsed(), MemoryFree());
                    SendKeyboardString("Testing allocator, will reset when complete\n");
                    for (int i = 0; i < 128; i++) {
                        andyalloc(256);
                    }
                    DumpReport = 1;
                    menuState = MENU_STATE_DUMPING;
                    break;*/
            }
            break;

        case MENU_STATE_DUMPING:
            // R, reenumerate devices
            if (menuKey == 0x15) 
            {
                ReenumerateAllPorts();
            }

            // ESC, stop dumping
            if (menuKey == 0x29)
            {
                menuState = MENU_STATE_INIT;
                KeyboardDebugOutput = 0;
                break;
            }
            break;
    }
    menuKey = 0;
}

// blue LED on by default
__xdata uint8_t LEDStatus = 0x04;

__xdata int16_t gpiodebounce = 0;

// How long to wait in ms before input event can be triggered again
#define DEBOUNCETIME 25

// How long in ms a button has to be pressed before it's considered held
#define HOLDTIME 2000

// should be run every 1ms
void inputProcess(void)
{

    uint8_t butstate = 0;

    static uint16_t ResetCounter = 0;
#if defined(OSC_EXTERNAL)
    if (!(P3 & (1 << 4)))
    {
#else
    if (!(P4_IN & (1 << 6)))
    {
#endif

        butstate = 1;

        // go into bootloader if user holds button for more than 5 seconds regardless of what else is going on
        ResetCounter++;
        if (ResetCounter > 5000)
        {
            runBootloader();
        }
    }

    else
        ResetCounter = 0;

    // gpiodebounce = 0 when button not pressed
    // > 0 and < DEBOUNCETIME when debouncing positive edge
    // >= DEBOUNCETIME and < HOLDTIME when waiting for release or hold action
    // = HOLDTIME when we register it as a hold action
    // > HOLDTIME when waiting for release
    // > -DEBOUNCETIME and < 0 when debouncing negative edge

    // Button not pressed, check for button
    if (gpiodebounce == 0)
    {
        if (butstate)
        {
            // button pressed

            // start the counter
            gpiodebounce++;
        }
    }

    // Debouncing positive edge, increment value
    else if (gpiodebounce > 0 && gpiodebounce < DEBOUNCETIME)
    {
        gpiodebounce++;
    }

    // debounce finished, keep incrementing until hold reached
    else if (gpiodebounce >= DEBOUNCETIME && gpiodebounce < HOLDTIME)
    {
        // check to see if unpressed
        if (!butstate)
        {

            if (menuState == MENU_STATE_DUMPING)
            {
                // reenumerate = 1;
            }
            else
            {
                // cycle through modes on unpress of button
                HMSettings.KeyboardMode++;
                if (HMSettings.KeyboardMode > 2)
                    HMSettings.KeyboardMode = 0;
                SyncSettings();
                ports[PORT_KEY].state = S_INIT;
            }

            // start the counter
            gpiodebounce = -DEBOUNCETIME;
        }

        else
            gpiodebounce++;
    }
    // Button has been held for a while
    else if (gpiodebounce == HOLDTIME)
    {
        MenuActive = 1;
        gpiodebounce++;
    }

    // Button still holding, check for release
    else if (gpiodebounce > HOLDTIME)
    {
        // Still pressing, do action repeatedly
        if (butstate)
        {
        }
        // not still pressing, debounce release
        else
        {
            // IOevent(i, IOEVENT_HOLDRELEASE);
            //  start the counter
            gpiodebounce = -DEBOUNCETIME;
        }
    }

    // Debouncing negative edge, increment value - will reset when zero is reached
    else if (gpiodebounce < 0)
    {
        gpiodebounce++;
    }
}