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

void PressKey(uint8_t currchar) {
    while (!SendKeyboard(
        FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoPS2_Make[ASCIItoHID[currchar]] : HIDtoXT_Make[ASCIItoHID[currchar]]
        ));
}

void ReleaseKey(uint8_t currchar) {
    while (!SendKeyboard(
        FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoPS2_Break[ASCIItoHID[currchar]] : HIDtoXT_Break[ASCIItoHID[currchar]]
        ));
}

void SendKeyboardBuffer(void)
{
    uint8_t currchar;
    uint8_t BufferIndex = 0;

    while (1)
    {
        currchar = SendBuffer[BufferIndex];

        if (!currchar){
            return;
        }

        // capitals, hold shift first
        if (currchar >= 0x41 && currchar <= 0x5A)
            while (!SendKeyboard(
                (FlashSettings->KeyboardMode == MODE_PS2) ? KEY_LSHIFT_MAKE : XT_KEY_LSHIFT_MAKE
                ));

        // press the key
        PressKey(currchar);

        // release the key
        ReleaseKey(currchar);

        // release shift
        if (currchar >= 0x41 && currchar <= 0x5A)
        {
            while (!SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? KEY_LSHIFT_BREAK : XT_KEY_LSHIFT_BREAK));
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

void YesNo(bool x){
    if (x) {SendKeyboardString("Yes\n");} else {SendKeyboardString("No\n");}
}

bool DebugMenu = 0;

void Menu_Task(void)
{
    switch (menuState)
    {
    case MENU_STATE_INIT:
        SendKeyboardString("\n\nHIDMAN v1.0 Main Menu\n\n");
        SendKeyboardString("1. Adv.Keyboard - ");
        YesNo(FlashSettings->KeyboardReportMode);

        SendKeyboardString("2. Adv.Mouse - ");
        YesNo(FlashSettings->MouseReportMode);

        SendKeyboardString("3. Intellimouse - ");
        YesNo(FlashSettings->Intellimouse);

        if (DebugMenu){
            SendKeyboardString("\n6. Simulate Hardlock\n");
            SendKeyboardString("7. Simulate Softlock\n");
            SendKeyboardString("8. Log HID Data\n");
            SendKeyboardString("9. Dump PS2 mouse status\n");
        }
        else SendKeyboardString("\n5. Debug\n");

        SendKeyboardString("ESC to exit menu\n\n");

        //SendKeyboardString("abababababababababababababababab"); // 32
        //SendKeyboardString("abababababababababababababababababababababababababababababababab"); // 64
        
        menuState = MENU_STATE_MAIN;
        menuKey = 0;
        break;
    case MENU_STATE_MAIN:
        if (menuKey)
        {
            switch (menuKey)
            {

            case KEY_1:
                HMSettings.KeyboardReportMode ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_2:
                HMSettings.MouseReportMode ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_3:
                HMSettings.Intellimouse ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_4:
                HMSettings.Intellimouse ^= 1;
                SyncSettings();
                menuState = MENU_STATE_INIT;
                break;

            case KEY_5:
                DebugMenu = 1;
                menuState = MENU_STATE_INIT;
                break;

            case KEY_6:
                // stop timer0 resetting watchdog
                ET0 = 0;
            case KEY_7:
                // trigger a watchdog reset by hanging around
                while(1);
                break;

            case KEY_8:
                SendKeyboardString("Logging HID Data. Press ESC to stop...\n");
                DumpReport = 1;
                menuState = MENU_STATE_DUMPING;
                break;

            case KEY_9:
				SendKeyboardString("Type           %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Type);
				SendKeyboardString("Rate           %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Rate);
				SendKeyboardString("Resolution     %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Resolution);
				SendKeyboardString("Scaling        %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2Scaling);
				SendKeyboardString("Data reporting %u\n", (&OutputMice[MOUSE_PORT_PS2])->Ps2DataReporting);
				SendKeyboardString("\nCommand buffer\n");
				for (UINT8 i=0; i<MOUSE_BUFFER_SIZE; i++) {
					if (!(i & 0x000F))
						SendKeyboardString("\n");
					SendKeyboardString("%02X ", MouseBuffer[i]);
				}
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

// blue LED on by default
uint8_t LEDStatus = 0x04;

int16_t gpiodebounce = 0;

// How long to wait in ms before input event can be triggered again
#define DEBOUNCETIME 25

// How long in ms a button has to be pressed before it's considered held
#define HOLDTIME 2000

//should be run every 1ms
void inputProcess(void) {

	uint8_t butstate = 0;

	static uint16_t ResetCounter = 0;
#if defined(OSC_EXTERNAL)
	if (!(P3 & (1 << 4))){
#else
	if (!(P4_IN & (1 << 6))){
#endif

		butstate = 1;

		// go into bootloader if user holds button for more than 5 seconds regardless of what else is going on
		ResetCounter++;
		if (ResetCounter > 5000) {
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
	if (gpiodebounce == 0) {
		if (butstate) {
			// button pressed


			// start the counter
			gpiodebounce++;
		}
	}

	// Debouncing positive edge, increment value
	else if (gpiodebounce > 0 && gpiodebounce < DEBOUNCETIME) {
		gpiodebounce++;
	}

	// debounce finished, keep incrementing until hold reached
	else if (gpiodebounce >= DEBOUNCETIME && gpiodebounce < HOLDTIME) {
		// check to see if unpressed
		if (!butstate) {

			if (menuState == MENU_STATE_DUMPING){
				//reenumerate = 1;
			}
			else {
				// cycle through modes on unpress of button
				HMSettings.KeyboardMode++;
				if (HMSettings.KeyboardMode > 1)
					HMSettings.KeyboardMode = 0;
				SyncSettings();
			}


			// start the counter
			gpiodebounce = -DEBOUNCETIME;
		}

		else
			gpiodebounce++;
	}
	// Button has been held for a while
	else if (gpiodebounce == HOLDTIME) {
		MenuActive = 1;
		gpiodebounce++;
	}

	// Button still holding, check for release
	else if (gpiodebounce > HOLDTIME) {
		// Still pressing, do action repeatedly
		if (butstate) {
		}
		// not still pressing, debounce release
		else {
			//IOevent(i, IOEVENT_HOLDRELEASE);
			// start the counter
			gpiodebounce = -DEBOUNCETIME;
		}
	}

	// Debouncing negative edge, increment value - will reset when zero is reached
	else if (gpiodebounce < 0) {
		gpiodebounce++;
	}



}