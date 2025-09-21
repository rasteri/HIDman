#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "usbhost.h"
#include "defs.h"
#include "type.h"

void HandleRepeats(void);
void HandleReceived(uint8_t port);
void RepeatTimer(void);

bool ParseReport(__xdata INTERFACE *interface, uint32_t len, __xdata uint8_t *report);

#define SetRepeatState(st) \
    TR0 = 0;               \
    RepeatState = st;      \
    TR0 = 1;



extern __xdata uint16_t StatusMode;

extern __xdata uint8_t LEDDelayMs;

// Mouse buffer size must be at least 6 for intellimouse detection, more for debugging
#define MOUSE_BUFFER_SIZE 32

extern __xdata uint8_t MouseBuffer[];
extern uint16_t KeyBatCounter;
extern uint16_t MouseBatCounter;
extern __code unsigned char KEY_BATCOMPLETE[];
#endif //__PROTOCOL_H__
