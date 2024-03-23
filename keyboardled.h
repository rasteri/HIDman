
#ifndef _KEYBOARDLED_H_
#define _KEYBOARDLED_H_

extern void SetKeyboardLedStatus(UINT8 led);
extern UINT8 GetKeyboardLedStatus(void);
extern void ProcessKeyboardLed(void);
extern void SetKeyboardLedStatusFromPS2(UINT8 ps2led);
#endif

