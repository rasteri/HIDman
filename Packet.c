
#include "Type.h"
#include "Protocol.h"
#include "Packet.h"

static BOOL BuildPacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 id, UINT8 *dat, UINT8 size)
{
	UINT8 index;
	UINT8 i;
	UINT8 sum;
	
	if (size + 2 > maxLen)
	{
		return FALSE;
	}

	index = 0;
	out[index++] = id;

	sum = id;
	for (i = 0; i < size; i++) 
	{
		out[index++] = *dat;

		sum ^= *dat;

		dat++;
	}
	out[index++] = sum;

	if (pLen != NULL)
	{
		*pLen = index;
	}
	
	return TRUE;
}

BOOL BuildUsbKeyboardPacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 keyboard[KEYBOARD_LEN])
{
	return BuildPacket(out, maxLen, pLen, ID_USB_KEYBOARD, keyboard, KEYBOARD_LEN);
}

BOOL BuildUsbMousePacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 mouse[MOUSE_LEN])
{
	return BuildPacket(out, maxLen, pLen, ID_USB_MOUSE, mouse, MOUSE_LEN);
}

BOOL BuildPs2KeyboardPacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 keyboard[KEYBOARD_LEN])
{
	return BuildPacket(out, maxLen, pLen, ID_PS2_KEYBOARD, keyboard, KEYBOARD_LEN);
}

BOOL BuildPs2MousePacket(UINT8 *out, UINT8 maxLen, UINT8 *pLen, UINT8 mouse[MOUSE_LEN])
{
	return BuildPacket(out, maxLen, pLen, ID_PS2_MOUSE, mouse, MOUSE_LEN);
}

