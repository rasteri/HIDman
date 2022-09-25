
#ifndef _PACKET_H_
#define _PACKET_H_

extern BOOL BuildUsbKeyboardPacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 keyboard[KEYBOARD_LEN]);

extern BOOL BuildUsbMousePacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 mouse[MOUSE_LEN]);

extern BOOL BuildPs2KeyboardPacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 keyboard[KEYBOARD_LEN]);

extern BOOL BuildPs2MousePacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 mouse[MOUSE_LEN]);

#endif

