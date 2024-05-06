#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "usbhost.h"
#include "defs.h"
#include "type.h"

void HandleRepeats();
void HandleReceived(uint8_t port);
void RepeatTimer();

bool ParseReport(HID_REPORT_DESC *desc, uint32_t len, uint8_t *report);

#define SetRepeatState(st) \
    TR0 = 0;               \
    RepeatState = st;      \
    TR0 = 1;



extern uint16_t StatusMode;

extern uint8_t LEDDelayMs;

#endif //__PROTOCOL_H__