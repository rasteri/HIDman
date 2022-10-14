#ifndef _PARSELASERMOUSEDATA_H_
#define _PARSELASERMOUSEDATA_H_

extern BOOL UsbKeyboardParse(UINT8 *pUsb, UINT8 *pOut, HID_SEG_STRUCT *pKeyboardSegStruct, KEYBOARD_PARSE_STRUCT *pKeyboardParseStruct);

extern BOOL UsbMouseParse(UINT8 *pUsb, UINT8 *pOut, HID_SEG_STRUCT *pMouseDataStruct);

#endif
