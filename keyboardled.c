#include "type.h"
#include "mcu.h"
#include "usbhost.h"
#include "keyboardled.h"
#include "usbhidkeys.h"
#include "ps2.h"

static UINT8 keyboardLed = 0x00;
static UINT8 oldLed = 0x00;


void SetKeyboardLedStatus(UINT8 led)
{
	HAL_CRITICAL_STATEMENT(keyboardLed = led);
}

void SetKeyboardLedStatusFromPS2(UINT8 ps2led)
{
	UINT8 hidled = 0x00;
	if (ps2led & PS2_KEY_LED_SCROLLLOCK) hidled |= HID_KEY_LED_SCROLLLOCK;
	if (ps2led & PS2_KEY_LED_NUMLOCK) hidled |= HID_KEY_LED_NUMLOCK;
	if (ps2led & PS2_KEY_LED_CAPSLOCK) hidled |= HID_KEY_LED_CAPSLOCK;

	SetKeyboardLedStatus(hidled);
}

UINT8 GetKeyboardLedStatus(void)
{
	UINT8 led;
	HAL_CRITICAL_STATEMENT(led = keyboardLed);

	return led;
}

void ProcessKeyboardLed(void) {

	UINT8 led = GetKeyboardLedStatus();

	if (oldLed != led){
		UpdateUsbKeyboardLed(led);
		oldLed = led;
	}
}